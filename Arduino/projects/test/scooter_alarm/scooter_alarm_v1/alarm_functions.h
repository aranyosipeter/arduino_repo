#define LED                       6
#define IRQ                       2
#define PIEZZO                    3

#define ALARMTIME                 60000
#define ARMINGTIME                30000

// I/O init
void IOInit(){
  pinMode(IRQ, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(PIEZZO, OUTPUT);
}

void saveGyroPosition(){
  if (armState){
    if (!armTimeSaved) armStartedTime = glbTimer; // nagyonkerdeses...
    if ((armStartedTime + ARMINGTIME) < glbTimer){
      armedGyroStateX = 
      systemArmed = true;
    }
  }
}

boolean checkGyroPosition(){
}


void drivePiezzo(){
  mills = millis()
  //if (alarmState && )
}

void resetAlarmFlags(){
  if (!systemArmed) {
    alarmState = false;
    sirenState = false;
    armTimeSaved = false;
  }
}
