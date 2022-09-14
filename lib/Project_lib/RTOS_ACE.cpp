#include "RTOS_ACE.h"
#include <Screen.h>
#include <Simple_Ace.h>
#include "Heater_PID.h"
#include <Adafruit_ADS1X15.h>

// TaskHandle_t ConfirmBreath,HandleSensor,HandleHeater,HandleData,HandleGraph,HandleSample;
Adafruit_ADS1115 ads;
int16_t plot_point;
double humd;
short adc_CO2;
unsigned long previous;
bool breathe_check;
int value;
SemaphoreHandle_t Sema_sensor;

void Task_Heater_Control (void *pvParameters){
    (void) pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    BaseType_t xWasDelayed;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    for(;;){
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        heater_control();
    }
}

void Task_Confirm_Breath (void *pvParameters){
    (void)pvParameters;
    vTaskSuspend(NULL);
    for(;;){
        printf("Resumed\n");
        int count = 0;
        previous = millis();
        printf("previous: %d\n", previous);
        while(millis()- previous< sampletime-1000){
            // printf("Now: %d\n",getTime());
            humd = read_humidity();
            printf("Humid: %.2f\n", humd);
            printf("Count: %d\n", count);
            if (humd>70){
                Serial.println("Certain a breathe. Recording...");
                breathe_check = true;
                break;
            }
            if (count>100){
                printf("This is not a valid breathe");
                breathe_check = false;
                break;
                //break the loop somhow;
            }  
            count = count+1;
            vTaskDelay(5);
        }
        vTaskSuspend(NULL);
    }
}


void Task_Store_Data (void *pvParameters){
    (void) pvParameters;
    vTaskSuspend(NULL);
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    BaseType_t xWasDelayed;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    printf("Recording\n");
    for(;;){
    int q=0;
    previous = millis();
    printf("previous: %d", previous);
    while(millis()- previous< sampletime-1000){
        // printf("Data Time: %d\n",getTime());
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        adc_CO2 = value;
        CO2_arr[q] = adc_CO2;
        printf("Stored Value: %d,%d\n",q,CO2_arr[q]);
        q = q + 1;
        }   
    printf("Stored Complete\n");
    vTaskResume(HandleConc);  
    }
}

void Task_Plot_Graph(void *pvParameters){
    (void)pvParameters;
    vTaskSuspend(NULL);
    vTaskSuspend(HandleRestoreS);
    // vTaskSuspend(HandleRestoreH);
    vTaskDelay(100);

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5;
    BaseType_t xWasDelayed;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    for(;;){
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        plot_point= value;
        draw_Graph(value); //grab value from the adc reading
    }
}

 
int temp;
int ref;

void Task_Restore_Sensor(void *pvParameters){
    (void) pvParameters;
    for(;;){    
        vTaskSuspend(HandleConc);    
        vTaskSuspend(HandleSensor);
        vTaskSuspend(HandleGraph);
        temp = baselineRead(CO2_channel );
        Serial.println(temp);

        ref = baselineRead(CO2_channel );
        Serial.println(ref);

        vTaskDelay(1);
        if (temp-ref <3 && temp-ref >-3) {
        printf("Found Baseline\n");
        baseline = temp;
        printf("Basleine: %d\n", baseline);
        vTaskResume(HandleSensor);
        vTaskResume(CheckBreath); 
        vTaskResume(HandleGraph);
        printf("Recording\n");
        }
    }
}

void Task_Calculate_Conc(void *pvParameters){
    (void) pvParameters;
    vTaskSuspend(NULL);
    vTaskSuspend(HandleData); 
    printf("Where am I?\n");
    for (;;){
        if(breathe_check){
            calculate_conc();
            draw_result(ratio_Ace);
        }
        else{
            printf("Uncesseful Sample,please retry\n");
        }
       
        // vTaskResume(HandleRestoreH);
        printf("New loop\n");
        printf("firstloop ended\n"); 
        vTaskResume(HandleRestoreS); 
        vTaskDelay(1);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    }
}

void Task_Read_sensor(void *pvParameters){
    (void) pvParameters;
    vTaskSuspend(NULL);
    TickType_t yLastWakeTime;
    const TickType_t yFrequency = 5;
    BaseType_t yWasDelayed;
    yLastWakeTime = xTaskGetTickCount ();
    for (;;){
    yWasDelayed = xTaskDelayUntil( &yLastWakeTime, yFrequency );
    xSemaphoreTake(Sema_sensor,portMAX_DELAY);
    value =  ads.readADC_SingleEnded(CO2_channel);    
    xSemaphoreGive(Sema_sensor);
    // xQueueSend( Sensor_buff,&value,( TickType_t ) 0 );
    }
}

void Task_Check_Breath(void *pvParameters){
    (void) pvParameters;
    vTaskSuspend(NULL);
    for(;;){
        breath_check();
    }

}

void begin_RTOS(){
    Sema_sensor = xSemaphoreCreateMutex();
    Sensor_buff = xQueueCreate(10, sizeof(int16_t));
    if(Sensor_buff == NULL){
    Serial.println("Error creating the queue");
    }
    xTaskCreatePinnedToCore(Task_Read_sensor, "Task_Handle_Sensor", 8096, NULL, 3, &HandleSensor, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Plot_Graph, "Task_Plot_Graph", 8096, NULL, 3, &HandleGraph, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Confirm_Breath, "Task_Confirm_Breath", 2048, NULL, 2, &ConfirmBreath, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Check_Breath, "Task_Check_Breath", 2048, NULL, 3, &CheckBreath, ARDUINO_RUNNING_CORE);
    // xTaskCreatePinnedToCore(Task_Restore_Humd, "Task_Restore_Humd", 2048, NULL, 2, &HandleRestoreH, ARDUINO_RUNNING_CORE);    
    xTaskCreatePinnedToCore(Task_Restore_Sensor, "Task_Restore_Sensor", 2048, NULL, 2, &HandleRestoreS, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Heater_Control, "Task_Heater_Control", 4096, NULL,2, &HandleHeater, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Store_Data, "Task_Store_Data", 4096, NULL, 1, &HandleData, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Calculate_Conc, "Task_Calculate_Conc", 2048, NULL, 1, &HandleConc, ARDUINO_RUNNING_CORE);
 // xTaskCreatePinnedToCore(Task_Plot_Result, "Task_Plot_Result", 2048, NULL, 3, &HandleResult, ARDUINO_RUNNING_CORE);
    vTaskStartScheduler();
}