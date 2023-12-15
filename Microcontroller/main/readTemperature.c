#include "readTemperature.h"

TaskHandle_t temperatureTaskHandle = NULL;

void temperatureTask(void *arg)
{
    ESP_LOGI(TAG, "temperature task started");
    float temperatureOld = 0;

    onewire_rmt_config_t config = {
        .gpio_pin = GPIO_NUM_32,
        .max_rx_bytes = 10, // 10 tx bytes(1byte ROM command + 8byte ROM number + 1byte device command)
    };

    // install new 1-wire bus
    onewire_bus_handle_t handle;
    ESP_ERROR_CHECK(onewire_new_bus_rmt(&config, &handle));
    ESP_LOGI(TAG, "1-wire bus installed");

    // create 1-wire rom search context
    onewire_rom_search_context_handler_t context_handler;
    ESP_ERROR_CHECK(onewire_rom_search_context_create(handle, &context_handler));

    uint8_t device_num = 0;
    uint8_t device_rom_id[EXAMPLE_ONEWIRE_MAX_DEVICES][8];

    // search for devices on the bus
    do {
        esp_err_t search_result = onewire_rom_search(context_handler);

        if (search_result == ESP_ERR_INVALID_CRC) {
            continue; // continue on crc error
        } else if (search_result == ESP_FAIL) {
            break; // break on finish
        }
        else if(search_result == ESP_ERR_NOT_FOUND) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue; // do not break on no device. Some devices are unreliable so try again.
        }

        ESP_ERROR_CHECK(onewire_rom_get_number(context_handler, device_rom_id[device_num]));
        ESP_LOGI(TAG, "found device with rom id " ONEWIRE_ROM_ID_STR, ONEWIRE_ROM_ID(device_rom_id[device_num]));
        device_num ++;
    } while (device_num < EXAMPLE_ONEWIRE_MAX_DEVICES);

    // delete 1-wire rom search context
    ESP_ERROR_CHECK(onewire_rom_search_context_delete(context_handler));
    ESP_LOGI(TAG, "%d device%s found on 1-wire bus", device_num, device_num > 1 ? "s" : "");

    // convert and read temperature
    while (device_num > 0) {
        esp_err_t err;
        vTaskDelay(pdMS_TO_TICKS(200));

        // set all sensors' temperature conversion resolution
        err = ds18b20_set_resolution(handle, NULL, DS18B20_RESOLUTION_12B);
        if (err != ESP_OK) {
            continue;
        }

        // trigger all sensors to start temperature conversion
        err = ds18b20_trigger_temperature_conversion(handle, NULL); // skip rom to send command to all devices on the bus
        if (err != ESP_OK) {
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // 12-bit resolution needs 750ms to convert, wait a little longer..

        // get temperature from sensors
        for (uint8_t i = 0; i < device_num; i ++) {
            float temperature;
            err = ds18b20_get_temperature(handle, device_rom_id[i], &temperature); // read scratchpad and get temperature
            if (err != ESP_OK) {
                continue;
            }
            if (temperature!=temperatureOld) // only put value into queue on change
            {
                //ESP_LOGI(TAG, "temperature: %.2fC", temperature);
                setIstTemp(temperature);
                xQueueSend(temperatureQueue, (void*)&temperature, (TickType_t)0);
                xQueueSend(controllerInputQueue, (void*)&temperature, (TickType_t)0);
                temperatureOld=temperature;
            }
        }
    }

    ESP_ERROR_CHECK(onewire_del_bus(handle));
    ESP_LOGI(TAG, "1-wire bus deleted");
}