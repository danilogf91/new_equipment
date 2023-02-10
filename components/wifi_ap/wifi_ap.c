#include "wifi_ap.h"

static const char* TAG = "wifi AP";

void wifi_event_handler (void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    // if ( event_id == WIFI_EVENT_AP_STACONNECTED )
    // {
    //     wifi_event_ap_staconnected_t* event = ( wifi_event_ap_staconnected_t* ) event_data;
    //     ESP_LOGI (TAG, "station "MACSTR" join, AID=%d",
    //         MAC2STR (event->mac), event->aid);
    // }
    // else if ( event_id == WIFI_EVENT_AP_STADISCONNECTED )
    // {
    //     wifi_event_ap_stadisconnected_t* event = ( wifi_event_ap_stadisconnected_t* ) event_data;
    //     ESP_LOGI (TAG, "station "MACSTR" leave, AID=%d",
    //         MAC2STR (event->mac), event->aid);
    // }

    wifi_event_ap_staconnected_t* event_connect = ( wifi_event_ap_staconnected_t* ) event_data;
    wifi_event_ap_stadisconnected_t* event_disconnect = ( wifi_event_ap_stadisconnected_t* ) event_data;
    switch ( event_id )
    {
        case WIFI_EVENT_AP_START:
            ESP_LOGI (TAG, "AP start... ");
            break;
        case WIFI_EVENT_AP_STOP:
            ESP_LOGI (TAG, "AP stop... ");
            break;

        case WIFI_EVENT_AP_STACONNECTED:
            ESP_LOGI (TAG, "station "MACSTR" join, AID=%d", MAC2STR (event_connect->mac), event_connect->aid);
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            ESP_LOGI (TAG, "station "MACSTR" leave, AID=%d", MAC2STR (event_disconnect->mac), event_disconnect->aid);
            break;
        case WIFI_EVENT_AP_PROBEREQRECVED:

            break;
    }

}

esp_err_t wifi_init_softap (wifi_ap_credentials_t wifi)
{
    ESP_ERROR_CHECK (esp_netif_init ());
    ESP_ERROR_CHECK (esp_event_loop_create_default ());
    esp_netif_create_default_wifi_ap ();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT ();
    ESP_ERROR_CHECK (esp_wifi_init (&cfg));

    ESP_ERROR_CHECK (esp_event_handler_instance_register (WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler,
        NULL,
        NULL));

    wifi_config_t wifi_config;
    strcpy (( char* ) wifi_config.ap.ssid, ( const char* ) wifi.ssid);
    strcpy (( char* ) wifi_config.ap.password, ( const char* ) wifi.pass);

    // wifi_config.ap.ssid = EXAMPLE_ESP_AP_WIFI_SSID;
    wifi_config.ap.ssid_len = strlen (( const char* ) wifi.ssid);
    wifi_config.ap.channel = EXAMPLE_ESP_WIFI_CHANNEL;
    // wifi_config.ap.password = EXAMPLE_ESP_AP_WIFI_PASS;
    wifi_config.ap.max_connection = EXAMPLE_MAX_STA_CONN;
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    if ( strlen (EXAMPLE_ESP_AP_WIFI_PASS) == 0 )
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK (esp_wifi_set_mode (WIFI_MODE_AP));
    ESP_ERROR_CHECK (esp_wifi_set_config (WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK (esp_wifi_start ());

    ESP_LOGI (TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d ssid_len:%d",
        wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel, wifi_config.ap.ssid_len);
    return ESP_OK;
}
