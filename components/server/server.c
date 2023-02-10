#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "server.h"
#include "esp_http_server.h"
#include "driver/gpio.h"
#include "cJSON.h"


static const char* TAG = "server";

esp_err_t on_url_hit (httpd_req_t* req)
{
    ESP_LOGI (TAG, "url %s was hit", req->uri);
    char* message = "hello world!";
    httpd_resp_send (req, message, strlen (message));
    return ESP_OK;
}

esp_err_t on_get_temperature (httpd_req_t* req)
{
    char* string = NULL;
    cJSON* temperature_json = NULL;
    cJSON* humidity_json = NULL;

    ESP_LOGI (TAG, "url %s was hit", req->uri);
    // char* message = "{\"temperature\":22}";
    //sprintf (message, "{\"temperature\":%d}", 5);
    cJSON* data = cJSON_CreateObject ();
    if ( data == NULL )
    {
        ESP_LOGW (TAG, "error data");
    }

    if ( !( temperature == -1 && humidity == -1 ) )
    {
        temperature_json = cJSON_CreateNumber (temperature / 10);
        if ( temperature_json == NULL )
        {
            ESP_LOGW (TAG, "error temperature_json");
        }

        humidity_json = cJSON_CreateNumber (humidity / 10);
        if ( humidity_json == NULL )
        {
            ESP_LOGW (TAG, "error humidity_json");
        }

        cJSON_AddItemToObject (data, "Humidity", humidity_json);
        cJSON_AddItemToObject (data, "Temperature", temperature_json);
    }
    else
    {
        humidity_json = cJSON_CreateString ("Error sensor desconectado");
        cJSON_AddItemToObject (data, "Error", humidity_json);
    }


    string = cJSON_Print (data);
    // ESP_LOGW (TAG, "%s", string);
    cJSON_Delete (data);
    if ( string == NULL )
    {
        ESP_LOGE (TAG, "error impresions");
    }

    // ESP_LOGI (TAG, "%s", message);
    // ESP_LOGW (TAG, "Humidity: %d%% Temperature: %dCº", humidity / 10, temperature / 10);
    httpd_resp_set_type (req, "application/json");
    httpd_resp_send (req, string, strlen (string));
    // httpd_resp_send (req, message, strlen (message));
    return ESP_OK;
}

esp_err_t on_led_set (httpd_req_t* req)
{
    ESP_LOGI (TAG, "url %s was hit", req->uri);
    char buf[150];
    memset (&buf, 0, sizeof (buf));
    httpd_req_recv (req, buf, req->content_len);
    ESP_LOGI (TAG, "%s", buf);
    cJSON* payload = cJSON_Parse (buf);
    cJSON* isLedOn = cJSON_GetObjectItem (payload, "isLedOn");
    gpio_set_level (WIFI_SERVER_LED, cJSON_IsTrue (isLedOn));
    cJSON_Delete (payload);

    httpd_resp_set_status (req, "204 NO CONTENT");
    httpd_resp_send (req, NULL, 0);
    return ESP_OK;
}

void RegisterEndPoints (void)
{
    gpio_set_direction (WIFI_SERVER_LED, GPIO_MODE_OUTPUT);
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG ();

    ESP_LOGI (TAG, "Starting server");
    if ( httpd_start (&server, &config) != ESP_OK )
    {
        ESP_LOGE (TAG, "COULD NOT START SERVER");
    }

    httpd_uri_t first_end_point_config = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = on_url_hit };
    httpd_register_uri_handler (server, &first_end_point_config);

    httpd_uri_t temperature_end_point_config = {
        .uri = "/api/temperature",
        .method = HTTP_GET,
        .handler = on_get_temperature };
    httpd_register_uri_handler (server, &temperature_end_point_config);

    httpd_uri_t led_end_point_config = {
        .uri = "/api/led",
        .method = HTTP_POST,
        .handler = on_led_set };
    httpd_register_uri_handler (server, &led_end_point_config);


}