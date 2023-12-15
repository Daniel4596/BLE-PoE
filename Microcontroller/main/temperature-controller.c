#include "temperature-controller.h"
TaskHandle_t controllerTaskHandle = NULL;

/* A simple 2 step controller */

void controllerTask(void *arg) {
    ESP_LOGI(TAG, "temperature controller task started");
    
    float temperatureSetpoint = DEFAULT_SETPOINT;
    float currentTemperature = DEFAULT_SETPOINT;
    uint8_t heaterState = 0;
    uint8_t oldHeaterState = 0;
    uint8_t contactState = 0;

    
    while(1) { 
        if(getSollTemp() != temperatureSetpoint)
        {
            temperatureSetpoint = getSollTemp(); 
            if(temperatureSetpoint<0) temperatureSetpoint=0;
            if(temperatureSetpoint>100) temperatureSetpoint=100; // lobster mode
            ESP_LOGI(TAG, "setpoint changed via BLE to %.2fC", temperatureSetpoint);
            setSollTemp(temperatureSetpoint);
            xQueueSend(mqttPublishSetpointQueue, &temperatureSetpoint, (TickType_t) 0); // publish to state topic
        }       
        else if(xQueueReceive(controllerSetpointQueue, &temperatureSetpoint, (TickType_t) 0)) {
            // sanitize value
            if(temperatureSetpoint<0) temperatureSetpoint=0;
            if(temperatureSetpoint>100) temperatureSetpoint=100; // lobster mode
            ESP_LOGI(TAG, "setpoint changed via MQTT to %.2fC", temperatureSetpoint);
            setSollTemp(temperatureSetpoint);
            xQueueSend(mqttPublishSetpointQueue, &temperatureSetpoint, (TickType_t) 0); // publish to state topic
        }
        xQueueReceive(controllerInputQueue, &currentTemperature, (TickType_t)0);
        xQueueReceive(contactStateToControllerQueue, &contactState, (TickType_t)0);
        if((temperatureSetpoint+HYSTERESIS/2)<currentTemperature) heaterState=0;
        if((temperatureSetpoint-HYSTERESIS/2)>currentTemperature) heaterState=1;
        if(contactState) heaterState=0;
        


        if(heaterState!=oldHeaterState && ENABLE_TEMP_CONTROLLER) {
            uint16_t valve = 0;
            if(heaterState) valve=600;
            else valve=0;
            xQueueSend(receiveValveQueue, (void*)&valve, (TickType_t) 0);
            setThermostat(heaterState);
            oldHeaterState=heaterState;            
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}