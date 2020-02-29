#define DEBUG 1                                    // comment out this line to remove debuggin Serial.print lines

const int axisCount = 3;                           // how many axis we are handling
char EOL = '~';                                    // End of Line - the delimiter for our acutator values
int maxCharCount = 4;                              // some insurance
int currentAxis;                                   // keep track of the current Axis being read in from serial port
int valueCharCount = 0;                            // how many value characters have we read (must be less than maxCharCount)

const int STEP[axisCount] = {8, 9, 10};            // Step Pins,  active HIGH
const int DIR[axisCount] = {11, 12, 13};           // Direction Pins, active HIGH
const byte DIR_ACTIVE[axisCount] = {LOW, LOW, LOW};
const char axisName[axisCount] = {'P', 'R', 'Y'};  // the letters ("names") sent from Sim Tools to identify each axis
const int axisScale[axisCount][2] = { {0, 2000}, {0, 2000}, {0, 2000} }; //Scaling
const int axisStepDelayMaxMinRate[axisCount][3] = { {10000, 1000, 500}, {10000, 1000, 500}, {10000, 1000, 500} };
int axisPosition[axisCount] = {996, 996, 996};        // current Axis positions, initialised to 6637 middle for 10 bit
int pos[axisCount] = {996, 996, 996};                 // Position logging, initialised to 6637 middle for 10 bit
int tmpAxisPosition[axisCount] = {0, 0, 0};        // Temporary Axis positions (used in Serial data collection)
int stepDirection[axisCount] = {1, 1, 1};          // 1 = increase position, -1 = decrease position
int delayInDir[axisCount] = {0, 0, 0};             // Current Delay for each axis
float accelSteps[axisCount] = {0, 0, 0};           // Calculated Accel/Decel Steps
int decelSteps[axisCount] = {0, 0, 0};             // Turn-around Decel Steps

int curStepDirection[axisCount] = {1, 1, 1};       // Step Direction of current command to compare to last step
int curDelay[axisCount] = {0, 0, 0};               // Initialize current delay for axis being calculated
int absStepsToGo[axisCount] = {0, 0, 0};           // Absolute steps for current move
int decelActive[axisCount] = {0, 0, 0};            // Deceleration is currently active

// set up some states for our state machine
// psReadAxis = next character from serial port tells us the Axis info that follows
// psReadValue = next characters from serial port until we hit our maxCharCount or EOL which tells us the value
enum TPortState { psReadAxis, psReadValue };
TPortState currentState = psReadAxis;

void setup()  {
  Serial.begin(9600);
  // Define pins
  for (int i = 0; i < axisCount; i++)  {
    pinMode(STEP[i], OUTPUT);
    pinMode(DIR[i], OUTPUT);
    delayInDir[i] = axisStepDelayMaxMinRate[i][0];     // Initialize all delays at slowest value
    accelSteps[i] = ((axisStepDelayMaxMinRate[i][0] - axisStepDelayMaxMinRate[i][1]) / axisStepDelayMaxMinRate[i][2]);

  }
}
char tmpChar;
void loop()
{
  while (Serial.available()) 
    { 
      tmpChar = Serial.read();
      Serial.write(tmpChar);
        }
  
  
  //serialEvent();
/*  for (int i = 0; i < axisCount; i++)
  {
    // check axis position
    if (pos[i] != axisPosition[i])
    {
      // check direction, calculate acceleration (delay) and write DIR pin
      absStepsToGo[i] = abs(pos[i] - axisPosition[i]);

      if (pos[i] >= axisPosition[i])
      {
        if ((curStepDirection[i] == 1) && (decelSteps[i] >= 1))
        {
          decelActive[i] = 1;
          accelCalc(i);
          digitalWrite(DIR[i], HIGH);
          pos[i] = pos[i] + 1;
          stepDirection[i] = 1;
          decelSteps[i] = decelSteps[i] - 1;
        }
        else
        {
          curStepDirection[i] = -1;
          decelActive[i] = 0;
          accelCalc(i);
          digitalWrite(DIR[i], LOW);
          pos[i] = pos[i] - 1;
          stepDirection[i] = -1;
          decelSteps[i] = decelSteps[i] + 1;
          if (decelSteps[i] > accelSteps[i])
          {
            decelSteps[i] = accelSteps[i];
          }
        }
      }
      
      if (pos[i] < axisPosition[i])
      {
        if ((curStepDirection[i] == -1) && (decelSteps[i] >= 1))
        {
          decelActive[i] = 1;
          accelCalc(i);
          digitalWrite(DIR[i], LOW);
          pos[i] = pos[i] - 1;
          stepDirection[i] = -1;
          decelSteps[i] = decelSteps[i] - 1;
        }
        else
        {
          curStepDirection[i] = 1;
          decelActive[i] = 0;
          accelCalc(i);
          digitalWrite(DIR[i], HIGH);
          pos[i] = pos[i] + 1;
          stepDirection[i] = 1;
          decelSteps[i] = decelSteps[i] + 1;
          if (decelSteps[i] > accelSteps[i])
          {
            decelSteps[i] = accelSteps[i];
          }
        }
      }

      if (absStepsToGo[i] >= 0)
      {
        // make one step
        digitalWrite(STEP[i], HIGH);
        delayMicroseconds(10);
        digitalWrite(STEP[i], LOW);
        delayMicroseconds(delayInDir[i]);
      }

#ifdef DEBUG
      Serial.print("Axis ");
      Serial.print(i);
      Serial.print(" Direction ");
      Serial.print(stepDirection[i]);
      Serial.print(" Pos ");
      Serial.print(pos[i]);
      Serial.print(" axisPosition ");
      Serial.print(axisPosition[i]);
      Serial.print(" absStepsToGo ");
      Serial.print(absStepsToGo[i]);
      Serial.print(" delayInDir ");
      Serial.print(delayInDir[i]);
      Serial.print(" decelSteps ");
      Serial.print(decelSteps[i]);
      Serial.print(" decelActive ");
      Serial.print(decelActive[i]);
      Serial.print("\n");
#endif
        
    }
  }*/
}

void accelCalc(int axis) {
  if (curStepDirection[axis] == stepDirection[axis])
  {
//    if (absStepsToGo[axis] <= accelSteps[axis] || decelActive[axis] == 1)
    if (decelActive[axis] == 1)
    {
      curDelay[axis] = delayInDir[axis] + axisStepDelayMaxMinRate[axis][2];
    }
    else
    {
      curDelay[axis] = delayInDir[axis] - axisStepDelayMaxMinRate[axis][2];
    }
  }
  else
  {
    curDelay[axis] = axisStepDelayMaxMinRate[axis][0];
  }

  if (curDelay[axis] <= axisStepDelayMaxMinRate[axis][1])
  {
    delayInDir[axis] = axisStepDelayMaxMinRate[axis][1];
  }
  else if (curDelay[axis] >= axisStepDelayMaxMinRate[axis][0])
  {
    delayInDir[axis] = axisStepDelayMaxMinRate[axis][0];
    if (decelActive[axis] == 1)
     {
       decelActive[axis] = 0;
     }
  }
  else
  {
    delayInDir[axis] = curDelay[axis];
  }
}



// this code only runs when we have serial data available. ie (Serial.available() > 0).
void serialEvent() {
  char tmpChar;
  int tmpValue;

  while (Serial.available()) {
    // if we're waiting for a Axis name, grab it here
    if (currentState == psReadAxis) {
      tmpChar = Serial.read();

#ifdef DEBUG
      Serial.print("read in ");
      Serial.println(tmpChar);
#endif

      for (int i = 0; i < axisCount; i++) {    // look for our axis in the array of axis names we set up
        if (tmpChar == axisName[i]) {

#ifdef DEBUG
          Serial.print("which is axis ");
          Serial.println(i);
#endif

          currentAxis = i;                            // remember which axis we found
          currentState = psReadValue;                 // start looking for the Axis position
          tmpAxisPosition[currentAxis] = 0;           // initialise the new position
          valueCharCount = 0;                         // initialise number of value chars read in
          //break;
        }
      }
    }

    // if we're ready to read in the current Axis position data
    if (currentState == psReadValue) {
      while ((valueCharCount < maxCharCount) && Serial.available()) {
        tmpValue = Serial.read();
        if (tmpValue != EOL) {
          tmpValue = tmpValue - 48;
          if ((tmpValue < 0) || (tmpValue > 9)) tmpValue = 0;
          tmpAxisPosition[currentAxis] = tmpAxisPosition[currentAxis] * 10 + tmpValue;
          valueCharCount++;
        }
        else break;
      }

      // if we've read the value delimiter, update the Axis and start looking for the next Axis name
      if (tmpValue == EOL || valueCharCount == maxCharCount) {
        // scale the new position so the value is between 0 and Axis Scale (255 = 8 bit, 1023 = 10 bit)
        axisPosition[currentAxis] = map(tmpAxisPosition[currentAxis], 0, 255, axisScale[currentAxis][0], axisScale[currentAxis][1]);

#ifdef DEBUG
        Serial.print("axisPosition ");
        Serial.println(axisPosition[currentAxis]);
        Serial.print("Position ");
        Serial.println(pos[currentAxis]);
        Serial.print("\n");
#endif

        currentState = psReadAxis;
      }
    }
  }
}
