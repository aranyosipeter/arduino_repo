// nRF24 module methods

/*
2015.07.24 [General]: Created/add header file => copy existed methods
2015.07.27 [Modification]: Delete all mods in connection with GPS module
*/

/******************************* Function for sending packet ***********************************/
void voidPacketSend(byte mode){
    float txbuff[7];
    if (mode == 0) {
      txbuff[0] = devID;
      txbuff[1] = command;
      txbuff[2] = 0;
      txbuff[3] = 0;
      txbuff[4] = 0;
      txbuff[5] = 0;
      txbuff[6] = 0;
      txbuff[7] = 0;
    }
    else if (mode == 1) {
      // Relek kiiratasa
      txbuff[0] = devID;
      txbuff[1] = command;
      txbuff[2] = 0;
      txbuff[3] = 0;
      txbuff[4] = 0;
      txbuff[5] = 0;
      txbuff[6] = 0;
      txbuff[7] = 0;
    }
    Mirf.send((byte *)&txbuff);
    while(Mirf.isSending()){
    }
}

/*********************************** Filling up server data **********************************/
void voidGetPacket(){
  float rxbuff[7];
  if (Mirf.dataReady() == true){
    Mirf.getData((byte *) &rxbuff);
  }
  receiveID      = rxbuff[0];
  receiveCommand = rxbuff[1];
  dhtTemp        = rxbuff[2];
  dhtHum         = rxbuff[3];
  bmpPreshPa     = rxbuff[4];
  bmpTemp        = rxbuff[5];
  bmpSeaLev      = rxbuff[6];
  dataRec = true;
}

/****************************************** Response *******************************************/
void voidResponse(){
  voidGetPacket();
  voidPacketSend(1);
}


