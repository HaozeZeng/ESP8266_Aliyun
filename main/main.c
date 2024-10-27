#include "mqttbasedef.h"
#include "wifi.h"
#include "mqtt.h"
#include "task.h"

static const char *TAG = "Main";
extern void defaultTask();
extern TaskHandle_t defaultTaskHandler;
void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGI(TAG, "Start");

    // 初始化 Wi-Fi
    wifi_connect_init();

    // 创建默认任务
    xTaskCreatePinnedToCore(defaultTask, "defaultTask", 2048, NULL, 3, defaultTaskHandler, 1);
}