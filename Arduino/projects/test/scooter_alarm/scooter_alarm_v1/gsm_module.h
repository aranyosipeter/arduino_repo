CallGSM call;
SMSGSM  sms;

#define ARMNUMBER "+36305503879"

boolean gsmInit(){
  if (gsm.begin(9600)){
    Serial.println("\nstatus=READY");
    return true;
  }
  else {
    Serial.println("\nstatus=IDLE"); 
    return false;
  }
}

boolean callNumberByAlarm(){
  
}

void checkIncomingCallToArmOrDisarm(char number[], boolean armStateFlag){
  byte callState; 
  callState = call.CallStatusWithAuth(number, 2, 2);
  if (callState == CALL_INCOM_VOICE_AUTH) {
    Serial.println("Incoming call to Arm or Disarm!");
    armStateFlag =!armStateFlag;
  } 
}
