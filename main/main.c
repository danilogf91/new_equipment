#include "wifi_connection.h"
#include "wifi_ap.h"
#include "driver/gpio.h"

#define PIN_INTERRUPT 15
static const char* TAG = "main program";
SemaphoreHandle_t ota_semaphore = NULL;

void on_button_pushed (void* params)
{
    xSemaphoreGiveFromISR (ota_semaphore, pdFALSE);
}


void run_ota (void* params)
{
    while ( true )
    {
        xSemaphoreTake (ota_semaphore, portMAX_DELAY);
        ESP_LOGI (TAG, "Disconnect network");
        wifi_disconnect ();

    }
}

void app_main (void)
{
    wifi_credentials_t wifi_sta = {
        .ssid = "hPalmiraCM",
        .pass = "Hpalmira17"
    };

    // wifi_ap_credentials_t wifi_ap = {
    //     .ssid = "esp32_network",
    //     .pass = "hpalmira17"
    // };

    //Initialize semaphore
    ota_semaphore = xSemaphoreCreateBinary ();
    //Create task
    xTaskCreate (run_ota, "run_ota", 1024 * 8, NULL, 2, NULL);

    //Initialize INTERRUP SERVICE

    gpio_config_t gpioConfig = {
    .pin_bit_mask = 1ULL << PIN_INTERRUPT,
    .mode = GPIO_MODE_DEF_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLUP_DISABLE,
    .intr_type = GPIO_INTR_POSEDGE };
    gpio_config (&gpioConfig);
    gpio_install_isr_service (0);
    gpio_isr_handler_add (PIN_INTERRUPT, on_button_pushed, ( void* ) PIN_INTERRUPT);

    //Initialize NVS
    esp_err_t ret = nvs_flash_init ();
    if ( ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND )
    {
        ESP_ERROR_CHECK (nvs_flash_erase ());
        ret = nvs_flash_init ();
    }
    ESP_ERROR_CHECK (ret);

    ESP_LOGI (TAG, "ESP_WIFI_MODE_STA");

    //Initialize WIFI STATION
    wifi_init_sta (wifi_sta);
    //wifi_init_softap (wifi_ap);
}



