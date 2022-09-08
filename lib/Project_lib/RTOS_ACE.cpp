#include "RTOS_ACE.h"
#include <Screen.h>
#include <Simple_Ace.h>
#include "Heater_PID.h"
#include <Adafruit_ADS1X15.h>

// TaskHandle_t CheckBreath,HandleSensor,HandleHeater,HandleData,HandleGraph,HandleSample;
Adafruit_ADS1115 ads;
int16_t plot_point;
double humd;
short adc_CO2;
unsigned long previous;



void Task_Check_Breath (void *pvParameters){
    (void)pvParameters;
    vTaskSuspend(CheckBreath);
    printf("Resumed\n");
    int count = 0;
    previous = getTime();
    printf("previous: %d\n", previous);
    while(getTime()- previous< sampletime-1){
        printf("Now: %d\n",getTime());
        humd = read_humidity();
        printf("Humid: %.2f\n", humd);
        // printf("Data Time: %d",getTime());
        printf("Count: %d\n", count);
        if (humd>70){
            Serial.println("Certain a breathe. Recording...");
            vTaskSuspend(CheckBreath);
        }
        if (count>100){
            printf("This is not a valid breathe");
            vTaskSuspend(HandleData);
            break;
            //break the loop somhow;
        }  
        count = count+1;  
        vTaskDelay(100);
    }
    vTaskSuspend(CheckBreath);
}

// void Task_Read_Sensor (void *pvParameters){
//     (void) pvParameters;
//     for(;;){

//         vTaskDelay(100);
//     }
// }
void Task_Heater_Control (void *pvParameters){
    (void) pvParameters;
    for(;;){
        heater_control();
        vTaskDelay(100);
    }
}

void Task_Store_Data (void *pvParameters){
    (void) pvParameters;
    vTaskSuspend(HandleData);
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    BaseType_t xWasDelayed;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    int q=0;
    previous = getTime();
    printf("previous: %d", previous);
    while(getTime()- previous< sampletime-1){
        printf("Data Time: %d\n",getTime());
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        adc_CO2 = ads.readADC_SingleEnded(CO2_channel);
        CO2_arr[q] = adc_CO2;
        printf("Stored Value: %d\n",CO2_arr[q]);
        q = q + 1;
    }
    printf("Stored Complete");
    vTaskResume(HandleSample);
    vTaskSuspend(HandleData);
}

void Task_Plot_Graph(void *pvParameters){
    (void)pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    BaseType_t xWasDelayed;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    for(;;){
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        plot_point= ads.readADC_SingleEnded(CO2_channel);
        draw_Graph(plot_point); //grab value from the adc reading
    }
}

void Task_Sample_Breath(void *pvParameters){
  (void) pvParameters;
  for(;;){
      sample_collection();
  }
}

void Task_Restore_Humd(void *pvParameters){
    (void) pvParameters;
    for(;;){
        restore_humidity();
        vTaskDelay(50);
    }
}

void Task_Restore_Sensor(void *pvParameters){
    (void) pvParameters;
    vTaskSuspend(HandleSample);
    for(;;){
        restore_baseline();
        vTaskDelay(50);
    }
}

void begin_RTOS(){
    xTaskCreatePinnedToCore(Task_Check_Breath, "Task_Check_Breath", 2048, NULL, 2, &CheckBreath, ARDUINO_RUNNING_CORE);
    // xTaskCreatePinnedToCore(Task_Read_Sensor, "TTask_Read_Sensor", 2048, NULL, 1, &HandleSensor, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Heater_Control, "Task_Heater_Control", 2048, NULL, 2, &HandleHeater, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Store_Data, "Task_Store_Data", 4096, NULL, 3, &HandleData, ARDUINO_RUNNING_CORE);
    delay(500);
    xTaskCreatePinnedToCore(Task_Plot_Graph, "Task_Plot_Graph", 4096, NULL, 1, &HandleGraph, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Sample_Breath, "Task_Sample_Breathe", 4096, NULL, 2, &HandleSample, ARDUINO_RUNNING_CORE);    
    xTaskCreatePinnedToCore(Task_Restore_Humd, "Task_Restore_Humd", 4096, NULL, 1, &HandleRestoreH, ARDUINO_RUNNING_CORE);    
    xTaskCreatePinnedToCore(Task_Restore_Sensor, "Task_Restore_Sensor", 4096, NULL, 1, &HandleRestoreS, ARDUINO_RUNNING_CORE);    
}