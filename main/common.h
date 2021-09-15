#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"  // FOR EventGroupHandle_t
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
//#include "esp_event_loop.h"

#include "esp_event.h"
#include <nvs_flash.h>
#include "driver/gpio.h"
#include "esp_log.h"  // for ESP_LOGE
#include "esp_event.h"
#include "string.h"
#include "sdkconfig.h"
#include <errno.h>
#include <esp_http_server.h>
#include "esp_smartconfig.h"

#include "esp_spi_flash.h"
#include "esp_log.h"  // for ESP_LOGE

void task_test_SSD1306i2c(void *ignore);
void oledDisplay(int x, int y, char* str);
void oledClear(void);
void smartconfig_run_task(void*);
void smartconfig_event_handler(void* arg, esp_event_base_t event_base, 
                                int32_t event_id, void* event_data);
void wifi_eraseconfig(void);
void printDiags(void);
void ble_main(void);
void wifi_mqtt_start(void*);
void wifi_send_mqtt(char*);
int hextodc(char *hex);