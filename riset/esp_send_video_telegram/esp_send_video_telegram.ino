#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

// WiFi credentials
const char* ssid = "9t";
const char* password = "12345678";

// Telegram configuration
#define BOT_TOKEN "6243468140:AAH1y2lqgf-Oa8LfAwZvgqb_R0671hrvEmY"
#define CHAT_ID "-1002046615083"

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WiFiClientSecure client;

void configInitCamera(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void sendVideoToTelegram() {
  const char* host = "api.telegram.org";
  const int httpsPort = 443;

  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.println("Connecting to " + String(host));

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }

  String head = "--ESP32CAM\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + String(CHAT_ID) + 
                "\r\n--ESP32CAM\r\nContent-Disposition: form-data; name=\"video\"; filename=\"esp32-cam-video.mp4\"\r\nContent-Type: video/mp4\r\n\r\n";
  String tail = "\r\n--ESP32CAM--\r\n";

  uint32_t imageLen = fb->len;
  uint32_t extraLen = head.length() + tail.length();
  uint32_t totalLen = imageLen + extraLen;

  client.println("POST /bot" + String(BOT_TOKEN) + "/sendVideo HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("Content-Length: " + String(totalLen));
  client.println("Content-Type: multipart/form-data; boundary=ESP32CAM");
  client.println();
  client.print(head);

  uint8_t *fbBuf = fb->buf;
  size_t fbLen = fb->len;
  for (size_t n=0; n < fbLen; n=n+1024) {
    if (n+1024 < fbLen) {
      client.write(fbBuf, 1024);
      fbBuf += 1024;
    }
    else if (fbLen%1024>0) {
      size_t remainder = fbLen%1024;
      client.write(fbBuf, remainder);
    }
  }  

  client.print(tail);

  esp_camera_fb_return(fb);

  int timeout = 10000;
  long startTimer = millis();
  boolean state = false;

  while ((startTimer + timeout) > millis()) {
    Serial.print(".");
    delay(100);
    while (client.available()) {
      char c = client.read();
      if (state == true) Serial.print(c);
      if (c == '\n') {
        if (state == false) state = true;
      }
      if (c == '\n' && state == true) {
        state = false;
      }
    }
    if (state == false) break;
  }

  client.stop();
  Serial.println("Video sent");
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());

  configInitCamera();

  client.setInsecure(); // Skip certificate validation
}

void loop() {
  sendVideoToTelegram();
  delay(60000); // Wait for 1 minute before sending the next "video"
}
