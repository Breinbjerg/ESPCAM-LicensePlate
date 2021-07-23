/**
 * @file json.c
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-06-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "json.h"

/** Used for debug messages */ 
static const char *TAG_JSON = "JSON";


int8_t json_load_config(const char *jsonstring, system_defs *sys)
{
    int8_t status = 0;
    const cJSON *DeviceName = NULL;
    const cJSON *Sensors = NULL;
    const cJSON *Sensor = NULL;

    cJSON *config_json = cJSON_Parse(jsonstring);
    if (config_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            ESP_LOGE(TAG_JSON, "Error before: %s\n", error_ptr);
        }
        status = -1;
        goto end;
    }

    DeviceName = cJSON_GetObjectItemCaseSensitive(config_json, "name");
    if (cJSON_IsString(DeviceName) && (DeviceName->valuestring != NULL))
    {
        ESP_LOGI(TAG_JSON, "Checking name: \"%s\"", DeviceName->valuestring);
        if (strncmp(DeviceName->valuestring, sys->DeviceName, strlen(DeviceName->valuestring)))
        {
            ESP_LOGE(TAG_JSON, "Device name and server name does not match");
            status = -1;
            goto end;
        }
    }

    Sensors = cJSON_GetObjectItemCaseSensitive(config_json, "Sensors");
    cJSON_ArrayForEach(Sensor, Sensors)
    {
        cJSON *SensorName = cJSON_GetObjectItemCaseSensitive(Sensor, "Name");
        cJSON *SampleRate = cJSON_GetObjectItemCaseSensitive(Sensor, "SampleRate");
        if (cJSON_IsString(SensorName) && (SensorName->valuestring != NULL))
        {
            ESP_LOGI(TAG_JSON, "Name of the sensor: \"%s\"", SensorName->valuestring);
            /** If 0 the strings are equal */
            if (!strncmp(SensorName->valuestring, "ADXL", strlen(SensorName->valuestring)))
            {
                if (cJSON_IsNumber(SampleRate))
                {
                    ESP_LOGI(TAG_JSON, "Samplerate is %lf", SampleRate->valuedouble);
                    sys->sampleFreqADXL = (int)SampleRate->valuedouble;
                    status = 0;
                }
                else
                {
                    ESP_LOGE(TAG_JSON, "Error: Samplerate not a number: ADXL");
                    status = -1;
                    goto end;
                }
            }
            else if (!strncmp(SensorName->valuestring, "MIC", strlen(SensorName->valuestring)))
            {
                if (cJSON_IsNumber(SampleRate))
                {
                    ESP_LOGI(TAG_JSON, "Samplerate is %lf", SampleRate->valuedouble);
                    sys->sampleFreqMIC = (int)SampleRate->valuedouble;
                    status = 0;
                }
                else
                {
                    ESP_LOGE(TAG_JSON, "Error: Samplerate not a number: MIC");
                    status = -1;
                    goto end;
                }
            }
            else
            {
                ESP_LOGE(TAG_JSON, "Did not find correct sensor in load config");
                status = -1;
                goto end;
            }
        }
    }

end:
    cJSON_Delete(config_json);
    return status;
}

/**
 * @brief Creates json string with system config. 
 * 
 * @param sys System define struct.
 * @param with_samplerates 
 *          -- 1: Creates string with samplerates
 *          -- 0: Creates string only with device name
 * @return char* 
 */
char *json_create_config_string(system_defs *sys, uint8_t with_samplerates)
{
    char *string = NULL;
    cJSON *Name = NULL;
    cJSON *sampleFreqADXL = NULL;
    cJSON *sampleFreqMIC = NULL;

    cJSON *config = cJSON_CreateObject();
    if (config == NULL)
    {
        ESP_LOGE(TAG_JSON, "ERROR: Creating json object for config");
        goto end;
    }

    Name = cJSON_CreateString("Name");
    if (Name == NULL)
    {
        ESP_LOGE(TAG_JSON, "ERROR: Creating json string: Name");
        goto end;
    }

    /** Add item to json object */
    cJSON_AddItemToObject(config, "Name", Name);
    if (with_samplerates == 1)
    {
        sampleFreqADXL = cJSON_CreateNumber(sys->sampleFreqADXL);
        if (Name == sampleFreqADXL)
        {
            ESP_LOGE(TAG_JSON, "ERROR: Creating json number: sampleFreqADXL");
            goto end;
        }

        /** Add item to json object */
        cJSON_AddItemToObject(config, "sampleFreqADXL", sampleFreqADXL);

        sampleFreqMIC = cJSON_CreateNumber(sys->sampleFreqMIC);
        if (sampleFreqMIC == NULL)
        {
            ESP_LOGE(TAG_JSON, "ERROR: Creating json number: sampleFreqMIC");
            goto end;
        }

        /** Add item to json object */
        cJSON_AddItemToObject(config, "sampleFreqMIC", sampleFreqMIC);

        string = cJSON_Print(config);
        if (string == NULL)
        {
            ESP_LOGE(TAG_JSON, "ERROR: Creating json string");
        }
    }

end:
    cJSON_Delete(config);
    return string;
}