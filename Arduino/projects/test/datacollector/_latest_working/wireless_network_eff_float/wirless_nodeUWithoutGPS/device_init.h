// Initialize device connections/settings

/*
2015.07.27 [Modification]: Delete all code in connection with GPS module
*/

// LCD display connection
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// RTC settings
RTC_DS1307 RTC;

// BMP085
Adafruit_BMP085 bmp;

/******************* Initalizing BMP085 **********************/
boolean bmpInit(){
    if (!bmp.begin()) return false;
    else return true;
}

/****************** Initalizing LCD screen ********************/
boolean lcdInit(){
  lcd.begin (16,2);  
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(LED_ON);
  return true;
}

/******************** Initalizing RTC ************************/
boolean rtcInit(){
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  return true;
}

/******************* Initalizing MiRF ************************/
boolean mirfInit(){
  Mirf.payload = 32;
  Mirf.channel = 82;
  Mirf.csnPin = 14;
  Mirf.cePin = 15;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"nodeU");
  Mirf.setTADDR((byte *)"servU");
  Mirf.config();
  return true;
}




