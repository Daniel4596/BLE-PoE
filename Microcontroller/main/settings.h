#pragma once

/* MQTT */

#define MQTT_BROKER_URI "mqtt://10.10.11.3"
#define TOPIC_TEMPERATURE_STATE "beyond-batteries/temperature/state"
#define TOPIC_VALVE_STATE "beyond-batteries/valve/state"
#define TOPIC_VALVE_SET "beyond-batteries/valve/set"
#define TOPIC_WINDOW_CONTACT "beyond-batteries/contact/state"
#define TOPIC_SETPOINT_STATE "beyond-batteries/setpoint/state"
#define TOPIC_SETPOINT_SET "beyond-batteries/setpoint/set"

/* IP-Settings */
#define USE_STATIC 1 // set to 1 to disable DHCP, set to 0 to use DHCP
#define STATIC_IP_ADDR "10.10.11.123"
#define STATIC_NETMASK_ADDR "255.255.255.0"
#define STATIC_GW_ADDR "10.10.11.3"
#define MAIN_DNS_SERVER "10.10.11.3"
#define BACKUP_DNS_SERVER "0.0.0.0"

/* Temperature Controller */
#define ENABLE_TEMP_CONTROLLER 1
#define DEFAULT_SETPOINT 21.0
#define HYSTERESIS 0