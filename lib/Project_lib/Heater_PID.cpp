#include "Heater_PID.h"
#include "Simple_ACE.h"
#include <Arduino.h>
#include <PID_v1.h>

double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);

void PID_setup(){
    Input = analogRead(NTCC);
    Setpoint = 1550;
    myPID.SetMode(AUTOMATIC);
}

void heater_control(){
    Input = analogRead(NTCC);
    myPID.Compute();
    ledcWrite(colChannel,Output); //220
}
