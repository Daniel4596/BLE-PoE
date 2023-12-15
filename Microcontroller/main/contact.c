#include "contact.h"

TaskHandle_t contactTaskHandle = NULL;

void contactTask(void *arg) 
{
    ESP_LOGI(TAG, "contact task started");
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_35);
    gpio_set_direction(GPIO_NUM_35, GPIO_MODE_INPUT);
    uint8_t contactState = 0;
    uint8_t newContactState = 0;
    uint8_t init = 1;

    while(1) {
        newContactState = gpio_get_level(GPIO_NUM_35);
        if(newContactState != contactState || init) {
            vTaskDelay(pdMS_TO_TICKS(50));
            if(gpio_get_level(GPIO_NUM_35) == newContactState) {
                contactState=newContactState;
                ESP_LOGI(TAG, "contact state changed to %d", contactState);
                setWindow(contactState);
                xQueueSend(mqttPublishContactQueue, &contactState, pdMS_TO_TICKS(100));
                xQueueSend(contactStateToControllerQueue, &contactState, pdMS_TO_TICKS(100));
                init=0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}