#ifdef NxtDisplay

  void NxtOpenSerial()
  {
   //This is for serial Nextion display.
   NXTSerial.begin(19200);
  
   pinMode(11,OUTPUT);
   pinMode(13,OUTPUT);
   digitalWrite(11,HIGH);
   digitalWrite(13,LOW);
  }

  void NextionMenu()
  {
    //Serial.println(EncOutValue);
    //Serial.println(NXTparam);
    switch(menuItem)
      {
        case 0:
            if (NXTparam==1) menuItem = 1;  // BPM
            if (NXTparam==2) menuItem = 3;  // Volume
            if (NXTparam==3) menuItem = 5;  // Pressure
            if (NXTparam==4) menuItem = 7;  // Peep
            if (NXTparam==5) menuItem = 9;  // Sync
            if (NXTparam==6) menuItem = 11; // Apnea
            if (NXTparam==7) menuItem = 13; // expirationRatio
            break;  
        case 1:
            InitNxtEncoder(ee.reqBPM,minBPM,maxBPM,stepBPM,4); // Init encoder
            menuItem = 2;
            break;
        case 2:   
            ee.reqBPM = EncOutValue; 
            if (NXTparam!=1) menuItem=0;
            break;  
 
        case 3:
            InitNxtEncoder(ee.reqVolume,minVolume,maxVolume,stepVolume,4); // Init encoder
            menuItem = 4;
            break;
        case 4:   
            ee.reqVolume = EncOutValue; 
            if (NXTparam!=2) menuItem=0;
            break;  

        case 5:
            InitNxtEncoder(ee.reqCompression,minCompression,maxCompression,stepCompression,4); // Init encoder
            menuItem = 6;
            break;
        case 6:   
            ee.reqCompression = EncOutValue; 
            if (NXTparam!=3) menuItem=0;
            break;  

        case 7:
            InitNxtEncoder(ee.peep ,minPEEPCompression,maxPEEPCompression,stepPEEPCompression,4); // Init encoder
            menuItem = 8;
            break;
        case 8:   
            ee.peep = EncOutValue; 
            if (NXTparam!=4) menuItem=0;
            break;  

        case 9:
            InitNxtEncoder((int)(ee.syncRatio*1),(int)(minsyncRatio*1),(int)(maxsyncRatio*1),(int)(stepsyncRatio*1),4); // Init encoder
            menuItem = 10;
            break;
        case 10:   
            ee.syncRatio = ((float)EncOutValue)/1; 
            if (NXTparam!=5) menuItem=0;
            break;  

        case 11:
            InitNxtEncoder(ee.apnea,minApnea,maxApnea,stepApnea,4); // Init encoder
            menuItem = 12;
            break;
        case 12:   
            ee.apnea = EncOutValue; 
            if (NXTparam!=6) menuItem=0;
            break;  

        case 13:
            InitNxtEncoder((int)(ee.expirationRatio*100),(int)(minExpirationRatio*100),(int)(maxExpirationRatio*100),(int)(stepExpirationRatio*100),4); // Init encoder
            menuItem = 14;
            break;
        case 14:   
            ee.expirationRatio = ((float)EncOutValue)/100; 
            if (NXTparam!=7) menuItem=0;
            break;  
      }

    ee.reqBPM          = constrain(ee.reqBPM, minBPM, maxBPM); 
    ee.reqVolume       = constrain(ee.reqVolume, minVolume, maxVolume); 
    ee.reqCompression  = constrain(ee.reqCompression, minCompression, maxCompression); 
    ee.peep            = constrain(ee.peep, minPEEPCompression, maxPEEPCompression); 
    ee.syncRatio       = constrain(ee.syncRatio, minsyncRatio, maxsyncRatio); 
    ee.apnea           = constrain(ee.apnea, minApnea, maxApnea); 
    ee.expirationRatio = constrain(ee.expirationRatio, minExpirationRatio, maxExpirationRatio); 
  }
  /*-----------------------------------------------------------*/
  void SendNxtData(String dev, String data)
  {
    sprintf(NxtMsg, "%s=%s%c%c%c", dev.c_str(), data.c_str(),0xff,0xff,0xff);
    NXTSerial.print(NxtMsg);
  }
  /*-----------------------------------------------------------*/
  void InitNxtData(String dev, String data)
  {
    memset(NxtMsg, 0, sizeof NxtMsg);  
    sprintf(NxtMsg, "%s=%s%c%c%c", dev.c_str(),data.c_str(),0xff,0xff,0xff);
  }
  /*-----------------------------------------------------------*/
  void Add_NxtData(String dev, String data)
  {
    sprintf(NxtMsg, "%s%s=%s%c%c%c", NxtMsg,dev.c_str(),data.c_str(),0xff,0xff,0xff);
  }
  /*-----------------------------------------------------------*/
  void SendNxtData()
  {
    NXTSerial.print(NxtMsg);
    //Serial.println(NxtMsg);
  }
  /*-----------------------------------------------------------*/
  int CheckNxtHex(byte b1,int &result)
  {
    int stat = 0;
    if (NxtBuf[0]==b1)
      {
        result = NxtBuf[1];
        stat = 1;
      }
    return(stat);
  }
  /*-----------------------------------------------------------*/
  void ClearNxtData()
  {
    memset(NxtData, 0, sizeof NxtData);
  }
  /*-----------------------------------------------------------*/
  void Nxt_Communicate()
  {
   Nxt_Poll(); 
   NextionMenu();
  }
  /*-----------------------------------------------------------*/
  void Nxt_Write()
  {
    ClearNxtData();

    sprintf(NxtMessage, "%u", ee.reqBPM);
    InitNxtData("PR1.val",NxtMessage);

    if (old_reqBPM!=ee.reqBPM) {
    sprintf(NxtMessage, "%u", (int)scale(ee.reqBPM,minBPM,maxBPM,0,100));
    Add_NxtData("PR1P.val",NxtMessage);
    old_reqBPM = ee.reqBPM;
    }      
  
    sprintf(NxtMessage, "%u", (int)scale(stepper.currentPosition(),0,maxVolume*motorVolumeRatio,0,100));
    Add_NxtData("dt1p.val",NxtMessage);      
    sprintf(NxtMessage, "%u", (int)scale(stepper.currentPosition(),0,volumePos*motorVolumeRatio,0,ee.reqVolume));
    Add_NxtData("dt1n.val",NxtMessage);      

    if (old_reqVolume!=ee.reqVolume) {
    sprintf(NxtMessage, "%u", ee.reqVolume);
    Add_NxtData("PR2.val",NxtMessage);
    sprintf(NxtMessage, "%u", (int)scale(ee.reqVolume,minVolume,maxVolume,0,100));
    Add_NxtData("PR2P.val",NxtMessage);     
    old_reqVolume = ee.reqVolume;
    } 

    if (old_reqCompression!=ee.reqCompression) {
    sprintf(NxtMessage, "%u", ee.reqCompression*10);
    Add_NxtData("PR3.val",NxtMessage);
    sprintf(NxtMessage, "%u", (int)scale(ee.reqCompression,minCompression,maxCompression,0,100));
    Add_NxtData("PR3P.val",NxtMessage);      
    old_reqCompression = ee.reqCompression;
    }

     if (old_peep!=ee.peep) {
    sprintf(NxtMessage, "%u", ee.peep);
    Add_NxtData("PR4.val",NxtMessage);
    sprintf(NxtMessage, "%u", (int)scale(ee.peep,minPEEPCompression,maxPEEPCompression,0,100));
    Add_NxtData("PR4P.val",NxtMessage);     
    old_peep = ee.peep;
    } 

     if (old_sync!=ee.syncRatio) {
    sprintf(NxtMessage, "%u", (int)(ee.syncRatio*10));
    Add_NxtData("MainPage.x2.val",NxtMessage);      
    sprintf(NxtMessage, "%u", (int)(ee.syncRatio*10));
    Add_NxtData("PR5.val",NxtMessage);
    sprintf(NxtMessage, "%u", (int)scale(ee.syncRatio,minsyncRatio,maxsyncRatio,0,100));
    Add_NxtData("PR5P.val",NxtMessage);     
    old_sync = ee.syncRatio;
    } 

     if (old_apnea!=ee.apnea) {
    sprintf(NxtMessage, "%u", ee.apnea);
    Add_NxtData("PR6.val",NxtMessage);
    sprintf(NxtMessage, "%u", (int)scale(ee.apnea,minApnea,maxApnea,0,100));
    Add_NxtData("PR6P.val",NxtMessage);     
    old_apnea = ee.apnea;
    } 

     if (old_expirationRatio!=ee.expirationRatio) {
    sprintf(NxtMessage, "%u", (int)(ee.expirationRatio*10));
    Add_NxtData("PR7.val",NxtMessage);
    sprintf(NxtMessage, "%u", (int)scale(ee.expirationRatio,minExpirationRatio,maxExpirationRatio,0,100));
    Add_NxtData("PR7P.val",NxtMessage);     
    old_expirationRatio = ee.expirationRatio;
    } 

   if (old_temp!=temperature) {
    sprintf(NxtMessage, "%d", (int)(temperature*10));
    Add_NxtData("MainPage.x0.val",NxtMessage);
    old_temp = temperature;
    }      

    if (old_hum!=humidity) {
    sprintf(NxtMessage, "%d", (int)(humidity*10));
    Add_NxtData("MainPage.x1.val",NxtMessage);      
    old_hum = humidity;
    }
    
    if (old_press!=relPressure) {
    sprintf(NxtMessage, "%d", (int)scale((int)relPressure,0,80,0,100));
    Add_NxtData("dt2p.val",NxtMessage);      
    sprintf(NxtMessage, "%d", (unsigned int)relPressure*10);
    Add_NxtData("dt2n.val",NxtMessage);      
    old_press = relPressure;
    }

    if (old_NxtBargraph!=NxtBargraph) {
    sprintf(NxtMessage, "%u", NxtBargraph);
    Add_NxtData("BAR.val",NxtMessage); 
    old_NxtBargraph = NxtBargraph;
    }     
    
    if (old_NxtCom!=NxtCom) {
    sprintf(NxtMessage, "%u", NxtCom);
    Add_NxtData("COM.val",NxtMessage);      
    old_NxtCom = NxtCom;
    }

    //if (old_ee_active!=ee.active) {
    sprintf(NxtMessage, "%u", ee.active);
    Add_NxtData("STRT.val",NxtMessage);      
    //old_ee_active = ee.active;
    //}

    if (old_ee_CVmode!=ee.CVmode) {
    sprintf(NxtMessage, "%u", ee.CVmode);
    Add_NxtData("VP.val",NxtMessage);      
    old_ee_CVmode = ee.CVmode;
    }

    if (old_SensorType!=SensorType) {
    sprintf(NxtMessage, "%u", SensorType);
    Add_NxtData("MainPage.SENS.val",NxtMessage); 
    }     

    SendNxtData();

    if(NxtPage!=old_NxtPage)
    {
       old_peep  = -1;
       old_sync  = -1;
       old_apnea = -1;
       old_expirationRatio = -1;
       old_NxtPage = NxtPage;
    }
  
 }
 /*-----------------------------------------------------------*/
 float scale(float A, float Amin, float Amax, float Min, float Max) 
  {
   return ((Max - Min) / (Amax - Amin)) * (A-Amin);
  }
 /*-----------------------------------------------------------*/
 void Nxt_Poll()
  {
    // READ/WRITE DATA FROM PANEL
    digitalWrite(13,HIGH);
    delayMicroseconds(10); // 10 usec delay 
    if (NXTSerial.available())
     {
         byte inc;
        inc = (char)NXTSerial.read();
        if ((inc >= 0xF0 && inc <= 0xFE) && !NxtSetStart) // <START MESSAGE>
          {
            NxtBufCnt = 0;
            NxtSetStart=true;
          }
        if(NxtSetStart)
          {
            NxtBuf[NxtBufCnt++] = inc;
            if (inc == 0x0A) // <END MESSAGE>
              {
                NxtSetStart=false;  
                //sprintf(NxtMsg,"Nextion TX [%02X,%02X,%02X]",NxtBuf[0],NxtBuf[1],NxtBuf[2]);
                //Serial.println(NxtMsg);

                CheckNxtHex(0xF0,NXTparam);                                                           // (F0) Parameter selection and settings 0..7  
                CheckNxtHex(0xF1,NXTstart);
                if (NXTstart==1 && !NXTstartFlag)         {ee.active=!ee.active;NXTstartFlag=true;}   // (F1) Toggle button (start/stop)
                if (NXTstart==0)                          {NXTstartFlag=false;} 
                CheckNxtHex(0xF2,NXTsave);
                if (NXTsave==1)                           {saveNxtdata();}                            // (F2) Save actual data to eeprom
                CheckNxtHex(0xF3,NXTvp);
                if (NXTvp==1 && !NXTvpFlag)               {ee.CVmode=!ee.CVmode;NXTvpFlag=true;}      // (F3) Toggle button control mode change (Volume/Pressure)
                if (NXTvp==0)                             {NXTvpFlag=false;}    
                CheckNxtHex(0xF4,NxtPage);                                                            // (F4) Active Page panel 1 of 2      
                
                NxtBufCnt = 0;

                if (NXTstart==1)
                 {
                  sprintf(NxtMsg,"DP start/stop %d",ee.active);
                  Serial.println(NxtMsg);
                 }
             }
          }      
      }
    else
      {
        Nxt_Write();
      }
    digitalWrite(13,LOW);
  }
 /*-----------------------------------------------------------*/
 void saveNxtdata()
 { 
  #ifdef E2PROM            
    eeput(1);  // Save settings to EEPROM
 #endif           
 }
 /*-----------------------------------------------------------*/
 void setNxtEncoder() 
 {
  PRR1 = PRR1 & B11111011;
  DDRJ = PINJ & B11111100;
  PCICR |= B00000010;     //Enable port B Registers i.e D8-D13
  PCMSK1 |= bit(EncB);    // Pin15
  PCMSK1 |= bit(EncA);    // Pin14
  sei(); //enable all interrupts
 }
 void InitNxtEncoder(int Value,int Min,int Max,int Step,int Filter)
 {
  EncOutValue = Value;
  CntStep = Step;
  CntFilter = Filter;
  CntMin = Min;
  CntMax = Max;
 }

 ISR(PCINT1_vect) 
 {
  MSB = PINJ & B00000001;
  LSB = PINJ & B00000010;
  encoded = MSB | LSB;
  sum  = (lastEncoded << 2) | encoded;

  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    EncValue ++;
    if (EncValue >= CntFilter) {
         if (EncOutValue < CntMax) {
           EncOutValue = EncOutValue + CntStep;
           //Serial.println(EncOutValue);
         }
         EncValue = 0;
        }
  }
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011){
    EncValue --;
    if (EncValue <= CntFilter*-1) {
         if (EncOutValue > CntMin) {
           EncOutValue = EncOutValue - CntStep;
           //Serial.println(EncOutValue);
         }
         EncValue = 0;
        }
  }
  lastEncoded = encoded;
  //Serial.println(EncOutValue);
 }
#endif
