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
//Global variables (time, flags)
unsigned long  glbTime           = 0;

//Gyroscope
float          axisX             = 0;
float          axisY             = 0;

//GSM Comm
byte           person1           = 0;
byte           person2           = 0;
int            value             = 0;
char           number[20];
char           number2[20];
char           value_str[5];

// Alarm
unsigned long  armStartedTime    = 0;

boolean        systemArmed       = false;
boolean        armState          = false;
boolean        alarmState        = false;
boolean        sirenState        = false;

float          sesitivityX       = 1.0;
float          sesitivityY       = 1.0;
float          armedGyroStateX   = 0;
float          armedGyroStateY   = 0;

boolean        armTimeSaved      = false;

void setup() {
  Serial.begin(38400);
  IOInit();
  gyroInit();
  gsmInit();
}

void loop() {
  glbTime = millis();
  checkIncomingCallToArmOrDisarm(ARMNUMBER, armFlag);
  calculateGyroscopeData(axisX, axisY);
  //delay(500);
}


