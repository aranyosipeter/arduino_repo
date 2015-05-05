#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include "header.h"


#define LED 4
#define PIR 2
#define BUTTON 5
#define INTERRUPT 3

// Deklaralas //

boolean alarmSent = true;
boolean regPushB = false;
boolean erasePushB = false;
boolean state = false;
boolean regFlag = false;
boolean timeFlag = false;
boolean sendTiming = false;

unsigned long time;
unsigned long sendTime = 0;
unsigned long prevTime = 0;
unsigned long timeOut = 0;
unsigned long buttonTime;
unsigned long startTime = 0;
unsigned long sendStTime = 0;

int i,j,k;
int clientq = 0;
int pir = 0;
int led = 0;
int alarm = 0;
int pins[]={
  6,9,10};

byte txbuff[16];
byte rxbuff[16];

int servNum=0;
byte services[3] = { 
  B11001100, B00110011, B10011001 };

// Node strukturak deklaralasa  //
node server = {
  "serv1", B11111111, B00000010, B00001010, true};
node reg = {
  "dev01",B10101010, B00000001, B00001010, false};

node dev = {
  "dev01",B10101010, B00000001, B00001010, false};

node client[3];

// NodePin strukturak deklaralasa //

nodepin devPin;
nodepin clientPin[3];


void setup(){
  Serial.begin(9600);
  voidMirfInit();
  voidInit();
  pinMode(PIR, INPUT);
  pinMode(INTERRUPT, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT); 
  Mirf.setRADDR((byte *)"dev01");
  Mirf.setTADDR((byte *) "serv1");
}

void loop(){
  time=millis();
  sendTime=time/30000;
  
  voidPir();
  if (sendTime != prevTime){
    Mirf.setTADDR((byte *)"serv1");
    Mirf.setRADDR((byte *)"dev01");
    lifeSignalSend();
    
    timeOut = millis();
    while(!Mirf.dataReady() && time - timeOut < 2000){
    }
    voidGetPacket();
    alarmSent=true;
    prevTime=sendTime;
    Serial.println(sendTime, DEC);
    Serial.println(prevTime, DEC);
  }
}

/******************** Mirf inicializalasa *****************/

void voidMirfInit(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clie1");
  Mirf.payload = 32;
  Mirf.config();
}

/********************* Initalizing ***********************/

void voidInit(){
  for (int i=0;i<3;i++){
    devPin.pins[i]=pins[i];
  }
}

/************************ Using PIR **********************/

void voidPir(){

  if(digitalRead(PIR)==HIGH){
    pir=1;
    digitalWrite(LED, HIGH);
  }
  else{
    pir=0;
    digitalWrite(LED, LOW);
  }
  Serial.print("Alarm: ");
  Serial.println(alarmSent);
 
  if (alarmSent==true){
    if(pir==1){
      setAlarmBit();
    }
    else{
      clearAlarmBit();
    }
    alarmSent = false;
  }
} 

/******************** LifeSignal sending ********************/

void lifeSignalSend(){
 voidPacketGen();
 Mirf.send((byte *)&txbuff);
 while(Mirf.isSending()){
 }
 Serial.println("Lifesignal is sent...");
 }
 
/****************** Manipulating alarm bit *****************/

void setAlarmBit(){
  bitSet(dev.state, 0);
}

void clearAlarmBit(){
  bitClear(dev.state, 0); 
}

/******************** Generating packet ********************/

void voidPacketGen(){
  byte checkSum;
  for (int i=0; i<6; i++){
    txbuff[i] = dev.address[i];
  }
  Serial.print("0. bájt: ");
  Serial.println(txbuff[0], BIN);
  Serial.print("1. bájt: ");
  Serial.println(txbuff[1], BIN);
  Serial.print("2. bájt: ");
  Serial.println(txbuff[2], BIN);
  Serial.print("3. bájt: ");
  Serial.println(txbuff[3], BIN);
  Serial.print("4. bájt: ");
  Serial.println(txbuff[4], BIN);
  Serial.print("5. bájt: ");
  Serial.println(txbuff[5], BIN);

  txbuff[6] = services[2];
  Serial.print("6. bájt: ");
  Serial.println(txbuff[6], BIN);
  txbuff[7] = dev.id;
  Serial.print("7. bájt: ");
  Serial.println(txbuff[7], BIN);
  txbuff[8] = dev.state;
  Serial.print("8. bájt: ");
  Serial.println(txbuff[8], BIN);
  txbuff[9] = dev.role;
  Serial.print("9. bájt: ");
  Serial.println(txbuff[9], BIN);
  txbuff[10] = B00000000;
  txbuff[11] = B00000000;
  txbuff[12] = B00000000;
  txbuff[13] = B00000000;
  txbuff[14] = B00000000;
  for (int k=0; k<15; k++){
    checkSum += txbuff[k];
  }
  txbuff[15] = checkSum; 
  Serial.println("Packet is created");
}

/****************** Filling up server data *******************/

int voidGetPacket(){
  if (Mirf.dataReady() == true){
    Mirf.getData((byte *) &rxbuff);  
    for (int i=0; i<16; i++){
      Serial.print(i, DEC);
      Serial.print(" .-k bájt: ");
      Serial.println(rxbuff[i], BIN);
    }
  }
}




