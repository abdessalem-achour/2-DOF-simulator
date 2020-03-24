#include <SoftwareSerial.h>
int c;
SoftwareSerial mySerial(10, 11);
void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);

}

    
void loop() {
  
  if (mySerial.available()) 
     {
      c=mySerial.read();
      Serial.print("Axe1a -->  ");
      Serial.print(c-127);
      Serial.print(" | ");
      c=mySerial.read();
      Serial.print("Axe2a -->   ");
      Serial.println(c-127);
      Serial.print(" | ");
      c=mySerial.read();
      Serial.print("Roll -->   ");
      Serial.println(c-127);
     }

}
