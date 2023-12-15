#pragma once

#include "global-defs.h"


void initBLE(void);
float getSollTemp();
void setSollTemp(float temp);
void setIstTemp(uint8_t temp);
void setWindow(uint8_t window);
void setThermostat(uint8_t thermostat);
void changeName(void);
/* Attributes State Machine */
enum
{
    IDX_SVC,
    IDX_CHAR_SOLL_TEMP,
    IDX_CHAR_VAL_SOLL_TEMP,
    IDX_CHAR_CFG_SOLL_TEMP,

    IDX_CHAR_IST_TEMP,
    IDX_CHAR_VAL_IST_TEMP,
    IDX_CHAR_CFG_IST_TEMP,

    IDX_CHAR_WINDOW,
    IDX_CHAR_VAL_WINDOW,
    IDX_CHAR_CFG_WINDOW,

    IDX_CHAR_THERMOSTAT,
    IDX_CHAR_VAL_THERMOSTAT,
    IDX_CHAR_CFG_THERMOSTAT,

    IDX_CHAR_NAME,
    IDX_CHAR_VAL_NAME,

    TH_IDX_NB,
};
