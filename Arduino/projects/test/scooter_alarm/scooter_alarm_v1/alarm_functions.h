#define LED                       6
#define IRQ                       2
#define PIEZZO                    3

#define ALARMTIME                 60000

// I/O init
void IOInit(){
  pinMode(IRQ, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(PIEZZO, OUTPUT);
}

void savePosition(){
  
}

boolean checkPosition(){
}


void drivePiezzo(){
  mills = millis()
  //if (alarmFlag && )
}
