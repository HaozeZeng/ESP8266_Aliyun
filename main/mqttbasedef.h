#ifndef MQTTBASEDEF_H
#define MQTTBASEDEF_H
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "../build/config/sdkconfig.h"
#include "../../components/mqtt/esp-mqtt/include/mqtt_client.h"
#include "cJSON.h"
#endif