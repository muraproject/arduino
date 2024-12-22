#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Tenda_F68790";
const char* password = "12345678";

// Server URL 
const String baseURL = "http://myproject123.com/telolet/";

// Pin GPIO untuk klakson
const int hornPins[6] = {5, 18, 19, 21, 22, 23};

// MIDI timing variables
unsigned long midiTick = 0;
unsigned long lastEventTime = 0;
uint32_t tempo = 500000; // default tempo (120 BPM)
uint16_t ticksPerBeat = 480; // default PPQN

// Declare client global
WiFiClient client;
HTTPClient http;

// Structure untuk MIDI event
struct MIDIEvent {
    unsigned long deltaTime;
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
};

void setup() {
    Serial.begin(115200);
    
    // Setup horn pins
    for (int i = 0; i < 6; i++) {
        pinMode(hornPins[i], OUTPUT);
        digitalWrite(hornPins[i], LOW);
    }

    // Connect WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
    
    playMIDI();
}

uint32_t readVarLen(WiFiClient* stream) {
    uint32_t value = 0;
    uint8_t byte;
    
    do {
        byte = stream->read();
        value = (value << 7) | (byte & 0x7F);
    } while (byte & 0x80);
    
    return value;
}

void processTempoEvent(WiFiClient* stream) {
    uint8_t len = stream->read();
    if(len == 3) {
        tempo = 0;
        for(int i = 0; i < 3; i++) {
            tempo = (tempo << 8) | stream->read();
        }
        Serial.printf("Tempo set to: %d microseconds per beat\n", tempo);
    }
}

void playMIDI() {
    while(true) {
        if(WiFi.status() == WL_CONNECTED) {
            http.begin(client, baseURL + "song1.mid");
            int httpCode = http.GET();
            
            if(httpCode == HTTP_CODE_OK) {
                WiFiClient* stream = http.getStreamPtr();
                
                // Skip MIDI header
                for(int i = 0; i < 14; i++) stream->read();
                
                lastEventTime = micros();
                midiTick = 0;
                
                while(stream->available()) {
                    // Read delta time
                    uint32_t deltaTime = readVarLen(stream);
                    
                    // Calculate wait time
                    if(deltaTime > 0) {
                        unsigned long waitTime = (deltaTime * tempo) / ticksPerBeat;
                        while(micros() - lastEventTime < waitTime) {
                            // Wait for correct timing
                        }
                        lastEventTime = micros();
                    }
                    
                    // Read event
                    uint8_t status = stream->read();
                    
                    // Meta event
                    if(status == 0xFF) {
                        uint8_t type = stream->read();
                        if(type == 0x51) { // Tempo change
                            processTempoEvent(stream);
                        } else {
                            // Skip other meta events
                            uint8_t len = stream->read();
                            for(int i = 0; i < len; i++) stream->read();
                        }
                    }
                    // Note events
                    else if((status & 0xF0) == 0x90 || (status & 0xF0) == 0x80) {
                        uint8_t note = stream->read();
                        uint8_t velocity = stream->read();
                        
                        int hornIndex = note - 60;
                        if(hornIndex >= 0 && hornIndex < 6) {
                            if((status & 0xF0) == 0x90 && velocity > 0) {
                                digitalWrite(hornPins[hornIndex], HIGH);
                                Serial.printf("Horn %d ON\n", hornIndex + 1);
                            } else {
                                digitalWrite(hornPins[hornIndex], LOW);
                                Serial.printf("Horn %d OFF\n", hornIndex + 1);
                            }
                        }
                    }
                }
            } else {
                Serial.printf("HTTP Error: %d\n", httpCode);
            }
            http.end();
        }
        
        // Reset all horns
        for(int i = 0; i < 6; i++) {
            digitalWrite(hornPins[i], LOW);
        }
        delay(2000);
    }
}

void loop() {
    // Empty - everything happens in playMIDI()
}
