#include "global-defs.h"
const char *TAG = "beyond-batteries";
QueueHandle_t controllerInputQueue;
QueueHandle_t contactStateToControllerQueue;
QueueHandle_t mqttPublishTemperatureQueue;
QueueHandle_t mqttPublishValveQueue;
QueueHandle_t receiveValveQueue;
QueueHandle_t mqttPublishContactQueue;
QueueHandle_t temperatureQueue;
QueueHandle_t controllerSetpointQueue;
QueueHandle_t mqttPublishSetpointQueue;