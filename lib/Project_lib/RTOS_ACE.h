#ifndef RTOS_ACE_H
#define RTOS_ACE_H


#include <Arduino.h>
extern TaskHandle_t CheckBreath,HandleSensor,HandleHeater,HandleData,HandleGraph,HandleSample,HandleRestoreH,HandleRestoreS,HandleConc,ConfirmBreath;
extern QueueHandle_t Sensor_buff;
extern int16_t baseline;
void begin_RTOS();

#endif 

