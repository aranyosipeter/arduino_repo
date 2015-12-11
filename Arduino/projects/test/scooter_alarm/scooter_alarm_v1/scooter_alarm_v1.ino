//
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include  <math.h>

//GSM Comm
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"

//GSM functions
#include  "gsm_module.h"

//Gyroscope functions
#include  "gyro_module.h"

//Alarm functions
#include  "alarm_functions.h"

//Variables
//Gyroscope
float      axisX             = 0;
float      axisY             = 0;

//GSM Comm
byte       person1           = 0;
byte       person2           = 0;
int        value             = 0;
char       number[20];
char       number2[20];
char       value_str[5];

// Alarm
boolean    armFlag           = false;
boolean    alarmFlag         = false;
boolean    sirenFlag         = false;
float      armedValue        = 0;
float      sesitivityX       = 1.0;
float      sesitivityY       = 1.0;

void setup() {
  Serial.begin(38400);
  IOInit();
  gyroInit();
  gsmInit();
}

void loop() {
  checkIncomingCallToArmOrDisarm(ARMNUMBER, armFlag);
  calculateGyroscopeData(axisX, axisY);
  //delay(500);
}


