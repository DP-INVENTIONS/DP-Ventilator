  #define EncBtn  23
  #define EncA    PCINT10
  #define EncB    PCINT9

  #define NXTSerial Serial2          // Serial port to connect with Nextion panel
  int   NxtCom = 0;
  char  NxtData[254];
  char  NxtMsg[254];
  char  NxtMessage[100];
  String NxtStr = "";
  bool  firststart = true;
  int   NxtBargraph;

  int   EncValue; 
  int   EncOutValue;
  int   lastEncoded;
  volatile int MSB;
  volatile int LSB;
  volatile int encoded;
  volatile int sum;
  int   CntStep = 0;
  int   CntFilter = 0;
  int   CntMin = 0;
  int   CntMax = 0;
  float old_temp=-1;
  float old_hum=-1;
  float old_press=-1;
  int   old_NxtBargraph=0;
  int   old_NxtCom=0;
  bool  old_ee_active=0;
  bool  old_ee_CVmode=0;
  int   old_reqBPM=-1;
  int   old_reqVolume=-1;
  int   old_reqCompression=-1;
  int   old_peep=-1;
  float old_sync=-1;
  int   old_apnea=-1;
  int   old_expirationRatio=-1;
  int   old_NxtPage=-1;
  int   motorOffset=0;
  bool  MoveUp=false;
  bool  MoveDown=false;
  bool  Init=false;
  bool  MotorInit=false;
 
  int   NXTparam=0;
  byte  NxtBuf[255];
  int   NxtBufCnt=0; 
  bool  NxtSetStart=false;

  int   NXTstart=0;
  bool  NXTstartFlag=false;
  int   NXTsave=0;
  int   NXTvp=0;
  bool  NXTvpFlag=false;
  int   NxtPage=0;
  int   Nxtmove=0;
  int   Nxtzero=0;

  int   old_MotorZero=-1;
  int   MotorZero=0;
  int   old_MotorUp=-1;
  int   MotorUp=0;
  int   old_MotorDown=-1;
  int   MotorDown=0;
  int   SensorType=0;
  int   old_SensorType=-1;
  int   SType=0;
