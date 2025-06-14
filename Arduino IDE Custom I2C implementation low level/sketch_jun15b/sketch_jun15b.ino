// Custom I2C implementation low level
// by @IlhamGods

#include <Wire.h>

#define ADS1115_ADDRESS 0x48
#define ADS1115_CONVERSION_REG 0x00
#define ADS1115_CONFIG_REG 0x01

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000); // 100kHz
  
  Serial.println("Custom ADS1115 Implementation");
}

void loop() {
  // Start conversion
  if (startConversion()) {
    delay(10); // Wait for conversion
    
    int16_t result = readConversion();
    if (result != -1) {
      float voltage = result * 0.1875 / 1000.0; // mV to V
      
      Serial.print("Raw ADC: ");
      Serial.print(result);
      Serial.print(" | Voltage: ");
      Serial.print(voltage, 4);
      Serial.println(" V");
    } else {
      Serial.println("Read failed");
    }
  } else {
    Serial.println("Start conversion failed");
  }
  
  delay(1000);
}

bool startConversion() {
  Wire.beginTransmission(ADS1115_ADDRESS);
  Wire.write(ADS1115_CONFIG_REG);
  // Config: Single-shot, AIN0, ±6.144V, 128SPS
  Wire.write(0xC1); // MSB
  Wire.write(0x83); // LSB
  
  return (Wire.endTransmission() == 0);
}

int16_t readConversion() {
  Wire.beginTransmission(ADS1115_ADDRESS);
  Wire.write(ADS1115_CONVERSION_REG);
  
  if (Wire.endTransmission() != 0) {
    return -1;
  }
  
  Wire.requestFrom(ADS1115_ADDRESS, 2);
  
  if (Wire.available() == 2) {
    int16_t result = Wire.read() << 8;
    result |= Wire.read();
    return result;
  }
  
  return -1;
}