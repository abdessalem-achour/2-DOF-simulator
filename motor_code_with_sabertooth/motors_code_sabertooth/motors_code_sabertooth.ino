#include <USBSabertooth.h>
#include <SoftwareSerial.h>
#include<Wire.h>

SoftwareSerial      SWSerial(NOT_A_PIN, 11);  // RX on no pin (unused), TX on pin 11 (to S1).
USBSabertoothSerial C(SWSerial);              // Use SWSerial as the serial port.
                      
USBSabertooth ST(C, 128); 

char c;
                   
void setup()
{ 
  Serial.begin(9600);
  SWSerial.begin(9600);
  delay(300);
  
}


void loop()
{
 if(Serial.available())
    { 
    c=Serial.read();
    Serial.println(c);
      switch(c)
      { 
        case 'A': M1Forward(300);break;
        case 'Z': M1Backward(300);break;
        case 'Q': M2Forward(300);break;
        case 'S': M2Backward(300);break;
        case 'W': MForward(300);break;
        case 'X': MBackward(300);break;
        default: stopp();
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



