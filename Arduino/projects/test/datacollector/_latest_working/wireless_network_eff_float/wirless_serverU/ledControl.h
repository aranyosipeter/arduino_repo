// LED control methods

/*
2015.08.04 [General]: Created/add header file 
*/

void blinkYellowOnTime(){
 digitalWrite(YELLOWLED, LOW);
 delay(500);
 digitalWrite(YELLOWLED, HIGH);
}
