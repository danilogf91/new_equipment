#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define EXAMPLE_ESP_AP_WIFI_SSID        "esp32_wifi"
#define EXAMPLE_ESP_AP_WIFI_PASS        "password1234"
#define EXAMPLE_ESP_WIFI_CHANNEL        1
#define EXAMPLE_MAX_STA_CONN            4

typedef struct wifi_ap_credentials
{
    unsigned char ssid[32];
    unsigned char pass[64];
}wifi_ap_credentials_t;

void wifi_event_handler (void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void wifi_init_softap (wifi_ap_credentials_t wifi);

