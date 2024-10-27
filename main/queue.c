#include "queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

QueueHandle_t ReceiveQueueHandler;
QueueHandle_t SendQueueHandler;

bool InfoQueueInit()
{
    ReceiveQueueHandler = xQueueCreate(1, sizeof(esp_mqtt_event_handle_t *));
    SendQueueHandler = xQueueCreate(1, sizeof(esp_mqtt_event_handle_t *));
    return (ReceiveQueueHandler != NULL && SendQueueHandler != NULL);
}