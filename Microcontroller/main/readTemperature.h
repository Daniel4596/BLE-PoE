#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#include "esp_log.h"
#include "esp_check.h"
#include "onewire_bus.h"
#include "ds18b20.h"
#include "global-defs.h"

extern TaskHandle_t temperatureTaskHandle;

#define EXAMPLE_ONEWIRE_MAX_DEVICES 1

extern void temperatureTask(void *arg);