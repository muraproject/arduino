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
static uint32_t userTempo = 500000;
uint32_t tempo = userTempo;
uint16_t ticksPerBeat = 480;

// Oktaf setting (default oktaf 4: midi notes 60-71)
static uint8_t selectedOctave = 4;

// Global variables
WiFiClient client;
HTTPClient http;
bool isPlaying = false;

void setup() {
    Serial.begin(115200);
    
    // Setup horn pins
    for (int i = 0; i < 6; i++) {
        pinMode(hornPins[i], OUTPUT);
        digitalWrite(hornPins[i], LOW);
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
    Serial.printf("Selected Octave: %d\n", selectedOctave);
    
    isPlaying = true;
    playMIDI();
}

void processSerialCommands() {
    if (Serial.available()) {
        char cmd = Serial.read();
        
        if (cmd == 't' && Serial.available() >= 3) {
            byte tempo_high = Serial.read();
            byte tempo_mid = Serial.read();
            byte tempo_low = Serial.read();
            
            userTempo = ((uint32_t)tempo_high << 16) | 
                       ((uint32_t)tempo_mid << 8) | 
                       tempo_low;
            tempo = userTempo;
            Serial.printf("Tempo set to: %d microseconds per beat\n", userTempo);
        }
        else if (cmd == 'o' && Serial.available() >= 1) {
            byte octave = Serial.read() - '0';
            if (octave >= 3 && octave <= 5) {
                selectedOctave = octave;
                Serial.printf("Octave set to: %d\n", selectedOctave);
            }
        }
    }
}

uint32_t readVarLen(WiFiClient* stream) {
    uint32_t value = 0;
    uint8_t byte;
    int bytesRead = 0;
    
    do {
        while(!stream->available()) delay(1);
        byte = stream->read();
        value = (value << 7) | (byte & 0x7F);
        bytesRead++;
    } while (byte & 0x80 && bytesRead < 4);
    
    return value;
}

void playMIDI() {
    while(isPlaying) {
        processSerialCommands();
        
        if(WiFi.status() == WL_CONNECTED) {
            http.begin(client, baseURL + "song1.mid");
            int httpCode = http.GET();
            
            if(httpCode == HTTP_CODE_OK) {
                WiFiClient* stream = http.getStreamPtr();
                Serial.println("Starting MIDI playback");
                
                // Skip MIDI header
                for(int i = 0; i < 14; i++) {
                    while(!stream->available()) delay(1);
                    stream->read();
                }
                
                lastEventTime = micros();
                tempo = userTempo;
                
                while(stream->available() || stream->connected()) {
                    processSerialCommands();
                    
                    if(!stream->available()) {
                        delay(1);
                        continue;
                    }
                    
                    uint32_t deltaTime = readVarLen(stream);
                    
                    if(deltaTime > 0) {
                        unsigned long waitTime = (deltaTime * tempo) / ticksPerBeat;
                        unsigned long targetTime = lastEventTime + waitTime;
                        while(micros() < targetTime) {
                            if(Serial.available()) processSerialCommands();
                            delay(1);
                        }
                        lastEventTime = targetTime;
                    }
                    
                    uint8_t status = stream->read();
                    
                    if(status == 0xFF) {
                        uint8_t type = stream->read();
                        uint8_t len = stream->read();
                        
                        if(type == 0x51 && len == 3) {
                            uint32_t newTempo = 0;
                            for(int i = 0; i < 3; i++) {
                                while(!stream->available()) delay(1);
                                newTempo = (newTempo << 8) | stream->read();
                            }
                            if(tempo == 500000) {
                                tempo = newTempo;
                                Serial.printf("MIDI Tempo: %d\n", tempo);
                            }
                        } else {
                            for(int i = 0; i < len; i++) {
                                while(!stream->available()) delay(1);
                                stream->read();
                            }
                        }
                    }
                    else if((status & 0xF0) == 0x90 || (status & 0xF0) == 0x80) {
                        uint8_t note = stream->read();
                        uint8_t velocity = stream->read();
                        
                        uint8_t noteOctave = (note / 12) - 1;
                        
                        if (noteOctave == selectedOctave) {
                            int noteBase = note % 12;
                            int hornIndex = -1;
                            
                            switch(noteBase) {
                                case 0:  hornIndex = 0; break;  // C (Do)
                                case 2:  hornIndex = 1; break;  // D (Re)
                                case 4:  hornIndex = 2; break;  // E (Mi)
                                case 5:  hornIndex = 3; break;  // F (Fa)
                                case 7:  hornIndex = 4; break;  // G (Sol)
                                case 9:  hornIndex = 5; break;  // A (La)
                            }
                            
                            if(hornIndex >= 0) {
                                if((status & 0xF0) == 0x90 && velocity > 0) {
                                    digitalWrite(hornPins[hornIndex], HIGH);
                                    Serial.printf("Horn %d ON (Note: %d, Oct: %d)\n", 
                                                hornIndex + 1, note, noteOctave);
                                } else {
                                    digitalWrite(hornPins[hornIndex], LOW);
                                    Serial.printf("Horn %d OFF (Note: %d, Oct: %d)\n", 
                                                hornIndex + 1, note, noteOctave);
                                }
                            }
                        }
                    }
                }
                Serial.println("MIDI playback complete");
            } else {
                Serial.printf("HTTP Error: %d\n", httpCode);
            }
            http.end();
        }
        
        for(int i = 0; i < 6; i++) {
            digitalWrite(hornPins[i], LOW);
        }
        delay(2000);
    }
}

void loop() {
    processSerialCommands();
}
