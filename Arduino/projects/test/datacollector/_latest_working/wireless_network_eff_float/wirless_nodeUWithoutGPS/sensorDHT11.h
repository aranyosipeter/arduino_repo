//DHT11 Sensor methods

/*
2015.08.06 [General]: Created/add header file => copy existed functions
*/


/***************** Fahrenheit ***********************/
float convertFahrenheit(int celsius){
  return 1.8 * celsius + 32;
}

/****************** DHT value ***********************/
void readDHTValues(){
  dhtTemp = DHT11.temperature;
  dhtHum = DHT11.humidity;
  dhtFah = convertFahrenheit(dhtTemp);
  
  if (serialDataFlag){
    Serial.print("Humidity (%): ");
    Serial.println(dhtHum, DEC);
 
    Serial.print("Temperature (oC): ");
    Serial.println(dhtTemp, DEC);
 
    Serial.print("Temperature (oF): ");
    Serial.println(dhtFah, 2);
    Serial.println("\n");
  }
}



