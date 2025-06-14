//Disable ACK check
// by @IlhamGods

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup(void) {
  Serial.begin(9600);
  
  // Workaround: Coba inisialisasi dengan error handling
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115!");
    // Coba dengan alamat berbeda
    if (!ads.begin(0x49)) {
      Serial.println("ADS1115 not found on any address!");
      return;
    }
  }
  Serial.println("ADS1115 initialized successfully!");
}

void loop(void) {
  // Implementasi error handling yang lebih robust
  bool success = false;
  int16_t adc0 = 0;
  
  // Retry mechanism
  for (int retry = 0; retry < 3; retry++) {
    adc0 = ads.readADC_SingleEnded(0);
    if (adc0 != -1) { // Assuming -1 indicates error
      success = true;
      break;
    }
    delay(10); // Short delay before retry
  }
  
  if (success) {
    float voltage = adc0 * 0.1875 / 1000;
    Serial.print("ADC0: "); Serial.print(adc0);
    Serial.print("\tVoltage: "); Serial.print(voltage, 4); Serial.println(" V");
  } else {
    Serial.println("Failed to read ADC after retries");
  }
  
  delay(1000);
}