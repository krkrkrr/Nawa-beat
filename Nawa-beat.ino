/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0; // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0; // value read from the pot
int outputValue = 0; // value output to the PWM (analog out)

int loopindex = 0;
int checkAimaitouched = 0;
/*
bool isTouched(int touch_pin) {
  return analogRead(touch_pin) >= 10;
}
*/
bool isTouched(int sensorValue, int checkNum)
{
  static int vecTouched[50];
  static int checkTouched = 0;
  int index = loopindex % 10;
  if(sensorValue < 20) sensorValue = 0;
//  if(sensorValue >= 120) sensorValue = 120;
  // check input and sum of 10 times => 100 then touched
  checkTouched -= vecTouched[index];
  checkTouched += sensorValue;
  vecTouched[index] = sensorValue;

  // debug
  if (loopindex % 10 == 0)
  {
    for (int i = 0; i < checkNum; i++)
    {
      if (vecTouched[i] >= 10)
      {
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(vecTouched[i]);
      }
    }
  Serial.print("checktouched = ");
  Serial.println(checkTouched);
  }

  return checkTouched >= checkNum * 10;
}

void setup()
{
  pinMode(LED0, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop()
{
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  // print the results to the Serial Monitor:
  if (loopindex % 100 == 0)
  {
    Serial.print("sensor = ");
    Serial.print(sensorValue);
    Serial.print("\t output = ");
    Serial.println(outputValue);
     if (isTouched(sensorValue, 20))    Serial.println("---- Touched ----");
  }
  
  if(isTouched(sensorValue, 20)) checkAimaitouched++;

  if(loopindex % 20 == 0){
    if (checkAimaitouched >= 10)
    {
      digitalWrite(LED0, HIGH);
      Serial.println("---- Touched ----");
    }
    else
    {
      digitalWrite(LED0, LOW);
    }
    checkAimaitouched = 0;
  }

  loopindex++;
  loopindex %= 1000;

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
