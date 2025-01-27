#include <U8g2lib.h>

// Konstruktor untuk OLED 1.3" 128x64 SSD1306
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);

  // Inisialisasi OLED
  u8g2.begin();
  u8g2.enableUTF8Print();

  Serial.println("OLED 1.3 inch Test Starting...");
}

void loop() {
  static int counter = 0;
  counter++;

  u8g2.clearBuffer();

  // Header
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Font yang lebih besar untuk header
  u8g2.drawStr(0, 10, "OLED 1.3\" Test");

  // Garis pemisah
  u8g2.drawHLine(0, 13, 128);

  // Informasi Display
  u8g2.setFont(u8g2_font_6x10_tf);  // Font yang lebih kecil untuk detail
  u8g2.setCursor(0, 25);
  u8g2.print("Resolution: 128x64");
  u8g2.setCursor(0, 35);
  u8g2.print("Driver: SSD1306");
  u8g2.setCursor(0, 45);
  u8g2.print("Size: 1.3 inch");

  // Counter di bagian bawah
  u8g2.setCursor(0, 60);
  u8g2.print("Counter: ");
  u8g2.print(counter);

  u8g2.sendBuffer();

  delay(1000);
}

void loop() {
  server.handleClient();

  // Button handling with debounce
  static bool prevState = true;
  static bool playState = true;
  static bool stopState = true;
  static bool nextState = true;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  bool currentPrev = digitalRead(BUTTON_PREV);
  bool currentPlay = digitalRead(BUTTON_PLAY);
  bool currentStop = digitalRead(BUTTON_STOP);
  bool currentNext = digitalRead(BUTTON_NEXT);

  unsigned long currentMillis = millis();

  if (currentMillis - lastDebounceTime > debounceDelay) {
    if (prevState && !currentPrev) {
      prevFile();
      lastDebounceTime = currentMillis;
    }
    if (playState && !currentPlay && !isPlaying && totalFiles > 0) {
      currentFileName = "/" + fileList[currentFileIndex];
      isPlaying = true;
      updateOLEDDisplay();
      lastDebounceTime = currentMillis;
    }
    if (stopState && !currentStop) {
      isPlaying = false;
      currentFileName = "";
      for (int i = 0; i < 6; i++) {
        noteStates[i] = false;
        digitalWrite(hornPins[i], LOW);
      }
      updateOLEDDisplay();
      lastDebounceTime = currentMillis;
    }
    if (nextState && !currentNext) {
      nextFile();
      lastDebounceTime = currentMillis;
    }
  }

  prevState = currentPrev;
  playState = currentPlay;
  stopState = currentStop;
  nextState = currentNext;

  delay(2);
}

// MIDI Variable Length Value Reader
uint32_t readVarLen(File* file) {
  uint32_t value = 0;
  uint8_t byte;

  do {
    byte = file->read();
    value = (value << 7) | (byte & 0x7F);
  } while (byte & 0x80);

  return value;
}

// Web Handlers
void handlePlayMidi() {
  // Handle manual note control
  if (server.hasArg("note") && server.hasArg("action")) {
    int noteIndex = server.arg("note").toInt();
    String action = server.arg("action");

    if (noteIndex >= 0 && noteIndex < 6) {
      if (action == "on") {
        digitalWrite(hornPins[noteIndex], HIGH);
        noteStates[noteIndex] = true;
      } else if (action == "off") {
        digitalWrite(hornPins[noteIndex], LOW);
        noteStates[noteIndex] = false;
      }
      updateOLEDDisplay();
    }
    server.send(200, "text/plain", "OK");
    return;
  }

  // Handle MIDI file playback
  if (server.hasArg("file")) {
    currentFileName = server.arg("file");
    if (!currentFileName.startsWith("/")) {
      currentFileName = "/" + currentFileName;
    }
    if (server.hasArg("tempo")) {
      userTempo = server.arg("tempo").toInt();
      tempo = userTempo;
    }
    isPlaying = true;
    updateOLEDDisplay();
    server.send(200, "text/plain", "Playing " + currentFileName);
  } else if (server.hasArg("tempo")) {
    userTempo = server.arg("tempo").toInt();
    tempo = userTempo;
    server.send(200, "text/plain", "Tempo updated");
  } else {
    server.send(400, "text/plain", "No file specified");
  }
}

void handleStopMidi() {
  isPlaying = false;
  currentFileName = "";
  for (int i = 0; i < 6; i++) {
    noteStates[i] = false;
    digitalWrite(hornPins[i], LOW);
  }
  updateOLEDDisplay();
  server.send(200, "text/plain", "Playback stopped");
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.printf("handleFileUpload Name: %s\n", filename.c_str());
    uploadFile = SPIFFS.open(filename, "w");
  }
  else if (upload.status == UPLOAD_FILE_WRITE && uploadFile) {
    uploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END) {
    uploadFile.close();
    updateFileList();
    updateOLEDDisplay();
    server.sendHeader("Location", "/");
    server.send(303);
  }
}

void handleFile() {
  if (server.hasArg("name")) {
    String fileName = server.arg("name");
    if (!fileName.startsWith("/")) {
      fileName = "/" + fileName;
    }
    if (SPIFFS.exists(fileName)) {
      File file = SPIFFS.open(fileName, "r");
      if (file) {
        server.streamFile(file, "application/octet-stream");
        file.close();
        return;
      }
    }
    server.send(404, "text/plain", "File not found");
  }
}

void handleDelete() {
  if (server.hasArg("file")) {
    String fileName = server.arg("file");
    if (!fileName.startsWith("/")) {
      fileName = "/" + fileName;
    }
    if (SPIFFS.remove(fileName)) {
      updateFileList();
      updateOLEDDisplay();
      server.sendHeader("Location", "/");
      server.send(303);
    } else {
      server.send(500, "text/plain", "Delete failed");
    }
  }

  // MIDI Player Task
  void midiPlayerTask(void * parameter) {
    while (true) {
      if (isPlaying && currentFileName != "") {
        File midiFile = SPIFFS.open(currentFileName, "r");
        if (!midiFile) {
          Serial.println("Failed to open file for reading");
          isPlaying = false;
          updateOLEDDisplay();
          continue;
        }

        // Skip MIDI header
        for (int i = 0; i < 14; i++) {
          midiFile.read();
        }

        lastEventTime = micros();
        tempo = userTempo;

        while (midiFile.available() && isPlaying) {
          // Read delta time
          uint32_t deltaTime = readVarLen(&midiFile);

          // Wait for the right time
          if (deltaTime > 0) {
            unsigned long waitTime = (deltaTime * tempo) / ticksPerBeat;
            unsigned long targetTime = lastEventTime + waitTime;
            while (micros() < targetTime && isPlaying) {
              delay(1);
            }
            lastEventTime = targetTime;
          }

          // Read status byte
          uint8_t status = midiFile.read();

          if (status == 0xFF) { // Meta event
            uint8_t type = midiFile.read();
            uint8_t len = midiFile.read();

            if (type == 0x51 && len == 3) { // Tempo change
              uint32_t newTempo = 0;
              for (int i = 0; i < 3; i++) {
                newTempo = (newTempo << 8) | midiFile.read();
              }
              if (tempo == 500000) { // Only change if default tempo
                tempo = newTempo;
                Serial.printf("MIDI Tempo: %d\n", tempo);
              }
            } else {
              for (int i = 0; i < len; i++) {
                midiFile.read();
              }
            }
          }
          else if ((status & 0xF0) == 0x90 || (status & 0xF0) == 0x80) { // Note events
            uint8_t note = midiFile.read();
            uint8_t velocity = midiFile.read();

            // Map note to horn (0-5)
            int noteBase = note % 12;
            int hornIndex;

            switch (noteBase) {
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

            if (hornIndex >= 0 && hornIndex < 6) {
              if ((status & 0xF0) == 0x90 && velocity > 0) {
                digitalWrite(hornPins[hornIndex], HIGH);
                noteStates[hornIndex] = true;
              } else {
                digitalWrite(hornPins[hornIndex], LOW);
                noteStates[hornIndex] = false;
              }
              updateOLEDDisplay();
            }
          }
        }

        midiFile.close();

        // Reset all horns
        for (int i = 0; i < 6; i++) {
          digitalWrite(hornPins[i], LOW);
          noteStates[i] = false;
        }

        isPlaying = false;
        currentFileName = "";
        updateOLEDDisplay();
      }
      delay(10);
    }
  }

  void handleRoot() {
    String html = "<!DOCTYPE html><html>";
    html += "<head><title>ESP32 Piano & MIDI Player</title>";
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

    // Piano styles
    html += ".piano-container{width:100%;max-width:600px;margin:0 auto;overflow-x:auto}";
    html += ".piano{position:relative;height:200px;background:#1a1a1a;padding:20px;border-radius:10px;box-shadow:0 5px 15px rgba(0,0,0,0.3)}";
    html += ".white-keys{display:flex;justify-content:space-between;height:100%;gap:4px}";
    html += ".white-key{flex:1;background:white;border:1px solid #ccc;border-radius:0 0 5px 5px;cursor:pointer;position:relative;transition:background 0.1s}";
    html += ".white-key:active,.white-key.active{background:#e6e6e6;transform:translateY(2px)}";
    html += ".white-key span{position:absolute;bottom:10px;left:50%;transform:translateX(-50%);font-size:14px;color:#666}";

    html += "@media (max-width: 600px) {";
    html += "  .piano{height:150px}";
    html += "  .white-key span{font-size:12px}";
    html += "}";

    html += "</style></head><body><div class='container'>";

    html += "<h2>ESP32 Piano & MIDI Player</h2>";

    // Piano interface
    html += "<div class='control-panel' style='margin: 20px 0; padding: 20px; background: #f9f9f9; border-radius: 5px;'>";
    html += "<h3>Piano Control</h3>";
    html += "<div class='piano-container'>";
    html += "<div class='piano'>";
    html += "<div class='white-keys'>";
    const char* noteNames[] = {"Do", "Re", "Mi", "Fa", "Sol", "La"};
    for (int i = 0; i < 6; i++) {
      html += "<button class='white-key' onmousedown='playNote(" + String(i) + ")' onmouseup='stopNote(" + String(i) + ")' "
              "ontouchstart='playNote(" + String(i) + ")' ontouchend='stopNote(" + String(i) + ")'>"
              "<span>" + String(noteNames[i]) + "</span></button>";
    }
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "<p style='text-align:center;margin-top:10px'>Keyboard: A=Do, S=Re, D=Mi, F=Fa, G=Sol, H=La</p>";
    html += "</div>";

    // Upload form
    html += "<div class='upload-form'>";
    html += "<h3>MIDI File Upload</h3>";
    html += "<form action='/upload' method='post' enctype='multipart/form-data'>";
    html += "<input type='file' name='file' accept='.mid,.midi' required>";
    html += "<button type='submit'>Upload File</button>";
    html += "</form></div>";

    // File table
    html += "<h3>MIDI Files</h3>";
    html += "<table><tr><th>Filename</th><th>Size</th><th>Actions</th></tr>";

    for (int i = 0; i < totalFiles; i++) {
      String fileName = fileList[i];
      File file = SPIFFS.open("/" + fileName, "r");
      if (file) {
        html += "<tr>";
        html += "<td>" + fileName + "</td>";
        html += "<td>" + String(file.size()) + " bytes</td>";
        html += "<td>";
        html += "<button class='play' onclick='playMidi(\"" + fileName + "\")'>Play</button>";
        html += "<button class='stop' onclick='stopMidi()'>Stop</button>";
        html += "<button onclick='window.location.href=\"/file?name=" + fileName + "\"'>Download</button>";
        html += "<form action='/delete' method='post' style='display:inline'>";
        html += "<input type='hidden' name='file' value='" + fileName + "'>";
        html += "<button type='submit' class='delete'>Delete</button>";
        html += "</form></td></tr>";
        file.close();
      }
    }
    html += "</table>";

    // Tempo control
    html += "<div class='control-panel' style='margin: 20px 0; padding: 20px; background: #f9f9f9; border-radius: 5px;'>";
    html += "<h3>Tempo Control</h3>";
    html += "<div style='margin: 10px 0;'>";
    html += "<label>Tempo (microseconds per beat): </label>";
    html += "<input type='number' id='tempoInput' value='" + String(userTempo) + "' min='100000' max='1000000' step='1000' style='width: 150px; padding: 5px;'>";
    html += "</div>";
    html += "<div style='margin: 10px 0;'>";
    html += "<button onclick='updateTempo()'>Update Tempo</button>";
    html += "<span id='currentTempo' style='margin-left: 10px;'>Current: " + String(userTempo) + " μs/beat</span>";
    html += "</div>";
    html += "</div>";

    // JavaScript
    html += "<script>";

    // Touch event handling
    html += "document.addEventListener('touchstart', function(e) {";
    html += "  if(e.target.classList.contains('white-key')) {";
    html += "    e.preventDefault();";
    html += "  }";
    html += "}, {passive: false});";

    // Keyboard controls
    html += "document.addEventListener('keydown', function(e) {";
    html += "  if(e.repeat) return;";
    html += "  const key = e.key.toLowerCase();";
    html += "  switch(key) {";
    html += "    case 'a': playNote(0); break;";
    html += "    case 's': playNote(1); break;";
    html += "    case 'd': playNote(2); break;";
    html += "    case 'f': playNote(3); break;";
    html += "    case 'g': playNote(4); break;";
    html += "    case 'h': playNote(5); break;";
    html += "  }";
    html += "});";

    html += "document.addEventListener('keyup', function(e) {";
    html += "  const key = e.key.toLowerCase();";
    html += "  switch(key) {";
    html += "    case 'a': stopNote(0); break;";
    html += "    case 's': stopNote(1); break;";
    html += "    case 'd': stopNote(2); break;";
    html += "    case 'f': stopNote(3); break;";
    html += "    case 'g': stopNote(4); break;";
    html += "    case 'h': stopNote(5); break;";
    html += "  }";
    html += "});";

    // Note control functions
    html += "function playNote(index) {";
    html += "  fetch('/play', {";
    html += "    method: 'POST',";
    html += "    headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
    html += "    body: 'note=' + index + '&action=on'";
    html += "  });";
    html += "}";

    html += "function stopNote(index) {";
    html += "  fetch('/play', {";
    html += "    method: 'POST',";
    html += "    headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
    html += "    body: 'note=' + index + '&action=off'";
    html += "  });";
    html += "}";

    // MIDI control functions
    html += "function playMidi(filename) {";
    html += "  const tempo = document.getElementById('tempoInput').value;";
    html += "  fetch('/play', {";
    html += "    method: 'POST',";
    html += "    headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
    html += "    body: 'file=' + filename + '&tempo=' + tempo";
    html += "  });";
    html += "}";

    html += "function stopMidi() {";
    html += "  fetch('/stop', {method: 'POST'});";
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
