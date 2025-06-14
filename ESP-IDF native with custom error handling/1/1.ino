//ESP-IDF native with custom error handling
// By @IlhamGods


#include "driver/i2c.h"
#include "esp_log.h"

// Disable specific log untuk I2C
esp_log_level_set("i2c.master", ESP_LOG_ERROR); // Atau ESP_LOG_NONE

// Custom I2C read dengan proper error handling
esp_err_t custom_i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    // Start condition
    i2c_master_start(cmd);
    
    // Address + Write bit
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, true);
    
    // Register address
    i2c_master_write_byte(cmd, reg_addr, true);
    
    // Repeated start
    i2c_master_start(cmd);
    
    // Address + Read bit
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, true);
    
    // Read data
    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + len - 1, I2C_MASTER_NACK);
    
    // Stop condition
    i2c_master_stop(cmd);
    
    // Execute and handle errors gracefully
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    
    return ret;
}