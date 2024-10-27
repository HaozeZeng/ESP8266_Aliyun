#include "task.h"
#include "mqtt.h"
#include "queue.h"
#include "dataparse.h"
#include "APP.h"

static const char *TAG = "Tasks";

TaskHandle_t ReceiveTaskHandler;
TaskHandle_t SendTaskHandler;
TaskHandle_t defaultTaskHandler;

extern QueueHandle_t ReceiveQueueHandler;
extern esp_mqtt_client_handle_t MyClient;
extern const char *JSONSendTemplate;
extern const char *MY_Topic_Send;

void sendTask(void *para);
void receiveTask(void *para);
void defaultTask()
{
    if (!InfoQueueInit())
    {
        ESP_LOGE(TAG, "Queue initialization failed");
        vTaskDelete(NULL);
    }
    else
    {
        ESP_LOGI(TAG, "Queue initialization successful");
    }
    LED_Init();
    my_mqtt_init();

    xTaskCreatePinnedToCore(receiveTask, "receiveTask", 1024, NULL, 2, ReceiveTaskHandler, 1);
    xTaskCreatePinnedToCore(sendTask, "sendTask", 1024, NULL, 2, SendTaskHandler, 1);

    vTaskDelete(NULL);
}
bool LEDTogger = true;
uint16_t objvalue = 0;
void receiveTask(void *para)
{

    esp_mqtt_event_handle_t ReceivedEvent;
    struct receiveData *rD;
    while (1)
    {
        xQueueReceive(ReceiveQueueHandler, &ReceivedEvent, portMAX_DELAY);
        rD = parse_json_2(ReceivedEvent->data, ReceivedEvent->data_len);
        if (rD)
        {
            if (rD->receiveed)
            {
                LEDTogger = rD->LightSwitch;
                objvalue = rD->CurrentTemperature;
                gpio_set_level(GPIO_NUM_5, LEDTogger);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void sendTask(void *para)
{

    // printf("sendTask start \r\n");
    char sendjson[100];

    while (1)
    {
        if (objvalue % 2)
            LEDTogger = !LEDTogger;
        sprintf(sendjson, JSONSendTemplate, objvalue, 30, LEDTogger);
        // printf("sendjson is %s\r\n", sendjson);
        esp_mqtt_client_publish(MyClient, MY_Topic_Send, sendjson, strlen(sendjson), 1, 0);
        vTaskDelay(pdMS_TO_TICKS(60000));
        objvalue++;
    }
}