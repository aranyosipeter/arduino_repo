#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include  <math.h>

// GSM functions
#include  <gsm_module.h>

// GSM Comm
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"

// Gyroscope
MPU6050 accelgyro;
CallGSM call;
SMSGSM  sms;

#define OUTPUT_READABLE_ACCELGYRO 
#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY     65.536 
#define M_PI                      3.14159265359	     
#define dt                        0.01	

#define RESET                     3
#define LED                       6
#define IRQ                       2

//Gyroscope
int16_t    acce[3];
int16_t    giro[3];
float      axisX             = 0;
float      axisY             = 0;
//GSM Comm
byte       person1           = 0;
byte       person2           = 0;
int        value             = 0;
char       number[20];
char       number2[20];
char       value_str[5];

void setup() {
    Wire.begin();
    Serial.begin(38400);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    IOInit();
}

void loop() {
  accelgyro.getAcceleration(&acce[0], &acce[1], &acce[2]);
  accelgyro.getRotation(&giro[0], &giro[1], &giro[2]);
  ComplementaryFilter(acce, giro, &axisY, &axisX);
}

// I/O init
void IOInit(){
  pinMode(IRQ, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(RESET, OUTPUT);
}

// Complementary Filter written by Pieter-Jan Van de Maele 
void ComplementaryFilter(int16_t accData[3], int16_t gyrData[3], float *roll, float *pitch){ 
  float pitchAcc, rollAcc;               
    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
    *roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
 
    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768){
	// Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2((float)accData[1], (float)accData[2]) * 180 / M_PI;
        *pitch = *pitch * 0.98 + pitchAcc * 0.02;
 
	// Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2((float)accData[0], (float)accData[2]) * 180 / M_PI;
        *roll = *roll * 0.98 + rollAcc * 0.02;
    }
} 
