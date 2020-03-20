#include <USBSabertooth.h>
#include <SoftwareSerial.h>
#include<Wire.h>

SoftwareSerial      SWSerial(NOT_A_PIN, 11);  // RX on no pin (unused), TX on pin 11 (to S1).
USBSabertoothSerial C(SWSerial);              // Use SWSerial as the serial port.
                      
USBSabertooth ST(C, 128); 

char c;


// sensor1 for Motor2
// sensor2 for Motor1

int sensor1Pin = A0;    // select the input pin for the potentiometer
int sensor1Value = 0;  // variable to store the value coming from the sensor
int PresSensor1Value = 0;
int sensor2Pin = A1;    // select the input pin for the potentiometer
int sensor2Value = 0;  // variable to store the value coming from the sensor
int PresSensor2Value = 0;
long nbreTics = 0 ;
long nbreTours = 0;  
int Vsensor1Min= 140;
int Vsensor1Max= 850;
int Vsensor2Min= 140;
int Vsensor2Max= 850;                 

int vitesse1=100;
int vitesse2=100;
int v1;
int v2;
double Kp1, Ki1, Kd1 ,Kp2, Ki2, Kd2 ;
double error1;
double error2;
double errorD1 = 0;
double errorI1 = 0;
double errorD2 = 0;
double errorI2 = 0;
double lasterror1 = 0;
double lasterror2 = 0;
int Tc = 5;
unsigned long t;

void setup()
{ Kp1=2.5;Ki1=0.0001;Kd1=0;
  Kp2=2.5;Ki2=0.0001;Kd2=0;
  
  Serial.begin(9600);
  SWSerial.begin(9600);
  delay(300);
  readSensoor();
  Serial.print("sensor1Value:         ");
  Serial.print(sensor1Value);
  Serial.print("       sensor2Value:         ");
  Serial.println(sensor2Value);
}


void loop()
{ // teta1 appartient à [30 , 750]
  // teta2 appartient à [200 , 1000]
  moove(500,500);
   if(Serial.available())
    { 
    c=Serial.read();
    Serial.println(c);
    if(c=='a') 
    {
    moove(650,500);  
    moove(300,500);
    moove(650,500);  
    moove(300,500);
    moove(500,650);  
    moove(500,300);
    moove(500,650);  
    moove(500,300);
    moove(500,500);
    moove(850,850);
    moove(500,850);
    moove(150,500);
    moove(150,150);
    moove(150,750);
    moove(500,500);
    moove(140,140);
    moove(850,850);
    moove(140,850);
    moove(850,140);
    moove(500,500);
   
    }
    else stopp();
    }

}


void MForward(int v1, int v2 ){ST.motor(1, v1);ST.motor(2, v2);}//2047
void MBackward(int v1 , int v2){ST.motor(1, -1*v1);ST.motor(2, -1*v2);}
void M1Forward(int v){ST.motor(1, v);}
void M1Backward(int v){ST.motor(1, -1*v);}
void M2Forward(int v){ST.motor(2,v);}
void M2Backward(int v){ST.motor(2,-1*v);}
void M1Stop(){ST.motor(1,0);}
void M2Stop(){ST.motor(2,0);}
void stopp(){ST.motor(1,0);ST.motor(2,0);}
void readSensoor(){ sensor1Value = analogRead(sensor1Pin);
                    sensor2Value = analogRead(sensor2Pin);
                    Serial.print("sensor1Value:         ");
                    Serial.print(sensor1Value);
                    Serial.print("       sensor2Value:         ");
                    Serial.println(sensor2Value);}
void testUsingSerial(){
    readSensoor();
    Serial.print("sensor1Value:         ");
    Serial.print(sensor1Value);
    Serial.print("       sensor2Value:         ");
    Serial.println(sensor2Value);
    
  
 if(Serial.available())
    { 
    c=Serial.read();
    Serial.println(c);
      switch(c)
      { case '1': v1=0;v2=0;break;
        case '2': v1=500;v2=500;break;
        case '3': v1=700;v2=700;break;
        case '4': v1=900;v2=900;break;
        case '5': v1=1200;v2=1200;break;
        case '6': v1=1500;v2=1500;break;
        case '7': v1=1700;v2=1700;break;
        case '8': v1=1900;v2=1900;break;
        case '9': v1=2000;v2=2000;break;
        case 'W': moove(120,120);break;
        case 'w': moove(120,120);break;
        case 'F': M1Forward(v1);break;
        case 'B': M1Backward(v1);break;
        case 'L': M2Forward(v2);break;
        case 'R': M2Backward(v2);break;
        case 'I': MForward(v1,v2);break;
        case 'H': MBackward(v1,v2);break;
        case 'S': stopp();break;
        default: stopp();
       }
       
    }
 //   else stopp();
  }

void PID1(int teta1)
{
  error1=abs(teta1-sensor1Value);
  errorD1 = error1 - lasterror1;  
  errorI1 += error1; 
  lasterror1 = error1;
  v1 = Kp1*error1 + (Kd1*errorD1)/Tc + Ki1*errorI1*Tc; 
  if (v1 > 2000) v1=2000; 
  Serial.print("v1= ");Serial.println(v1);
}

void PID2(int teta2)
{
  error2=abs(teta2-sensor2Value);
  errorD2 = error2 - lasterror2;  
  errorI2 += error2; 
  lasterror2 = error2;
  v2 = Kp2*error2 + (Kd2*errorD2)/Tc + Ki2*errorI2*Tc; 
  if (v2 > 2000) v2=2000;
  Serial.print("v2= ");Serial.println(v2);
}


  
void moove(int teta1 , int teta2)
{ v1=0; v2=0;
  bool AtPos1 = false;
  bool AtPos2 = false;
  if(teta1> Vsensor1Max) teta1 = Vsensor1Max;
  if(teta1< Vsensor1Min) teta1 = Vsensor1Min;
  if(teta2> Vsensor2Max) teta2 = Vsensor2Max;
  if(teta2< Vsensor2Min) teta2 = Vsensor2Min;
  readSensoor();
  lasterror1=0;  //abs(teta1-sensor1Value);
  lasterror2=0; //abs(teta2-sensor2Value);
  while( !AtPos1 || !AtPos2)
  { t= millis();
    readSensoor();
    if (!AtPos1){
      if (sensor1Value > teta1-50 && sensor1Value < teta1+50 ) {AtPos1=true; M2Stop;}
      else if(sensor1Value< teta1){PID1(teta1); M2Forward(v1);} 
      else {PID1(teta1);M2Backward(v1);}}
    if (!AtPos2){
      if (sensor2Value > teta2-50 && sensor2Value < teta2+50) {AtPos2=true; M1Stop;}
      else if(sensor2Value< teta2) {PID2(teta2); M1Forward(v2);}
      else {PID2(teta2);M1Backward(v2);}}
   while((millis()-t)< Tc); 
   }
  stopp();
}
