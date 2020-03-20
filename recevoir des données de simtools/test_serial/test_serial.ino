#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}
int c ;
void loop() 
{
   while (Serial.available()) 
     {
      c= Serial.read();
      mySerial.write(c);
     }
}
