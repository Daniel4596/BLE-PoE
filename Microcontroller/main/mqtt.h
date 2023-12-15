#pragma once
#include "global-defs.h"
extern esp_mqtt_client_handle_t client;

extern TaskHandle_t mqttTaskHandle;

extern void mqttTask(void *arg);
extern void mqtt_app_start(void);