//VARS---------------------------------------------------------------------------------------------------------------------
unsigned long timer;
unsigned long timersec;
unsigned long timerdebouce;
boolean halfsec=false;

unsigned int Hora=0;
unsigned int Minuto=0;
unsigned int Segundo=0;

unsigned int Test=0;

boolean leds[60];
unsigned short int l=0;

boolean transistors[4];
unsigned short int selectTransistor=0;

byte BCD[16][4] ={{0,0,0,0},
{1,0,0,0},
{0,1,0,0},
{1,1,0,0},
{0,0,1,0},
{1,0,1,0},
{0,1,1,0},
{1,1,1,0},
{0,0,0,1},
{1,0,0,1},
{0,1,0,1},
{1,1,0,1},
{0,0,1,1},
{1,0,1,1},
{0,1,1,1},
{1,1,1,1}};

boolean trigger[4]={0,0,0,0};
//PINOUT-------------------------------------------------------------------------------------------------------------------
short int bcdPins[4] = {6,5,4,3}; // A,B,C,D inputs
short int ledPin=2;

short int dataPin=7;
short int latchPin=8;
short int clockPin=9;

short int btn_hp=10;
short int btn_hm=11;
short int btn_mp=12;
short int btn_mm=13;
//-------------------------------------------------------------------------------------------------------------------------

//SETUP--------------------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(bcdPins[0],OUTPUT);
  pinMode(bcdPins[1],OUTPUT);
  pinMode(bcdPins[2],OUTPUT);
  pinMode(bcdPins[3],OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(ledPin,OUTPUT);
  
  pinMode(btn_hp,INPUT);
  pinMode(btn_hm,INPUT);
  pinMode(btn_mp,INPUT);
  pinMode(btn_mm,INPUT);
  digitalWrite(btn_hp,HIGH);
  digitalWrite(btn_hm,HIGH);
  digitalWrite(btn_mp,HIGH);
  digitalWrite(btn_mm,HIGH);
  
  timer=millis();
  timersec=millis();
  timerdebouce=millis();
  
  digitalWrite(latchPin, HIGH);delay(500);
}
//-------------------------------------------------------------------------------------------------------------------------

//LOOP---------------------------------------------------------------------------------------------------------------------
void loop() {
  timercount();
  relogio();
  seccount(); 
  printShift(); 
  printBCD();
  btndebouce();
  btncheck(); //erro no botao mm desabilitado em codigo  
delay(100);
}
//-------------------------------------------------------------------------------------------------------------------------

//TIMER--------------------------------------------------------------------------------------------------------------------
void timercount(){
  if(timer<=millis()-500){
    timer=millis();
    halfsec=!halfsec;
    if(halfsec){
    digitalWrite(ledPin,HIGH);
    Segundo++;l=0;
    }else
    digitalWrite(ledPin,LOW);
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//RELOGIO------------------------------------------------------------------------------------------------------------------
void relogio(){
  if(Segundo>=60){
    Segundo=0;
    Minuto++;
  }
  if(Minuto>=60){
    Minuto=0;
    Hora++;
  }
  if(Hora>=24){
    Hora=0;
  }
}
//-------------------------------------------------------------------------------------------------------------------------

//SECCOUNT-----------------------------------------------------------------------------------------------------------------
 void seccount(){
   if(Segundo==0||Segundo>=60)
     for(int i=0; i<60; i++)
       leds[i]=false;
  if(timersec<=millis()-(1000/(60-Segundo))){
      timersec=millis();                                 
      l++; 
      if(60-l<60)  
      leds[60-l]=true;
      if(61-l<60)      
      leds[61-l]=false;
  }
  for(int i=0; i<Segundo; i++)
       leds[i]=true;
   
}
//-------------------------------------------------------------------------------------------------------------------------

//PRINT--------------------------------------------------------------------------------------------------------------------
void printShift(){  
    digitalWrite(latchPin, LOW); 
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[52],leds[53],leds[54],leds[55],leds[56],leds[57],leds[58],leds[59]));
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[44],leds[45],leds[46],leds[47],leds[48],leds[49],leds[50],leds[51]));
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[36],leds[37],leds[38],leds[39],leds[40],leds[41],leds[42],leds[43]));
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[28],leds[29],leds[30],leds[31],leds[32],leds[33],leds[34],leds[35]));
      
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[20],leds[21],leds[22],leds[23],leds[24],leds[25],leds[26],leds[27]));
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[12],leds[13],leds[14],leds[15],leds[16],leds[17],leds[18],leds[19]));
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(1,leds[4],leds[5],leds[6],leds[7],leds[8],leds[9],leds[10],leds[11]));
    shiftOut(dataPin, clockPin,LSBFIRST, shiftBits(0,leds[3],leds[2],leds[4],leds[0],transistors[0],transistors[3],transistors[1],transistors[2]));
    digitalWrite(latchPin, HIGH); 
}
//-------------------------------------------------------------------------------------------------------------------------

//PRINTBCD-----------------------------------------------------------------------------------------------------------------
void printBCD(){
  for(int i=0; i<4; i++)
       transistors[i]=false;
  switch(selectTransistor){
    default:
      transistors[0]=true;
      for(int c = 0; c < 4; c++){
        digitalWrite(bcdPins[c], BCD[(int)Hora/10][c]);
      }
    break;
    
    case 1:
      transistors[1]=true;
      for(int c = 0; c < 4; c++){
        digitalWrite(bcdPins[c], BCD[(int)Hora%10][c]);
      }
    break;
    
    case 2:
      transistors[2]=true;
      for(int c = 0; c < 4; c++){
        digitalWrite(bcdPins[c], BCD[(int)Minuto/10][c]);
      }
    break;
    
    case 3:
      transistors[3]=true;
      for(int c = 0; c < 4; c++){
        digitalWrite(bcdPins[c], BCD[(int)Minuto%10][c]);
      }
    break;
  }
  
  selectTransistor++;
  if(selectTransistor>=4)
    selectTransistor=0;
}
//-------------------------------------------------------------------------------------------------------------------------

//CHECKBTN-----------------------------------------------------------------------------------------------------------------
void btndebouce(){
  if(timerdebouce<=millis()-250){
    timerdebouce=millis();
    if(trigger[0])
      if(digitalRead(btn_hp)==LOW)
        if(Hora>=23)
          Hora=0;
        else
          Hora++;
    if(trigger[1])
      if(digitalRead(btn_hm)==LOW)
        if(Hora<=0)
          Hora=23;
        else
          Hora--;
    if(trigger[2])
      if(digitalRead(btn_mp)==LOW)
        if(Minuto>=59)
            Minuto=0;
          else
            Minuto++;
    if(trigger[3])
      if(digitalRead(btn_mm)==LOW)
        if(Minuto<=0)
          Minuto=59;
        else
          Minuto--;
    for(int i=0; i<4; i++)
       trigger[i]=false;
  }
}

void btncheck(){
  if(digitalRead(btn_hp)==LOW)
    trigger[0]=true;
  if(digitalRead(btn_hm)==LOW)
    trigger[1]=true;
  if(digitalRead(btn_mp)==LOW)
    trigger[2]=true;
  //if(digitalRead(btn_mm)==LOW)
    //trigger[3]=true;
}
 
//-------------------------------------------------------------------------------------------------------------------------
int shiftBits(boolean type,boolean a,boolean b,boolean c,boolean d,boolean e,boolean f,boolean g,boolean h){
int value=0;
if(!type)
value=h*1+g*2+f*4+e*8+d*16+c*32+b*64+a*128;
else
value=a*1+b*2+c*4+d*8+e*16+f*32+g*64+h*128;
return value;
}

    
    
    
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

