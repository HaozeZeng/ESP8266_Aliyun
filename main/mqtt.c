#include "mqtt.h"
#include "queue.h"
// 可以连接阿里云不能订阅topic,"Reason":"topic no authorization"
const char *TAG = "MQTT";

const char *clientId = "k1wbgzq58PT.esp8266|securemode=2,signmethod=hmacsha256,timestamp=1729928395023|";
const char *username = "esp8266&k1wbgzq58PT";
const char *passwd = "8a75cecb87f428a5f6a40d47e949f741c287a351eed04333487b3070d8e5bd2d";
const char *mqttHostUrl = "iot-06z00adld5lr6hq.mqtt.iothub.aliyuncs.com";

const int Hostport = 1883;
// 目前只有这两个topic能够响应
const char *MY_Topic_Receive = "/sys/k1wbgzq58PT/esp8266/thing/service/property/set";
const char *MY_Topic_Send = "/sys/k1wbgzq58PT/esp8266/thing/event/property/post";

esp_mqtt_client_handle_t MyClient;
QueueHandle_t ReceiveQueueHandler;

esp_err_t my_mqtt_event_cb(esp_mqtt_event_handle_t event);

esp_mqtt_client_handle_t my_mqtt_init()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .host = mqttHostUrl,
        .port = Hostport,
        .client_id = clientId,
        .username = username,
        .password = passwd,
        .event_handle = my_mqtt_event_cb,
    };
    MyClient = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(MyClient);
    return MyClient;
}

esp_err_t my_mqtt_event_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    char topicname[100];

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_client connected.");
        esp_mqtt_client_subscribe(client, MY_Topic_Receive, 1);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "MQTT client disconnected.");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT subscribed. msg_id = %d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT unsubscribed. msg_id = %d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT published. msg_id = %d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        strncpy(topicname, event->topic, event->topic_len);
        ESP_LOGI(TAG, "MQTT received topic: %s", topicname);
        if (strncmp(event->topic, MY_Topic_Receive, event->topic_len) == 0)
        {
            xQueueSend(ReceiveQueueHandler, &event, 0);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        break;
    }

    return ESP_OK;
}