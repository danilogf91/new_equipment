#include <string.h>
#include "wifi_connection.h"


static EventGroupHandle_t s_wifi_event_group;
static const char* TAG = "wifi station";
static int s_retry_num = 0;
TaskHandle_t xHandle = NULL;
static esp_netif_t* esp_netif;

wifi_sta_config_t wifi_creed = {
    .ssid = "hPalmiraCM",
    .password = "Hpalmira17"
};

void event_handler (void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if ( event_base == WIFI_EVENT )
    {
        ESP_LOGW (TAG, "WIFI_EVENT");
    }

    if ( event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START )
    {
        wifi_connect ();
    }
    else if ( event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED )
    {
        if ( s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY )
        {
            wifi_connect ();
            s_retry_num++;
            ESP_LOGW (TAG, "retry to connect to the AP");

            // Use the handle to delete the task.
            if ( xHandle != NULL )
            {
                ESP_LOGI (TAG, "ERASE TASK");
                vTaskDelete (xHandle);
                gpio_set_level (WIFI_STATUS_LED, OFF);
            }
        }
        else
        {
            xEventGroupSetBits (s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI (TAG, "connect to the AP fail");
    }
    else if ( event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP )
    {
        ip_event_got_ip_t* event = ( ip_event_got_ip_t* ) event_data;
        ESP_LOGI (TAG, "got ip:" IPSTR, IP2STR (&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits (s_wifi_event_group, WIFI_CONNECTED_BIT);
        xTaskCreate (wifi_status_led, "Wifi status", 1024 * 2, ( void* ) 250, 2, &xHandle);
    }
}

void wifi_init_sta (wifi_credentials_t wifi)
{
    s_wifi_event_group = xEventGroupCreate ();

    ESP_ERROR_CHECK (esp_netif_init ());

    ESP_ERROR_CHECK (esp_event_loop_create_default ());
    //esp_netif_create_default_wifi_sta ();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT ();
    ESP_ERROR_CHECK (esp_wifi_init (&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK (esp_event_handler_instance_register (WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK (esp_event_handler_instance_register (IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));


    esp_netif = esp_netif_create_default_wifi_sta ();

    // for static ip...
    esp_netif_dhcpc_stop (esp_netif);
    esp_netif_ip_info_t ip_info;
    ip_info.ip.addr = ESP_IP4TOADDR (192, 168, 1, 105);
    ip_info.gw.addr = ESP_IP4TOADDR (192, 168, 1, 100);
    ip_info.netmask.addr = ESP_IP4TOADDR (255, 255, 255, 0);
    esp_netif_set_ip_info (esp_netif, &ip_info);

    wifi_config_t wifi_config = {
        .sta = {
            // .ssid = ssid,
            // .password = " wifi.pass",

            // Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
            // If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
            // to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
            // WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.

               .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
               .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
           },
    };
    strcpy (( char* ) wifi_config.sta.ssid, ( const char* ) wifi.ssid);
    strcpy (( char* ) wifi_config.sta.password, ( const char* ) wifi.pass);

    ESP_ERROR_CHECK (esp_wifi_set_mode (WIFI_MODE_STA));
    ESP_ERROR_CHECK (esp_wifi_set_config (WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK (esp_wifi_start ());

    ESP_LOGI (TAG, "wifi_init_sta finished.");

    // Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
    // number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) 
    EventBits_t bits = xEventGroupWaitBits (s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    //xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually happened. 
    if ( bits & WIFI_CONNECTED_BIT )
    {
        ESP_LOGI (TAG, "connected to ap SSID:%s password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else if ( bits & WIFI_FAIL_BIT )
    {
        ESP_LOGI (TAG, "Failed to connect to SSID:%s, password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else
    {
        ESP_LOGE (TAG, "UNEXPECTED EVENT");
    }
}

void wifi_connect (void)
{
    esp_wifi_connect ();
}

void wifi_disconnect (void)
{
    esp_wifi_disconnect ();
    esp_wifi_stop ();
}


void wifi_status_led (void* pvParameters)
{
    gpio_set_direction (WIFI_STATUS_LED, GPIO_MODE_OUTPUT);
    int isOn = OFF;
    while ( true )
    {
        // int random = esp_random ();
        // int positiveNumber = abs (esp_random ());
        int time_reset = ( ( esp_random () ) % 500 ) + 1;
        isOn = !isOn;
        gpio_set_level (WIFI_STATUS_LED, isOn);
        vTaskDelay (time_reset / portTICK_PERIOD_MS);
    }
}

