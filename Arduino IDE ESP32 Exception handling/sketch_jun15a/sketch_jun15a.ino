// Pastikan menggunakan library terbaru
// Gunakan ini jika library ESP32 V3.2.0+++
// Adafruit_ADS1X15 library versi terbaru
// ESP32 board package versi terbaru
// By @IlhamGods

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup(void) {
  Serial.begin(9600);
  
  // Set I2C frequency lebih rendah untuk stabilitas
  Wire.setClock(100000); // 100kHz instead of default 400kHz
  
  // Inisialisasi dengan timeout
  if (!ads.begin()) {
    Serial.println("ADS1115 initialization failed!");
    while(1); // Stop execution
  }
  
  Serial.println("ADS1115 ready!");
}

void loop(void) {
  // Reading dengan exception handling
  try {
    int16_t adc0 = ads.readADC_SingleEnded(0);
    float voltage = ads.computeVolts(adc0); // Gunakan fungsi bawaan library
    
    Serial.print("ADC0: "); Serial.print(adc0);
    Serial.print("\tVoltage: "); Serial.print(voltage, 4); Serial.println(" V");
  } catch (...) {
    Serial.println("Error reading ADC");
  }
  
  delay(1000);
}