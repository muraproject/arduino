#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Konfigurasi Access Point
const char* ssid = "ESP32-FileManager";
const char* password = "12345678";

WebServer server(80);
File uploadFile;

// Deklarasi fungsi
void handleRoot();
void handleFileUpload();
void handleFile();
void handleDelete();
void listFiles();

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed. Trying to format...");
    if(SPIFFS.format()){
      Serial.println("SPIFFS formatted successfully");
      if(!SPIFFS.begin()){
        Serial.println("SPIFFS mount after format failed");
        return;
      }
    } else {
      Serial.println("SPIFFS format failed");
      return;
    }
  }

  // Konfigurasi ESP32 sebagai Access Point
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Setup routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.send(200);
  }, handleFileUpload);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/file", HTTP_GET, handleFile);

  server.begin();
  Serial.println("HTTP server started");
  
  // List files setelah mount
  Serial.println("\nInitial files in SPIFFS:");
  listFiles();
}

void loop() {
  server.handleClient();
  delay(2);
}

// Fungsi untuk memeriksa file di SPIFFS
void listFiles() {
  File root = SPIFFS.open("/");
  Serial.println("Files in SPIFFS:");
  
  File file = root.openNextFile();
  while(file) {
    Serial.print("  ");
    Serial.print(file.name());
    Serial.print("  -  ");
    Serial.println(file.size());
    file = root.openNextFile();
  }
  root.close();
  Serial.println("----------");
}

void handleRoot() {
  // Debug: List files in SPIFFS
  Serial.println("\nListing files in SPIFFS:");
  listFiles();
  
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>ESP32 File Manager</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;margin:20px;background:#f0f0f0}";
  html += ".container{max-width:800px;margin:0 auto;background:white;padding:20px;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}";
  html += "table{width:100%;border-collapse:collapse;margin-top:20px}";
  html += "th,td{padding:12px;text-align:left;border-bottom:1px solid #ddd}";
  html += "th{background-color:#4CAF50;color:white}";
  html += "tr:hover{background-color:#f5f5f5}";
  html += ".upload-form{margin:20px 0;padding:20px;background:#f9f9f9;border-radius:5px}";
  html += "button{background-color:#4CAF50;color:white;padding:10px 15px;border:none;border-radius:4px;cursor:pointer}";
  html += "button.delete{background-color:#f44336}";
  html += ".upload-form input[type='file']{margin-right:10px}";
  html += "</style></head><body><div class='container'>";
  
  html += "<h2>ESP32 File Manager</h2>";
  
  // Form upload file
  html += "<div class='upload-form'>";
  html += "<form action='/upload' method='post' enctype='multipart/form-data'>";
  html += "<input type='file' name='file' required>";
  html += "<button type='submit'>Upload File</button>";
  html += "</form></div>";
  
  // Tabel daftar file
  html += "<table><tr><th>Filename</th><th>Size</th><th>Actions</th></tr>";
  
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  
  while(file) {
    String fileName = file.name();
    // Hapus slash awal jika ada
    if(fileName.startsWith("/")) fileName = fileName.substring(1);
    
    html += "<tr>";
    html += "<td>" + fileName + "</td>";
    html += "<td>" + String(file.size()) + " bytes</td>";
    html += "<td>";
    html += "<button onclick='window.location.href=\"/file?name=" + fileName + "\"' style='margin-right:10px'>Download</button>";
    html += "<form action='/delete' method='post' style='display:inline'>";
    html += "<input type='hidden' name='file' value='" + fileName + "'>";
    html += "<button type='submit' class='delete'>Delete</button>";
    html += "</form></td></tr>";
    file = root.openNextFile();
  }
  root.close();
  
  html += "</table></div></body></html>";
  server.send(200, "text/html", html);
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/" + filename;
    Serial.printf("handleFileUpload Name: %s\n", filename.c_str());
    uploadFile = SPIFFS.open(filename, "w");
  } 
  else if(upload.status == UPLOAD_FILE_WRITE && uploadFile){
    if(uploadFile.write(upload.buf, upload.currentSize)){
      Serial.printf("Written: %u\n", upload.currentSize);
    } else {
      Serial.println("Write failed");
    }
  } 
  else if(upload.status == UPLOAD_FILE_END){
    if(uploadFile){
      uploadFile.close();
      Serial.printf("handleFileUpload Size: %u\n", upload.totalSize);
      server.sendHeader("Location","/");
      server.send(303);
    } else {
      server.send(500, "text/plain", "Upload failed");
    }
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
  } else {
    server.send(400, "text/plain", "Bad Request");
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
