#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include <Ethernet.h>
#include <SPI.h>
#include <sha1.h>
//#include <avr/pgmspace.h>
#include <stdlib.h>
#include <mysql.h>
#include <EthernetClient.h>

#include "device_init.h"

/****************************************** Setup ************************************/
void setup(){
  pinMode(IRQ, INPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);
  digitalWrite(YELLOWLED, HIGH);
  mirfInit();
  attachInterrupt(0, voidResponse, LOW);
  //Serial.begin(9600);
  Ethernet.begin(mac_addr);
  commInit();
  Serial.println("Controller started!"); 
}

/******************************************* Loop ***********************************/
void loop(){
  // Flag of connection fail is set!
  if (/*( my_conn.is_available() == 0 ) ||*/ ( my_conn.is_connected() == 0 )){
    Serial.println("Problem with connection!");
    connectionFail = true;
    digitalWrite(YELLOWLED, HIGH);
    digitalWrite(GREENLED, LOW);
  }
  // Method of reconnecting
  if ( connectionFail == true ){
    Serial.println("Try to connect MySQL server!");
    my_conn.disconnect();
    Ethernet.begin(mac_addr);
    if (my_conn.mysql_connect(server_name, 3306, user, password)){
      connectionFail = false;
      Serial.println("Connection established!");
      digitalWrite(YELLOWLED, LOW);
      digitalWrite(GREENLED, HIGH);
    }
  } 
  if ( dataRec == true ){
    detachInterrupt(0);
    if ( timer == 0 && connectionFail == false){
             dataLogSQL();
             Serial.println("datalog: Done!");
             timer++;    
    } else
    if ( timer == 0 && connectionFail == true){
              Serial.println("There is no connection! Skip the queries! ");
              timer++;
    }
    if ( timer == 1 ){
              timer = 0; 
    }
    attachInterrupt(0, voidResponse, LOW);
    dataRec = false;
  }
  delay(37);
} 

/******************************* Function for sending packet ***********************************/
void voidPacketSend(){
    float txbuff[16];
    txbuff[0] = devID;
    txbuff[1] = command;
    txbuff[2] = 0;
    txbuff[3] = 0;
    txbuff[4] = 0;
    txbuff[5] = 0;
    txbuff[6] = 0;
    txbuff[7] = 0;
    txbuff[8] = 0;
    txbuff[9] = 0;
    txbuff[10] = 0;
    txbuff[11] = 0;
    txbuff[12] = 0;
    txbuff[13] = 0;
    txbuff[14] = 0;
    txbuff[15] = 0;
    Mirf.send((byte *)&txbuff);
    while(Mirf.isSending()){
    }
}

/*********************************** Filling up server data **********************************/
void voidGetPacket(){
  float rxbuff[15];
  if (Mirf.dataReady() == true){
    Mirf.getData((byte *) &rxbuff);
  }
  receiveID      = rxbuff[0];
  receiveCommand = rxbuff[1];
  dhtTemp        = rxbuff[2];
  dhtHum         = rxbuff[3];
  bmpPreshPa     = rxbuff[4];
  bmpTemp        = rxbuff[5];
  GPSAlt         = rxbuff[6];
  bmpSeaLev      = rxbuff[7];
  dataRec = true;
}

/****************************************** Response *******************************************/
void voidResponse(){
  voidGetPacket();
  voidPacketSend();
}

/*********************************** Data log to SQL database *********************************/
void dataLogSQL(){
    short id = 1;
    short id2 = 2;
    short id3 = 3;
    short id4 = 4;
    short id5 = 5;
    short id6 = 6;
    char query[64];
    sprintf(query, UPDATE, dhtTemp, id);
    my_conn.cmd_query(query);
    delay(250);
    sprintf(query, UPDATE, dhtHum, id2);
    my_conn.cmd_query(query);
    delay(250);
    sprintf(query, UPDATE, bmpTemp, id3);
    my_conn.cmd_query(query);
    delay(250);
    sprintf(query, UPDATE, bmpPreshPa, id4);
    my_conn.cmd_query(query);
    delay(250);
    sprintf(query, UPDATE, buttonValue, id5);
    my_conn.cmd_query(query);
    delay(250);
    sprintf(query, UPDATE, val, id6);
    my_conn.cmd_query(query);
    
    sprintf(query, SELECT_RELAY);
    my_conn.cmd_query(query);
    my_conn.show_results(results);
    Serial.println(results);
    Serial.println();
    Serial.println("UPDATED!!");
}
