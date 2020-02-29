// Simplest Sample for USB Sabertooth Packet Serial
// Copyright (c) 2012-2013 Dimension Engineering LLC
// See license.txt for license details.

#include <USBSabertooth.h>
#include <SoftwareSerial.h>
#include<Wire.h>

SoftwareSerial      SWSerial(NOT_A_PIN, 11);  // RX on no pin (unused), TX on pin 11 (to S1).
USBSabertoothSerial C(SWSerial);              // Use SWSerial as the serial port.
                      
USBSabertooth ST(C, 128); 
//#define Addr 0x68

const int MPU_addr=0x68;
int16_t axis_X,axis_Y,axis_Z;
int minVal=-360;//265
int maxVal=360;//402
double x;
double y;
double z;   
char c;
int vmax = 2047;
                                     
void setup()
{ Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);//6B
  Wire.write(0);//0
  Wire.endTransmission(true);
  Serial.begin(9600);
  SWSerial.begin(9600);
  delay(300);
  
}

//double Targx=50;

void loop()
{
  
 if(Serial.available())
    { 
    c=Serial.read();
    Serial.println(c);
      switch(c)
      { 
        case 'A': M1Forward(300);/*break;//*/delay(20);stopp();break;
        case 'Z': M1Backward(300);/*break;//*/delay(20);stopp();break;
        case 'Q': M2Forward(300);/*break;//*/delay(20);stopp();break;
        case 'S': M2Backward(300);/*break;//*/delay(20);stopp();break;
        case 'W': MForward(300);/*break;*/delay(20);stopp();break;
        case 'X': MBackward(300);/*break;*/delay(20);stopp();break;
        default: stopp();delay(20);
       }
       
    }
    else stopp();
}


void MForward(int v ){ST.motor(1, v);ST.motor(2, v);}//2047
void MBackward(int v){ST.motor(1, -1*v);ST.motor(2, -1*v);}
void M1Forward(int v){ST.motor(1, v);ST.motor(2, 0);}
void M1Backward(int v){ST.motor(1, -1*v);ST.motor(2, 0);}
void M2Forward(int v){ST.motor(1,0);ST.motor(2,v);}
void M2Backward(int v){ST.motor(1,0);ST.motor(2,-1*v);}
void M1Stop(){ST.motor(1,0);}
void M2Stop(){ST.motor(2,0);}
void stopp(){ST.motor(1,0);ST.motor(2,0);}

/*void CalculAngle()
{

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true);//14
  axis_X=Wire.read()<<8|Wire.read();
  axis_Y=Wire.read()<<8|Wire.read();
  axis_Z=Wire.read()<<8|Wire.read();
  Wire.endTransmission(true);//*********  
    int xAng = map(axis_X,minVal,maxVal,-90,90);
    int yAng = map(axis_Y,minVal,maxVal,-90,90);
    int zAng = map(axis_Z,minVal,maxVal,-90,90);
       x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
       y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
       z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
   if(x<=360&&x>=90) x=x-360;
   if(y<=360&&y>=90) y=y-360;
   x=x+90; y= y+90;
 }*/
/* void goToX(double Targx)
 {
  
  while(x-Targx>0)
  { M1Backward();
  CalculAngle();
  }
  
  while(x-Targx<0)
  { M1Forward();
  CalculAngle();
  }
  
 }*/

  
