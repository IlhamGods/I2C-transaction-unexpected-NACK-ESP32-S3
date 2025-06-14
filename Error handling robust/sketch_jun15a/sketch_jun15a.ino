#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;
bool adsInitialized = false;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Starting ADS1115 Test...");
  
  // Set I2C clock lebih rendah untuk stabilitas
  Wire.setClock(100000); // 100kHz (default 400kHz)
  
  // Coba beberapa alamat yang mungkin
  uint8_t addresses[] = {0x48, 0x49, 0x4A, 0x4B};
  
  for (int i = 0; i < 4; i++) {
    Serial.print("Trying address 0x");
    Serial.println(addresses[i], HEX);
    
    if (ads.begin(addresses[i])) {
      Serial.print("ADS1115 found at address 0x");
      Serial.println(addresses[i], HEX);
      adsInitialized = true;
      break;
    }
    delay(100);
  }
  
  if (!adsInitialized) {
    Serial.println("ADS1115 not found on any address!");
    Serial.println("Check wiring and I2C connections");
    runI2CScanner(); // Jalankan scanner untuk debug
  } else {
    Serial.println("ADS1115 initialized successfully!");
    
    // Set gain jika perlu (opsional)
    ads.setGain(GAIN_TWOTHIRDS); // ±6.144V range (default)
    // bisa juga sesuaikan set gain sesuai kondisional anda
  }
}

void loop(void) {
  if (!adsInitialized) {
    Serial.println("ADS1115 not initialized. Check connections.");
    delay(5000);
    return;
  }
  
  // Implementasi dengan retry mechanism
  bool readSuccess = false;
  int16_t adc0 = 0;
  int maxRetries = 3;
  
  for (int retry = 0; retry < maxRetries && !readSuccess; retry++) {
    if (retry > 0) {
      Serial.print("Retry attempt: ");
      Serial.println(retry);
      delay(50); // Delay sebelum retry
    }
    
    // Coba baca ADC
    adc0 = ads.readADC_SingleEnded(0);
    
    // Check jika pembacaan valid (bukan error value)
    if (adc0 >= 0 || adc0 != -1) {
      readSuccess = true;
    }
  }
  
  if (readSuccess) {
    // Gunakan fungsi computeVolts untuk akurasi lebih baik
    float voltage = ads.computeVolts(adc0);
    
    Serial.print("ADC0: ");
    Serial.print(adc0);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 4);
    Serial.println(" V");
  } else {
    Serial.println("Failed to read ADC after multiple retries");
    
    // Coba reinitialize jika gagal terus
    static int failCount = 0;
    failCount++;
    if (failCount > 10) {
      Serial.println("Too many failures, attempting reinitialize...");
      adsInitialized = false;
      failCount = 0;
      setup(); // Re-run setup
      return;
    }
  }
  
  delay(1000);
}

// Fungsi I2C Scanner untuk debugging
void runI2CScanner() {
  Serial.println("Running I2C Scanner...");
  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.print("Found ");
    Serial.print(nDevices);
    Serial.println(" device(s)");
  }
  Serial.println();
}