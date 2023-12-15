#include "mqtt.h"

int topicCompare(char * in1, int len1, char * in2, int len2) {
    if (len1!=len2) return 1;
    for (int x = 0; x<len1; x++) if(in1[x]!=in2[x]) return 1;
    return 0;
}

void parseMessageSetPoint(esp_mqtt_event_t * in) {
    int error = 0;
    for(int x = 0; x < in->data_len; x++) { // ensure data is parsable
        if((in->data[x] < 48 || in->data[x]> 57) && in->data[x]!='.') error = 1;
    }
    if(in->data_len > 4) error = 1; // tempstr has to be big enough
    if(!error) {
        char tempstr[5] = {0};
        for (int i = 0; i < in->data_len; i++) tempstr[i]=in->data[i];
        float val = atof(tempstr);
        ESP_LOGI(TAG, "received new setpoint value: %g", val); 
        xQueueSend(controllerSetpointQueue, (void*)&val, (TickType_t) 0);
    } else {
        ESP_LOGI(TAG, "Received non parsable value on setpoint topic");
    }
}

void parseMessageValve(esp_mqtt_event_t * in) {
    int error = 0;
    for(int x = 0; x < in->data_len; x++) { //  ensure data is parsable
        if(in->data[x] < 48 || in->data[x]> 57) error = 1;
    }
    if(in->data_len > 3) error = 1; // tempstr has to be big enough
    if(!error) {
        char tempstr[4] = {0};
        for (int i = 0; i < in->data_len; i++) tempstr[i]=in->data[i];
        uint16_t val = atoi(tempstr);
        ESP_LOGI(TAG, "received new valve value: %i", val); 
        xQueueSend(receiveValveQueue, (void*)&val, (TickType_t) 0);
    } else {
        ESP_LOGI(TAG, "Received non parsable value on valve set topic");
    }
}

esp_mqtt_client_handle_t client = NULL;
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;
    int msg_id;


    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_VALVE_SET, 0);
        ESP_LOGI(TAG, "sent subscribe successful, valveset, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_SETPOINT_SET, 0);
        ESP_LOGI(TAG, "sent subscribe successful, setpointset, msg_id=%d", msg_id);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        printf("LEN=%i\r\n", event->data_len);
        
        if (!topicCompare(event->topic, event->topic_len, TOPIC_SETPOINT_SET, sizeof(TOPIC_SETPOINT_SET)-1)) // cannot use regular strcmp because event->topic is not a 0 terminated string
        {
           parseMessageSetPoint(event);
        } else if(!topicCompare(event->topic, event->topic_len, TOPIC_VALVE_SET, sizeof(TOPIC_VALVE_SET)-1))
        {
            parseMessageValve(event);
        }        
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI, //CONFIG_BROKER_URL,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}



TaskHandle_t mqttTaskHandle = NULL;

void mqttTask(void *arg) 
{
    ESP_LOGI(TAG, "MQTT task started");
    uint16_t valveVal = 0;
    float tempFloat = 0;
    uint8_t contactVal = 0;
    QueueSetHandle_t xQueueSet;
    QueueSetMemberHandle_t xActivatedMember;

    xQueueSet = xQueueCreateSet(4); // 4 queues with 1 member each
    xQueueAddToSet(temperatureQueue, xQueueSet);
    xQueueAddToSet(mqttPublishSetpointQueue, xQueueSet);
    xQueueAddToSet(mqttPublishValveQueue, xQueueSet);
    xQueueAddToSet(mqttPublishContactQueue, xQueueSet);

    while(1) {
        xActivatedMember = xQueueSelectFromSet(xQueueSet, portMAX_DELAY); // wait for new element in any queue, block forever

        if(xActivatedMember == temperatureQueue)
        {
            xQueueReceive(xActivatedMember, &tempFloat, 0);
            ESP_LOGI(TAG, "publish current temperature: %.2fC", tempFloat);      
            char tempstr[20];
            sprintf(tempstr, "%g", tempFloat);
            esp_mqtt_client_publish(client, TOPIC_TEMPERATURE_STATE, tempstr, 0, 0, 0);
        }
        else if(xActivatedMember == mqttPublishSetpointQueue)
        {
            xQueueReceive(xActivatedMember, &tempFloat, 0);
            //ESP_LOGI(TAG, "publish new setpoint: %.2fC", tempFloat);      
            char tempstr[20];
            sprintf(tempstr, "%g", tempFloat);
            esp_mqtt_client_publish(client, TOPIC_SETPOINT_STATE, tempstr, 0, 0, 0);
        }
        else if(xActivatedMember == mqttPublishValveQueue)
        {
            xQueueReceive(xActivatedMember, &valveVal, 0);
            char tempstr[20];
            sprintf(tempstr, "%i", valveVal);
            esp_mqtt_client_publish(client, TOPIC_VALVE_STATE, tempstr, 0, 0, 0);
            ESP_LOGI(TAG, "publish valve state: %i", valveVal); 
        }
        else if(xActivatedMember == mqttPublishContactQueue)
        {
            xQueueReceive(xActivatedMember, &contactVal, 0);
            char stateStr[20];
            sprintf(stateStr, "%i", contactVal);
            esp_mqtt_client_publish(client, TOPIC_WINDOW_CONTACT, stateStr, 0, 0, 0);
            ESP_LOGI(TAG, "publish contact state: %i", contactVal); 
        }
    }
}