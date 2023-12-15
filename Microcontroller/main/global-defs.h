#pragma once
//#include <stdio.h>
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "freertos/semphr.h"
//#include "esp_log.h"
//#include "esp_check.h"




#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "driver/gpio.h"


#include "global-defs.h"
#include "esp_log.h"
#include "esp_check.h"
#include "onewire_bus.h"
#include "ds18b20.h"

#include "esp_eth.h"
#include <esp_eth_mac.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "mqtt_client.h"
#include "ethernet-ip.h"
#include "settings.h"
#include "mqtt.h"

#include "contact.h"
#include "readTemperature.h"
#include "slowPWM.h"
#include "temperature-controller.h"

//Include for BLE
#include <stdlib.h>
#include <string.h>

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include "ble.h"


extern const char *TAG;

/* Queues */
extern QueueHandle_t controllerInputQueue;
extern QueueHandle_t contactStateToControllerQueue;
extern QueueHandle_t mqttPublishTemperatureQueue;
extern QueueHandle_t mqttPublishValveQueue;
extern QueueHandle_t receiveValveQueue;
extern QueueHandle_t temperatureQueue;
extern QueueHandle_t mqttPublishContactQueue;
extern QueueHandle_t controllerSetpointQueue;
extern QueueHandle_t mqttPublishSetpointQueue;