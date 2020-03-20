#include <Servo.h>

typedef enum {  NONE, GOT_S, GOT_R, GOT_T, GOT_F, GOT_G } states;

states state = NONE;

unsigned int currentValue;
Servo turboservo;

void setup ()
{
  Serial.begin (115200);
  state = NONE;
  pinMode(22, OUTPUT);
  turboservo.attach(22);
  turboservo.write(0);
}

void processSpeed (const unsigned int value)
{
  //Print out the Speed value
  Serial.print ("Speed = ");
  Serial.println (value);
}


void processRPM (const unsigned int value)
{
  //Print out the RPM value
  Serial.print ("RPM = ");
  Serial.println (value);
}

void processTurbo (const unsigned int value)
{
  //Print out the Turbo value
  Serial.print ("Turbo = ");
  Serial.println (value);
  turboservo.write(value);
}

void processFuel (const unsigned int value)
{
  //Print out the Fuel value
  Serial.print ("Fuel = ");
  Serial.println (value);
}

void processGear (const unsigned int value)
{
  //Print out the Gear value
  Serial.print ("Gear = ");
  Serial.println (value); 
}

void handlePreviousState ()
{
  switch (state)
  {
  case GOT_S:
    processSpeed (currentValue);
    break;
  case GOT_R:
    processRPM (currentValue);
    break;
  case GOT_T:
    processTurbo (currentValue);
    break;
  case GOT_F:
    processFuel (currentValue);
    break;
  case GOT_G:
    processGear (currentValue);
    break;
  }

  currentValue = 0;
}

void processIncomingByte (const byte c)
{
  if (isdigit (c))
  {
    currentValue *= 10;
    currentValue += c - '0';
  }  // end of digit
  else
  {

    // The end of the number signals a state change
    handlePreviousState ();

    // set the new state
    switch (c)
    {
    case 'S':
      state = GOT_S;
      break;
    case 'R':
      state = GOT_R;
      break;
    case 'T':
      state = GOT_T;
      break;
    case 'F':
      state = GOT_F;
      break;
    case 'G':
      state = GOT_G;
      break;
    default:
      state = NONE;
      break;
    }  // end of switch on incoming byte
  } // end of not digit 
 
} // end of processIncomingByte

void loop ()
{
  while (Serial.available ()){
 Serial.println(Serial.read());
  
  }
    

 
}  // end of loop
