#include <stdio.h>
#include <string.h>
#include "nvs_wifi_credentials.h"
#include "esp_log.h"

static const char* TAG = "nvs wifi credentials";

void write_nvs_test (char* ssid, char* password)
{
    //Init nvs system
    init_nvs_test ();

    NVS_Data_t nvs_struct;
    nvs_handle test;
    size_t required_size = sizeof (nvs_struct);
    esp_err_t err = nvs_open ("storage", NVS_READWRITE, &test);

    // Write
    // printf ("Adding text to NVS Struct... ");
    ESP_LOGI (TAG, "Adding wifi credentials to NVS Struct...");
    sprintf (nvs_struct.ssid, "%s", ssid);
    sprintf (nvs_struct.pass, "%s", password);

    err = nvs_set_blob (test, "nvs_struct", ( const void* ) &nvs_struct, required_size);

    if ( err != ESP_OK )
    {
        ESP_LOGI (TAG, "Failed wifi save credentials");
    }
    else
    {
        ESP_LOGI (TAG, "Done wifi save credentials");
    }


    // printf (( err != ESP_OK ) ? "Failed!\n" : "Done\n");

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    ESP_LOGI (TAG, "Committing updates in NVS ...");
    // printf ("Committing updates in NVS ... ");
    err = nvs_commit (test);
    //printf (( err != ESP_OK ) ? "Failed!\n" : "Done\n");

    if ( err != ESP_OK )
    {
        ESP_LOGI (TAG, "Failed commit credentials");
    }
    else
    {
        ESP_LOGI (TAG, "Done commit credentials");
    }

    // Close
    nvs_close (test);
}

esp_err_t read_nvs_test (nvs_handle* my_handle, NVS_Data_t* data_retrieve)
{
    //Init nvs system
    init_nvs_test ();

    NVS_Data_t nvs_struct;
    size_t required_size;
    nvs_handle test = *my_handle;
    esp_err_t err = nvs_open ("storage", NVS_READONLY, &test);
    if ( err != ESP_OK )
    {
        // printf ("Error (%s) opening NVS handle!\n", esp_err_to_name (err));
        ESP_LOGE (TAG, "Error (%s) opening NVS handle!", esp_err_to_name (err));
        sprintf (data_retrieve->ssid, "%s", "");
        sprintf (data_retrieve->pass, "%s", "");
        nvs_close (test);
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI (TAG, "Reading wifi credentials from NVS ...");
        // printf ("Done\n");
        // printf ("Reading string from NVS ... ");
        err = nvs_get_blob (test, "nvs_struct", NULL, &required_size);
        err = nvs_get_blob (test, "nvs_struct", ( void* ) &nvs_struct, &required_size);
        switch ( err )
        {
            case ESP_OK:
                // printf ("Done\n\n");
                // printf ("ssid = %s\n\n", nvs_struct.ssid);
                // printf ("pass = %s\n\n", nvs_struct.pass);
                ESP_LOGI (TAG, "ssid = %s", nvs_struct.ssid);
                ESP_LOGI (TAG, "pass = %s", nvs_struct.pass);

                sprintf (data_retrieve->ssid, "%s", nvs_struct.ssid);
                sprintf (data_retrieve->pass, "%s", nvs_struct.pass);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                // printf ("The value is not initialized yet!\n");
                ESP_LOGW (TAG, "The value is not initialized yet!");
                required_size = sizeof (nvs_struct);
                memset (nvs_struct.ssid, 0, sizeof (nvs_struct.ssid));
                memset (nvs_struct.pass, 0, sizeof (nvs_struct.pass));
                break;
            default:
                // printf ("Error (%s) reading!\n", esp_err_to_name (err));
                ESP_LOGE (TAG, "Error (%s) reading!\n", esp_err_to_name (err));
        }
    }
    nvs_close (test);
    return ESP_OK;
}

esp_err_t init_nvs_test (void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init ();
    if ( err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND )
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK (nvs_flash_erase ());
        err = nvs_flash_init ();
    }
    ESP_ERROR_CHECK (err);

    // Open
    // printf ("\n");
    // printf ("Opening Non-Volatile Storage (NVS) handle... \n");
    ESP_LOGI (TAG, "Opening Non-Volatile Storage (NVS) handle...");
    return ESP_OK;
}