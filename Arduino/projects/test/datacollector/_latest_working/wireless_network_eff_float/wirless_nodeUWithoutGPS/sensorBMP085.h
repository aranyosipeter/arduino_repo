//BMP085 Sensor functions

/*
2015.07.24 [General]: Created/add header file => copy existed functions
2015.07.27 [Modification]: Delete all code in connection with GPS module
*/

/**************************************************************************/
/*                      Reading values from sensor                        */
/**************************************************************************/
void getSensorData(){  
  bmpTemp = bmp.readTemperature();
  bmpPres = bmp.readPressure();
  bmpPreshPa = (float)bmpPres / 100;
  bmpRealAlt = bmp.readAltitude(102425);
  bmpSeaLev = ((float)bmp.readSealevelPressure(124)) / 100;
  
  if (debugFlag) {
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
}
