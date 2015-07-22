#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Adafruit_BMP085.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// This headers for SPI connection to the wireless connction with NRF24L01
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include "device_init.h"
#include "wireless.h"
#include "gps.h"


/*
2015.06.19 [General]: modultesztek atalakitasa
2015.06.23 [General]: device_init.h letrehozasa, fuggveny atnevezes/atertelmezes
2015.07.20 [General]: declarations.h letrehozasa
2015.07.22 [Modul]: nRF modul fuggvenyeinek atalakitasa => wireless.h
*/

/************************** Setup *******************************/
void setup()  {
  pinMode(IRQ, INPUT);
  attachInterrupt(0, sendResponse, LOW);
  Wire.begin();
  Serial.begin(9600);
  
  if (lcdInit())  displayInit = true;
  if (rtcInit())  realTimeClockInit = true;
  if (mirfInit()) wirelessModInit = true; 
  if (bmpInit())  sensorInit = true;
  if (GPSInit())  gpsInit = true;
  
  if (!sensorInit) systemInit = false;
  
  if (systemInit){
    if (displayInit){
      lcd.setCursor(6,1);
      lcd.print(F("System failure!"));
    }
    while(1);
  }
}

/************************** Loop *******************************/
void loop(){
  printRealTime();
  getGPSDateTime();
  voidLCDData();
  //processPacket();
  getSensorData();
  voidSerialData();
  if (commComp) {
    for ( k = 0; k < s; k++ ){
    Serial.print(command[k]);  
    }
    Serial.println("");
    commandProc();
    commComp = false;
    s = 0;
  }
}

/********************* BMP085 Value **************************/
void getSensorData(){  
  bmpTemp = bmp.readTemperature();
  bmpPres = bmp.readPressure();
  bmpPreshPa = (float)bmpPres / 100;
  bmpRealAlt = bmp.readAltitude(102425);
  if (GPSAlt > -1) bmpSeaLev = ((float)bmp.readSealevelPressure(GPSAlt)) / 100;
  else bmpSeaLev = ((float)bmp.readSealevelPressure(bmpRealAlt)) / 100;
  
  Serial.print("Temperature = ");
  Serial.print((float)bmpTemp, 2);
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bmpPres, DEC);
  Serial.println(" Pa");
  Serial.print("Sea level = ");
  Serial.print((float)bmpSeaLev, 2);
  Serial.println(" hPa");
  Serial.print("Real altitude = ");
  Serial.print((float)bmpRealAlt, 2);
  Serial.println(" meters");
  Serial.println("\n"); 
}

/********************* Actual time *********************/
void printRealTime(){
  now = RTC.now();

  lcd.setCursor(0,0);
  lcd.print(now.year(), DEC);

  lcd.setCursor(4,0);
  lcd.print("/");

  if ( now.month() < 10 ){
    lcd.setCursor(5,0);
    lcd.print(0, DEC);
    lcd.setCursor(6,0);
    lcd.print(now.month(), DEC);
  }
  else {
    lcd.setCursor(5,0);
    lcd.print(now.month(), DEC);
  }

  lcd.setCursor(7,0);
  lcd.print("/");

  if ( now.day() < 10 ){
    lcd.setCursor(8,0);
    lcd.print(0, DEC);
    lcd.setCursor(9,0);
    lcd.print(now.day(), DEC);
  }
  else {
    lcd.setCursor(8,0);
    lcd.print(now.day(), DEC);
  }

  if ( now.hour() < 10 ){
    lcd.setCursor(11,0);
    lcd.print(0, DEC);
    lcd.setCursor(12,0);
    lcd.print(now.hour(), DEC);
  }
  else {
    lcd.setCursor(11,0);
    lcd.print(now.hour(), DEC);
  }


  if (( now.second() % 2 ) == 0 ){
    lcd.setCursor(13,0);
    lcd.print(":");
  }
  else{
    lcd.setCursor(13,0);
    lcd.print(" ");
  }

  if ( now.minute() < 10 ){
    lcd.setCursor(14,0);
    lcd.print(0, DEC);
    lcd.setCursor(15,0);
    lcd.print(now.minute(), DEC);
  }
  else {
    lcd.setCursor(14,0);
    lcd.print(now.minute(), DEC);
  }
} 

/******************************** Datas on LCD ***************************/
void voidLCDData(){
  if ((millis() % DataScreenChange == 0) && NotInASec){
       cycleVar++;
       clearScreen();
       NotInASec = false;
       }
  if ((millis() % DataScreenChange != 0) && !NotInASec){
       NotInASec = true;
       }
       
  switch (cycleVar){
    case 0: 
      lcd.setCursor(3,1);
      lcd.print("H: ");
      lcd.setCursor(6,1);
      lcd.print(bmpTemp,1);
      lcd.setCursor(11,1);
      lcd.print("C");
      break;
    case 1: 
      lcd.setCursor(2,1);
      lcd.print("Ny:");
      lcd.setCursor(6,1);
      lcd.print(bmpPreshPa, 1);
      lcd.setCursor(12,1);
      lcd.print("hPa");  
      break;
    case 2: 
      lcd.setCursor(2,1);
      lcd.print("S: ");
      lcd.setCursor(5,1);
      lcd.print(bmpSeaLev, 1);
      lcd.setCursor(11,1);
      lcd.print(" hPa");
      break;
    case 3: 
      lcd.setCursor(3,1);
      lcd.print("A: ");
      lcd.setCursor(6,1);
      lcd.print(bmpRealAlt, 1);
      lcd.setCursor(11,1);
      lcd.print(" m");
      break;
    case 4:
      lcd.setCursor(1,1);
      lcd.print("GPS: ");
      lcd.setCursor(6,1);
      lcd.print(GPSAlt, 1);
      lcd.setCursor(13,1);
      lcd.print("m");
      break;
    default:
      cycleVar = 0;
  }    
}

/*************************** SerialEvent *********************/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char commandChar = (char)Serial.read(); 
    // add it to the inputString:
    command[s] = commandChar;
    s++;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (commandChar == '\n') {
      commComp = true;
    } 
  }
}

/************************ Commands *************************/
void commandProc(){
  //Serial.println("teszteles jelleggel/ sajnos csak igy mukodik");
  //Serial.println(&test[0]);
  //Serial.println("dataChar[]");
  //Serial.println(&dataChar[0]);
  //Serial.println("relayChar[]");
  //Serial.println(&relayChar[0]);
  if ( (command[0] == dataChar[0]) && (command[1] == dataChar[1]) && (command[2] == dataChar[2]) && (command[3] == dataChar[3])) {
    dataFlag = true;
    Serial.println("data parancs fogadva"); 
  }
  else {
    dataFlag = false;
    Serial.println("nem data parancs"); 
  } 
  if (( dataFlag == true ) && ( serialDataFlag == false )) serialDataFlag = true;
  if (( dataFlag == false ) && ( serialDataFlag == true )) serialDataFlag = false; 
  if ( command[0] == relayChar[0] && command[1] == relayChar[1] && command[2] == relayChar[2] && command[3] == relayChar[3] && command[4] == relayChar[4] ) {
    relayFlag = true;
    Serial.println("relay parancs fogadva");
  }
  else {
    relayFlag = false;
    Serial.println("nem relay parancs");
  }
  if ( relayFlag == true ){
    relayComPin = atoi(&command[6]) - 1;
    Serial.print("Vezerelt rele szama: ");
    Serial.println(relayComPin, DEC);
    if ( command[10] == trueChar[0]  && command[11] == trueChar[1] && command[12] == trueChar[2] && command[13] == trueChar[3] ){
      Serial.print("Allapota: ");
      Serial.println("Bekapcsolva"); 
      bitSet(relayBuff, relayComPin);
      Serial.println(relayBuff, BIN);
    }
    if ( command[10] == falseChar[0]  && command[11] == falseChar[1] && command[12] == falseChar[2] && command[13] == falseChar[3] && command[14] == falseChar[4] ){
      Serial.print("Allapota: "); 
      Serial.println("Kikapcsolva"); 
      bitClear(relayBuff, relayComPin);
      Serial.println(relayBuff, BIN);
    }
  }
}

/******************************************- Serial data ************************************/
void voidSerialData(){
  if ( serialDataFlag == true && dataRec == true){
    Serial.print("Fenyero: ");
    Serial.print(light, DEC);
    Serial.println(" %");
    Serial.print("Paratartalom: ");
    Serial.print(dhtHum, DEC);
    Serial.println(" %");
    Serial.print("Homerseklet: ");
    Serial.print(dhtTemp, DEC);
    Serial.println(" C");
    Serial.print("Legnyomas: ");
    Serial.print(bmpPreshPa, DEC);
    Serial.println(" kPa");
    dataRec = false;  
  }
  
}
/************************ Clear the screen ************************/
void clearScreen(){
   lcd.setCursor(0,1);
   lcd.print("                ");
}

/********************* Adjust time and date ************************/
/*void adjTimeAndDate(){
  if (adjFlag) {
    if (
  }
}*/



