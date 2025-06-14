# I2C-transaction-unexpected-NACK-ESP32-S3
Solusi alternatif untuk problem I2C transaction unexpected NACK pada board ESP32-S3 dev module

Bagian ini merupakan solusi alternatif untuk problem I2C transaction unexpected NACK pada board ESP32-S3 dev module. Soalnya saya ngalamin ini juga hehe.
Alasan terjadinya NACK sudah dijelaskan pada: https://esp32.com/viewtopic.php?t=45303
Secara singkat, yang terjadi adalah : 
 1. Address ACK berhasil - I2C devices merespon alamat I2C dengan ACK
 2. Data NACK terjadi - Ketika ESP32-S3 mencoba membaca atau menulis register tertentu, perangkat I2C mengirim NACK
 3. Log Spam - ESP IDF menghasilkan error log berulang
 4. Transaksi gagal

Hal ini adalah bukan masalah hardware, seperti permasalahan wiring, tetapi bug pada ESP-IDF I2C driver (V.3.2.0). Driver tersebut mungkin terlalu agresif dalam error. Bug report : https://github.com/espressif/esp-idf/issues/14715

Solusi 
# Solusi pada ESP-IDF
1. Solusi sementara - Downgrade ESP-IDF dari 3.2.0 ke 3.1.3
   git checkout release/v3.1
2. Disable ACK check dengan Workaround dan custom error handling pada ESP-IDF
3. Konfigurasi Menuconfig ESP-IDF
   idf.py menuconfig
   Navigasi ke:
   Component config → Driver configurations → I2C configuration
   Aktifkan "Enable I2C debug log"
   Atur "I2C timeout" ke nilai yang sesuai

# Solusi  pada Arduino IDE
 1. Update Board package ESP32 by Espressif
    File → Preferences → Additional Board Manager URLs:
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    Tools → Board → Boards Manager → Search "ESP32" → Update ke versi terbaru (3.2.0++)
 2. Error handling yang robust
 3. Custom I2C implementation low-level

