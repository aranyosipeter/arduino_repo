// Methods of serial communication

/*
2015.08.03. [General] Create header
*/

/*********************************************************************************************************/
/*                                    Write out received datas                                           */
/*********************************************************************************************************/
void printDataSerial(){
  Serial.print("ID: ");
  Serial.println(receiveID);
  
  Serial.print("Command: ");
  Serial.println(receiveCommand);
  
  Serial.print("Temp: ");
  Serial.println(dhtTemp);
  
  Serial.print("Humidity: ");
  Serial.println(dhtHum);
  
  Serial.print("Pressure: ");
  Serial.println(bmpPreshPa);
  
  Serial.print("BMPTemp: ");
  Serial.println(bmpTemp);
  
  Serial.print("Sea level pressure: ");
  Serial.println(bmpSeaLev);
}
