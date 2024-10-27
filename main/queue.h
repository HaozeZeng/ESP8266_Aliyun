#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "mqttbasedef.h"

extern QueueHandle_t ReceiveQueueHandler;
extern QueueHandle_t SendQueueHandler;

bool InfoQueueInit();

#endif // QUEUE_H