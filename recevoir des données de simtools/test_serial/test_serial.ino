void setup() {
  Serial.begin(9600);

}
char tmpChar;
void loop() 
{
   while (Serial.available()) 
    { 
      tmpChar = Serial.read();
      Serial.print(tmpChar);
     }
}
