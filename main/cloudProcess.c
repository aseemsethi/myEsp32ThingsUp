#include "common.h"
#include "mqtt_client.h"
#include <time.h>
#include <sys/time.h>

extern tcpip_adapter_ip_info_t ip_info;
extern esp_mqtt_client_handle_t client;
extern uint8_t chipid[6];
static const char *TAG = "cloudProcess  ";

/*
 * The GW should send a publish message every X min to gurupada/gw/add
 * with the following body for it to update data about itself in the DB tree
 {
    "gwid"     : "10010",
    "type"     : "esp32",
    "ip"       : "1.1.1.1"
 }
*/
void cloudProcess(void* param) {
    char temp1[100];

    // Sleep for 120 sec in a for ever loop.
    while(1) {
        TickType_t xDelay = 120 * 1000 / portTICK_PERIOD_MS;
        vTaskDelay(xDelay);
        sprintf(temp1, "{\"gwid\":\"%2x%2x%2x\", \"type\":\"esp32\", \"ip\":\"%s\"}", 
        chipid[0], chipid[1], chipid[2], ip4addr_ntoa(&ip_info.ip));

        int msg_id = esp_mqtt_client_publish(client, "gurupada/gw/add", temp1, 0, 0, 0);
        ESP_LOGI(TAG, "\ncloudProcess: %s on topic gurupada/gw/add , with id: %d !!!", 
                        temp1, msg_id);
    }
}