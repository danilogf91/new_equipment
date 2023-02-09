#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#define BUFFSIZE 32

typedef struct NVS_Data
{
    uint8_t s_led_state;
    char ssid[BUFFSIZE];
    char pass[BUFFSIZE];
}NVS_Data_t;


void write_nvs_test (char* ssid, char* password);
esp_err_t read_nvs_test (nvs_handle* my_handle, NVS_Data_t* data_retrieve);
esp_err_t init_nvs_test (void);

