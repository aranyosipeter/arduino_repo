// Initialize device connections/settings

/*

*/

#include "declarations.h"

//Connector my_conn;        // The Connector/Arduino reference

/************************************* Communication Init *************************************
void commInit(){
  Serial.println("Connecting...");
  if (my_conn.mysql_connect(server_name, 3306, user, password)) {
    delay(1000);
    digitalWrite(YELLOWLED, LOW);
    digitalWrite(GREENLED, HIGH);
  }
  else
    Serial.println("Connection failed.");
}

/*************************************** Mirf init *********************************************/
void mirfInit(){
  Mirf.payload = 32;
  Mirf.channel = 82;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"servU");
  Mirf.setTADDR((byte *)"nodeU"); 
  Mirf.config();  
}




