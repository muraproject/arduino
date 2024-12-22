//1 oktaf tanpa buffer
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
static uint32_t userTempo = 500000; // tempo yang diset user (tidak akan reset)
uint32_t tempo = userTempo; // tempo yang bisa diubah oleh MIDI
uint16_t ticksPerBeat = 480; // default PPQN

// Declare client global
WiFiClient client;
HTTPClient http;

// Serial command flag
bool isPlaying = false;

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
    
    isPlaying = true;
    playMIDI();
}

// Handle incoming serial commands
void processSerialCommands() {
    if (Serial.available() >= 4) {
        char cmd = Serial.read();
        
        if (cmd == 't') {
            // Read 3 bytes of tempo data
            byte tempo_high = Serial.read();
            byte tempo_mid = Serial.read();
            byte tempo_low = Serial.read();
            
            // Update both user tempo and current tempo
            userTempo = ((uint32_t)tempo_high << 16) | 
                       ((uint32_t)tempo_mid << 8) | 
                       tempo_low;
            tempo = userTempo; // Set current tempo to user tempo
                   
            Serial.printf("User tempo set to: %d microseconds per beat\n", userTempo);
        }
    }
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
        // Hanya update tempo MIDI jika tidak ada user tempo
        if (tempo == 500000) { // default tempo
            tempo = 0;
            for(int i = 0; i < 3; i++) {
                tempo = (tempo << 8) | stream->read();
            }
            Serial.printf("MIDI Tempo set to: %d microseconds per beat\n", tempo);
        } else {
            // Skip tempo MIDI jika user sudah set tempo
            for(int i = 0; i < 3; i++) stream->read();
        }
    }
}

void playMIDI() {
    while(isPlaying) {
        // Check for serial commands
        processSerialCommands();
        
        if(WiFi.status() == WL_CONNECTED) {
            http.begin(client, baseURL + "song1.mid");
            int httpCode = http.GET();
            
            if(httpCode == HTTP_CODE_OK) {
                WiFiClient* stream = http.getStreamPtr();
                
                // Skip MIDI header
                for(int i = 0; i < 14; i++) stream->read();
                
                lastEventTime = micros();
                midiTick = 0;
                
                // Reset tempo to user tempo at start of song
                tempo = userTempo;
                
                while(stream->available() && isPlaying) {
                    processSerialCommands();
                    
                    uint32_t deltaTime = readVarLen(stream);
                    
                    if(deltaTime > 0) {
                        unsigned long waitTime = (deltaTime * tempo) / ticksPerBeat;
                        unsigned long startWait = micros();
                        while(micros() - startWait < waitTime && isPlaying) {
                            processSerialCommands();
                        }
                        lastEventTime = micros();
                    }
                    
                    uint8_t status = stream->read();
                    
                    if(status == 0xFF) {
                        uint8_t type = stream->read();
                        if(type == 0x51) {
                            processTempoEvent(stream);
                        } else {
                            uint8_t len = stream->read();
                            for(int i = 0; i < len; i++) stream->read();
                        }
                    }
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
    processSerialCommands();
}
