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
#include "wireless.h"
#include "manageSQL.h"

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
    if (connectionFail == false){
             dataLogSQL();
             Serial.println("datalog: Done!");   
    } else
    if (connectionFail == true) Serial.println("There is no connection! Skip the queries! ");
    attachInterrupt(0, voidResponse, LOW);
    dataRec = false;
  }
  delay(37);
} 



