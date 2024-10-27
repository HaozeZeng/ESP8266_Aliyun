#include "dataparse.h"

static const char *TAG = "JSON";
const char *JSONSendTemplate =
    "{"
    "\"method\": \"thing.service.property.post\","
    "\"id\": \"%d\","
    "\"params\": {"
    "\"CurrentTemperature\": %d,"
    "\"LightSwitch\": %d"
    " },"
    " \"version\": \"1.0.0\""
    "}";
struct receiveData rD;

// function for Ali云信息流转
struct receiveData *parse_json_2(const char *json_data, uint16_t len)
{
    // ESP_LOGI(TAG, "Start JSON parse");
    char *json_buffer = (char *)malloc(len + 1);
    if (json_buffer == NULL)
    {
        ESP_LOGE(TAG, "JSON buffer allocation failed");
        return NULL;
    }

    strncpy(json_buffer, json_data, len);
    json_buffer[len] = '\0';
    printf("%s\r\n", json_buffer);

    cJSON *root = cJSON_Parse(json_buffer);
    if (root == NULL)
    {
        ESP_LOGE(TAG, "JSON parse error");
        rD.receiveed = false;
    }
    else
    {
        rD.receiveed = true;
        cJSON *items = cJSON_GetObjectItem(root, "items");
        cJSON *PosterName = cJSON_GetObjectItem(root, "deviceName");
        cJSON *LightSwitch = cJSON_GetObjectItem(items, "LightSwitch");
        cJSON *CurrentTemperature = cJSON_GetObjectItem(items, "CurrentTemperature");

        if (PosterName && cJSON_IsString(PosterName))
        {
            rD.PosterName = PosterName->valuestring;
            ESP_LOGI(TAG, "DeviceName: %s", PosterName->valuestring);
        }
        else
        {
            ESP_LOGE(TAG, "DeviceName attribute get error");
        }
        if (LightSwitch)
        {
            cJSON *value = cJSON_GetObjectItem(LightSwitch, "value");
            if (value && cJSON_IsNumber(value))
            {

                rD.LightSwitch = value->valueint;
                ESP_LOGI(TAG, "LightSwitch: %d", value->valueint);
            }
        }
        else
        {
            ESP_LOGE(TAG, "LightSwitch attribute get error");
        }

        if (CurrentTemperature)
        {
            cJSON *value = cJSON_GetObjectItem(CurrentTemperature, "value");
            if (value && cJSON_IsNumber(value))
            {
                rD.CurrentTemperature = value->valueint;
                ESP_LOGI(TAG, "CurrentTemperature: %d", value->valueint);
            }
        }
        else
        {
            ESP_LOGE(TAG, "CurrentTemperature attribute get error");
        }
    }
    cJSON_Delete(root);
    free(json_buffer);
    return &rD;
}

// function for Ali调试
struct receiveData *parse_json(const char *json_data, uint16_t len)
{
    // ESP_LOGI(TAG, "Start JSON parse");
    char *json_buffer = (char *)malloc(len + 1);
    if (json_buffer == NULL)
    {
        ESP_LOGE(TAG, "JSON buffer allocation failed");
        return NULL;
    }

    strncpy(json_buffer, json_data, len);
    json_buffer[len] = '\0';
    printf("%s\r\n", json_buffer);

    cJSON *root = cJSON_Parse(json_buffer);
    if (root == NULL)
    {
        ESP_LOGE(TAG, "JSON parse error");
        rD.receiveed = false;
    }
    else
    {
        rD.receiveed = true;
        cJSON *params = cJSON_GetObjectItem(root, "params");
        cJSON *LightSwitch = cJSON_GetObjectItem(params, "LightSwitch");
        cJSON *CurrentTemperature = cJSON_GetObjectItem(params, "CurrentTemperature");
        if (LightSwitch && cJSON_IsNumber(LightSwitch))
        {
            rD.LightSwitch = LightSwitch->valueint;
            ESP_LOGI(TAG, "LightSwitch: %d", LightSwitch->valueint);
        }
        else
        {
            ESP_LOGE(TAG, "LightSwitch attribute get error");
        }

        if (CurrentTemperature && cJSON_IsNumber(CurrentTemperature))
        {
            rD.CurrentTemperature = CurrentTemperature->valueint;
            ESP_LOGI(TAG, "CurrentTemperature: %d", CurrentTemperature->valueint);
        }
        else
        {
            ESP_LOGE(TAG, "CurrentTemperature attribute get error");
        }
    }
    cJSON_Delete(root);
    free(json_buffer);
    return &rD;
}