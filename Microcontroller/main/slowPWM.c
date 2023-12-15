#include "slowPWM.h"

TaskHandle_t slowPWMTaskHandle = NULL;

void slowPWMTask(void *arg) {
    ESP_LOGI(TAG, "slow pwm task started");

    // setup valve gpio
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_33);
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_33, 0);

    // setup led D5 gpio
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_16);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_16, 1);
    
    uint16_t counter = 0;
    uint16_t valveVal = 0;

    xQueueSend(mqttPublishValveQueue, (void*)&valveVal, (TickType_t)0);
    
    TickType_t lastWake;
    lastWake = xTaskGetTickCount();
    
    while(1) {
        if(xQueueReceive(receiveValveQueue, &valveVal, (TickType_t)0)) {
            // sanitize values
            if(valveVal>600) valveVal=600;
            xQueueSend(mqttPublishValveQueue, (void*)&valveVal, (TickType_t) 0); // publish to state topic
        }

        if(valveVal>=counter) {
            gpio_set_level(GPIO_NUM_33,1); // valve
            gpio_set_level(GPIO_NUM_16,0); // led
        } else {
            gpio_set_level(GPIO_NUM_33,0); // valve
            gpio_set_level(GPIO_NUM_16,1); // led
        }

        vTaskDelayUntil(&lastWake,pdMS_TO_TICKS(1000));
        counter++;
        if (counter>600) counter = 0;
    }
}