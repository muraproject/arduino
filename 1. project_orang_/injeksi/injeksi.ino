#include <driver/ledc.h>

const int inputPin = 34;  // Pin input untuk sinyal frekuensi
const int outputPin = 25; // Pin output untuk sinyal PWM (bisa diganti sesuai kebutuhan)
volatile unsigned long pulseCount = 0;
unsigned long lastTime = 0;
unsigned long updateInterval = 200;  // Interval update dalam milidetik

// Konfigurasi PWM
const int pwmChannel = 0;
const ledc_timer_bit_t pwmResolution = LEDC_TIMER_8_BIT;
const int maxPwmValue = 255; // 2^8 - 1
const int dutyCycle = 90; // 90% duty cycle

ledc_channel_config_t ledc_channel;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), pulseCounter, RISING);

  // Konfigurasi PWM
  ledc_timer_config_t ledc_timer;
  ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_timer.duty_resolution = pwmResolution;
  ledc_timer.timer_num = LEDC_TIMER_0;
  ledc_timer.freq_hz = 1000; // Frekuensi awal 1000 Hz
  ledc_timer_config(&ledc_timer);

  ledc_channel.gpio_num = outputPin;
  ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_channel.channel = LEDC_CHANNEL_0;
  ledc_channel.timer_sel = LEDC_TIMER_0;
  ledc_channel.duty = 0;
  ledc_channel.hpoint = 0;
  ledc_channel_config(&ledc_channel);
}

void loop() {
  if (millis() - lastTime >= updateInterval) {
    detachInterrupt(digitalPinToInterrupt(inputPin));
    
    // Hitung frekuensi
    float frequency = (float)pulseCount * (1000.0 / updateInterval);
    
    // Tambahkan 5% ke frekuensi
    float adjustedFrequency = frequency * 1.05;
    
    // Update PWM dengan frekuensi baru
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, (uint32_t)adjustedFrequency);
    
    // Set duty cycle ke 90%
    uint32_t duty = (dutyCycle * maxPwmValue) / 100;
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    
    // Cetak frekuensi asli dan yang disesuaikan
    Serial.print("Frekuensi asli: ");
    Serial.print(frequency);
    Serial.print(" Hz, Frekuensi output: ");
    Serial.print(adjustedFrequency);
    Serial.println(" Hz");
    
    // Reset counter dan waktu
    pulseCount = 0;
    lastTime = millis();
    
    // Aktifkan kembali interrupt
    attachInterrupt(digitalPinToInterrupt(inputPin), pulseCounter, RISING);
  }
}
