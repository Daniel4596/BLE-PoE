#include "global-defs.h"




void app_main(void)
{
    
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_ERROR_CHECK(nvs_flash_init());

    initEthIP(); // enables ethernet + ip stack
    initBLE();
    
    mqtt_app_start(); // start MQTT service, connect to broker
    
    // init queues
    controllerInputQueue = xQueueCreate(1, sizeof(float));
    temperatureQueue = xQueueCreate(1, sizeof(float)); 
    mqttPublishValveQueue = xQueueCreate(1, sizeof(uint16_t));
    mqttPublishSetpointQueue = xQueueCreate(1, sizeof(float));
    receiveValveQueue = xQueueCreate(1, sizeof(uint16_t));
    mqttPublishContactQueue = xQueueCreate(1, sizeof(uint8_t));
    controllerSetpointQueue = xQueueCreate(1, sizeof(float)); 
    contactStateToControllerQueue = xQueueCreate(1, sizeof(uint8_t));
    
    // start tasks
    xTaskCreate(mqttTask, "MQTT Task", 4096, NULL, 10, &mqttTaskHandle); // handle MQTT incoming and outgoing MQTT messages
    xTaskCreate(temperatureTask, "Temperature Task", 4096, NULL, 10, &temperatureTaskHandle); // periodically reads DS18B20 temperature sensor
    xTaskCreate(contactTask, "Window Contact Task", 2048, NULL, 10, &contactTaskHandle); // periodically reads window contact input, does debounceing
    xTaskCreate(slowPWMTask, "Slow PWM Task", 2048, NULL, 10, &slowPWMTaskHandle); // ultra slow PWM for the valve
    xTaskCreate(controllerTask, "Temperature Controller Task", 2048, NULL, 10, &controllerTaskHandle); // rudimentary temperature controller
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}