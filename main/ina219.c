// Example from
// https://github.com/UncleRus/esp-idf-lib/blob/master/examples/ina219/main/main.c

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <ina219.h>
#include <string.h>
#include <esp_log.h>

#define I2C_PORT 0
#define I2C_ADDR INA219_ADDR_GND_GND
#if defined(CONFIG_IDF_TARGET_ESP8266)
#define SDA_GPIO 4
#define SCL_GPIO 5
#else
#define SDA_GPIO 16
#define SCL_GPIO 17
#endif

const static char *TAG = "INA219";

void i2cTask(void *pvParameters)
{
    ina219_t dev;
    memset(&dev, 0, sizeof(ina219_t));

    // SDA, SCL - 5,4 - SDA_GPIO, SCL_GPIO, 21, 22
    ESP_ERROR_CHECK(ina219_init_desc(&dev, I2C_ADDR, I2C_PORT, 21, 22));
    ESP_LOGI(TAG, "Initializing INA219");
    //ESP_ERROR_CHECK(ina219_init(&dev));
    if (ina219_init(&dev) != ESP_OK) {
        ESP_LOGI(TAG, "init INA219...failed !!!!!!!!1");
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }

    ESP_LOGI(TAG, "Configuring INA219");
    ESP_ERROR_CHECK(ina219_configure(&dev, INA219_BUS_RANGE_16V, INA219_GAIN_0_125,
            INA219_RES_12BIT_1S, INA219_RES_12BIT_1S, INA219_MODE_CONT_SHUNT_BUS));

    ESP_LOGI(TAG, "Calibrating INA219");
    ESP_ERROR_CHECK(ina219_calibrate(&dev, 5.0, 0.1)); // 5A max current, 0.1 Ohm shunt resistance

    float bus_voltage, shunt_voltage, current, power;

    ESP_LOGI(TAG, "Starting the loop");
    while (1)
    {
        ESP_ERROR_CHECK(ina219_get_bus_voltage(&dev, &bus_voltage));
        ESP_ERROR_CHECK(ina219_get_shunt_voltage(&dev, &shunt_voltage));
        ESP_ERROR_CHECK(ina219_get_current(&dev, &current));
        ESP_ERROR_CHECK(ina219_get_power(&dev, &power));
        /* Using float in printf() requires non-default configuration in
         * sdkconfig. See sdkconfig.defaults.esp32 and
         * sdkconfig.defaults.esp8266  */
        printf("VBUS: %.04f V, VSHUNT: %.04f mV, IBUS: %.04f mA, PBUS: %.04f mW\n",
                bus_voltage, shunt_voltage * 1000, current * 1000, power * 1000);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void i2c_start()
{
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreate(i2cTask, "test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
}
