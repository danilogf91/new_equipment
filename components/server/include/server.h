#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_http_server.h"

#define WIFI_SERVER_LED 2

extern int16_t temperature;
extern int16_t humidity;

esp_err_t on_url_hit (httpd_req_t* req);

esp_err_t on_get_temperature (httpd_req_t* req);

esp_err_t on_led_set (httpd_req_t* req);

void RegisterEndPoints (void);

