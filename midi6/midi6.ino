#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Konfigurasi Access Point
const char* ssid = "ESP32-FileManager";
const char* password = "12345678";

// Pin GPIO untuk klakson
const int hornPins[6] = {5, 18, 19, 21, 22, 23};

// MIDI timing variables
unsigned long midiTick = 0;
unsigned long lastEventTime = 0;
static uint32_t userTempo = 500000; // tempo yang diset user
uint32_t tempo = userTempo; 
uint16_t ticksPerBeat = 480; // default PPQN

WebServer server(80);
File uploadFile;
File currentMidiFile;
bool isPlaying = false;
String currentFileName = "";

// Task handle untuk MIDI player
TaskHandle_t MidiPlayerTask;

// Function declarations
void handleRoot();
void handleFileUpload();
void handleFile();
void handleDelete();
void handlePlayMidi();
void handleStopMidi();
void listFiles();
uint32_t readVarLen(File* file);
void midiPlayerTask(void * parameter);

void setup() {
  Serial.begin(115200);
  
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Setup horn pins
  for (int i = 0; i < 6; i++) {
    pinMode(hornPins[i], OUTPUT);
    digitalWrite(hornPins[i], LOW);
  }

  // Configure AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Setup web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.send(200);
  }, handleFileUpload);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/file", HTTP_GET, handleFile);
  server.on("/play", HTTP_POST, handlePlayMidi);
  server.on("/stop", HTTP_POST, handleStopMidi);

  server.begin();

  // Create MIDI player task on core 0
  xTaskCreatePinnedToCore(
    midiPlayerTask,    // Task function
    "MidiPlayer",      // Name
    10000,            // Stack size
    NULL,             // Parameters
    1,                // Priority
    &MidiPlayerTask,  // Task handle
    0                 // Core ID (0)
  );
}

void loop() {
  // Web server runs on core 1
  server.handleClient();
  delay(2);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>ESP32 MIDI File Manager</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;margin:20px;background:#f0f0f0}";
  html += ".container{max-width:800px;margin:0 auto;background:white;padding:20px;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}";
  html += "table{width:100%;border-collapse:collapse;margin-top:20px}";
  html += "th,td{padding:12px;text-align:left;border-bottom:1px solid #ddd}";
  html += "th{background-color:#4CAF50;color:white}";
  html += "tr:hover{background-color:#f5f5f5}";
  html += ".upload-form{margin:20px 0;padding:20px;background:#f9f9f9;border-radius:5px}";
  html += "button{background-color:#4CAF50;color:white;padding:10px 15px;border:none;border-radius:4px;cursor:pointer;margin-right:5px}";
  html += "button.delete{background-color:#f44336}";
  html += "button.play{background-color:#2196F3}";
  html += "button.stop{background-color:#ff9800}";
  html += "</style></head><body><div class='container'>";
  
  html += "<h2>ESP32 MIDI File Manager</h2>";
  
  // Upload form
  html += "<div class='upload-form'>";
  html += "<form action='/upload' method='post' enctype='multipart/form-data'>";
  html += "<input type='file' name='file' accept='.mid,.midi' required>";
  html += "<button type='submit'>Upload File</button>";
  html += "</form></div>";
  
  // File table
  html += "<table><tr><th>Filename</th><th>Size</th><th>Actions</th></tr>";
  
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  
  while(file) {
    String fileName = file.name();
    if(fileName.startsWith("/")) fileName = fileName.substring(1);
    
    // Only show .mid files
    if(fileName.endsWith(".mid") || fileName.endsWith(".midi")) {
      html += "<tr>";
      html += "<td>" + fileName + "</td>";
      html += "<td>" + String(file.size()) + " bytes</td>";
      html += "<td>";
      
      // Add play button
      html += "<button class='play' onclick='playMidi(\"" + fileName + "\")'>Play</button>";
      
      // Add stop button
      html += "<button class='stop' onclick='stopMidi()'>Stop</button>";
      
      // Download button
      html += "<button onclick='window.location.href=\"/file?name=" + fileName + "\"'>Download</button>";
      
      // Delete form
      html += "<form action='/delete' method='post' style='display:inline'>";
      html += "<input type='hidden' name='file' value='" + fileName + "'>";
      html += "<button type='submit' class='delete'>Delete</button>";
      html += "</form></td></tr>";
    }
    file = root.openNextFile();
  }
  root.close();
  
  html += "</table>";
  
  // Add tempo control
  html += "<div class='control-panel' style='margin: 20px 0; padding: 20px; background: #f9f9f9; border-radius: 5px;'>";
  html += "<h3>Control Panel</h3>";
  html += "<div style='margin: 10px 0;'>";
  html += "<label>Tempo (microseconds per beat): </label>";
  html += "<input type='number' id='tempoInput' value='" + String(userTempo) + "' min='100000' max='1000000' step='1000' style='width: 150px; padding: 5px;'>";
  html += "</div>";
  html += "<div style='margin: 10px 0;'>";
  html += "<button onclick='updateTempo()'>Update Tempo</button>";
  html += "<span id='currentTempo' style='margin-left: 10px;'>Current: " + String(userTempo) + " μs/beat</span>";
  html += "</div>";
  html += "</div>";

  // Add JavaScript for MIDI control
  html += "<script>";
  html += "function playMidi(filename) {";
  html += "  const tempo = document.getElementById('tempoInput').value;";
  html += "  fetch('/play', {";
  html += "    method: 'POST',";
  html += "    headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
  html += "    body: 'file=' + filename + '&tempo=' + tempo";
  html += "  });";
  html += "}";
  html += "function stopMidi() {";
  html += "  fetch('/stop', {";
  html += "    method: 'POST'";
  html += "  });";
  html += "}";
  html += "function updateTempo() {";
  html += "  const tempo = document.getElementById('tempoInput').value;";
  html += "  if (tempo >= 100000 && tempo <= 1000000) {";
  html += "    fetch('/play', {";
  html += "      method: 'POST',";
  html += "      headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
  html += "      body: 'tempo=' + tempo";
  html += "    }).then(() => {";
  html += "      document.getElementById('currentTempo').textContent = 'Current: ' + tempo + ' μs/beat';";
  html += "    });";
  html += "  } else {";
  html += "    alert('Tempo must be between 100,000 and 1,000,000 microseconds per beat');";
  html += "  }";
  html += "}";
  html += "</script>";
  
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void handlePlayMidi() {
  if(server.hasArg("file")) {
    currentFileName = server.arg("file");
    if(!currentFileName.startsWith("/")) {
      currentFileName = "/" + currentFileName;
    }
    // Update tempo if provided
    if(server.hasArg("tempo")) {
      userTempo = server.arg("tempo").toInt();
      tempo = userTempo;
    }
    isPlaying = true;
    server.send(200, "text/plain", "Playing " + currentFileName);
  } else {
    server.send(400, "text/plain", "No file specified");
  }
}

void handleStopMidi() {
  isPlaying = false;
  server.send(200, "text/plain", "Playback stopped");
}

void midiPlayerTask(void * parameter) {
  while(true) {
    if(isPlaying && currentFileName != "") {
      File midiFile = SPIFFS.open(currentFileName, "r");
      if(!midiFile) {
        Serial.println("Failed to open file for reading");
        isPlaying = false;
        continue;
      }

      // Skip MIDI header
      for(int i = 0; i < 14; i++) {
        midiFile.read();
      }

      lastEventTime = micros();
      tempo = userTempo;

      while(midiFile.available() && isPlaying) {
        // Read delta time
        uint32_t deltaTime = readVarLen(&midiFile);

        // Wait for the right time
        if(deltaTime > 0) {
          unsigned long waitTime = (deltaTime * tempo) / ticksPerBeat;
          unsigned long targetTime = lastEventTime + waitTime;
          while(micros() < targetTime && isPlaying) {
            delay(1);
          }
          lastEventTime = targetTime;
        }

        // Read status byte
        uint8_t status = midiFile.read();

        if(status == 0xFF) { // Meta event
          uint8_t type = midiFile.read();
          uint8_t len = midiFile.read();

          if(type == 0x51 && len == 3) { // Tempo change
            uint32_t newTempo = 0;
            for(int i = 0; i < 3; i++) {
              newTempo = (newTempo << 8) | midiFile.read();
            }
            if(tempo == 500000) { // Only change if default tempo
              tempo = newTempo;
              Serial.printf("MIDI Tempo: %d\n", tempo);
            }
          } else {
            for(int i = 0; i < len; i++) {
              midiFile.read();
            }
          }
        }
        else if((status & 0xF0) == 0x90 || (status & 0xF0) == 0x80) { // Note events
          uint8_t note = midiFile.read();
          uint8_t velocity = midiFile.read();

          // Map note to horn (0-5)
          int noteBase = note % 12;
          int hornIndex;

          switch(noteBase) {
            case 0:  // C (Do)
              hornIndex = 0;
              break;
            case 2:  // D (Re)
              hornIndex = 1;
              break;
            case 4:  // E (Mi)
              hornIndex = 2;
              break;
            case 5:  // F (Fa)
              hornIndex = 3;
              break;
            case 7:  // G (Sol)
              hornIndex = 4;
              break;
            case 9:  // A (La)
              hornIndex = 5;
              break;
            default:
              hornIndex = -1;
              break;
          }

          if(hornIndex >= 0 && hornIndex < 6) {
            if((status & 0xF0) == 0x90 && velocity > 0) {
              digitalWrite(hornPins[hornIndex], HIGH);
              Serial.printf("Horn %d ON (Note: %d)\n", hornIndex + 1, note);
            } else {
              digitalWrite(hornPins[hornIndex], LOW);
              Serial.printf("Horn %d OFF (Note: %d)\n", hornIndex + 1, note);
            }
          }
        }
      }

      midiFile.close();
      
      // Reset all horns
      for(int i = 0; i < 6; i++) {
        digitalWrite(hornPins[i], LOW);
      }
      
      isPlaying = false;
      currentFileName = "";
    }
    delay(10);
  }
}

uint32_t readVarLen(File* file) {
  uint32_t value = 0;
  uint8_t byte;
  
  do {
    byte = file->read();
    value = (value << 7) | (byte & 0x7F);
  } while (byte & 0x80);
  
  return value;
}

// File handling functions remain the same as in the original file manager code
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/" + filename;
    Serial.printf("handleFileUpload Name: %s\n", filename.c_str());
    uploadFile = SPIFFS.open(filename, "w");
  } 
  else if(upload.status == UPLOAD_FILE_WRITE && uploadFile){
    uploadFile.write(upload.buf, upload.currentSize);
  } 
  else if(upload.status == UPLOAD_FILE_END){
    uploadFile.close();
    server.sendHeader("Location","/");
    server.send(303);
  }
}

void handleFile() {
  if(server.hasArg("name")) {
    String fileName = server.arg("name");
    if(!fileName.startsWith("/")) fileName = "/" + fileName;
    if(SPIFFS.exists(fileName)) {
      File file = SPIFFS.open(fileName, "r");
      if(file) {
        server.streamFile(file, "application/octet-stream");
        file.close();
        return;
      }
    }
    server.send(404, "text/plain", "File not found");
  }
}

void handleDelete() {
  if(server.hasArg("file")) {
    String fileName = server.arg("file");
    if(!fileName.startsWith("/")) fileName = "/" + fileName;
    if(SPIFFS.remove(fileName)){
      server.sendHeader("Location","/");
      server.send(303);
    } else {
      server.send(500, "text/plain", "Delete failed");
    }
  }
}
