#include "LightSculptureLib.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                        Global Variable Declarations (Nouns)                                        //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool switchIsOff = true;

const byte knobPin = A5; // Potentiometer is connected to Pin A5 on the Arduino
const byte switchPin = A4; // ON/OFF Switch is connected to Pin A4 on the Arduino
const int maxKnobValue = 1023; // The Analog to Digital converter maps the analog voltages (0-5V) into 1024 (0-1023) values
const int numOfModes = 3; //Number of modes

const int nightLightRange = 255; // NightLight mode is active for values 0-255 on the potentiometer
const int nightLightMinValue = maxKnobValue - nightLightRange; // = 1023 - 255 OR 768
const int pwmPeriod = 10000; // Period of the PWM signal is

int prevMode = findMode();

float knobValue = analogRead(knobPin);




float fadingStepSize = 0.01;
const int msPerStep = 10;
const float maxKnobRatio = 0.9;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                   Mode Selection                                                   //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * setupLightSculpture()
 *
 * sets up the pinModes of pin 2-15 as OUTPUT
 *
 * sets up the Potentiometer pin A5 (knobPin) as an INPUT - this will be the pin that we later use a analogRead on
 *
 * sets up the Potentiometer pin A4 (switchPin) as an INPUT_PULLUP - this will be the pin that we later use a digitalRead on

 * sets up the serial monitor at 9600 baud
 */
void setupLightSculpture() {
  for (byte pin = 2; pin < 16; pin++) {
    pinMode(pin, OUTPUT);
  }

  pinMode(knobPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP);

  Serial.begin(9600);
}

int findMode()
{
  updateKnobValue();
  updateOffSwitch();
  
  int modeRange = maxKnobValue/numOfModes;
  if(switchIsOff)
  {
    return 0;
  }
  else if(knobValue < modeRange)
  {
    return 1;
  }
  else if(knobValue > modeRange * 2)
  {
    return 3;
  }
  else
  {
    return 2;
  }
}

/**
 * updateOffSwitch()
 *
 * Reads the value of pin A4 (switchPin) and sets global variable switchIsOff to that value
 *
 * When digitalRead(switchPin =A4) if the pin A4 is (HIGH ==1) or at 5V then the light sculpture should be OFF
 */
void updateOffSwitch() {
  switchIsOff = digitalRead(switchPin);
}

void updateKnobValue() {
  knobValue = analogRead(knobPin);
}

void setPrevMode()
{
  prevMode = findMode();
}

///////////////////////////////////////////



///////////////////////////////////////////

void nightLight() {
  int onTime = 1;
  int offTime = 10;

  turnOnAll();
  delayMicroseconds(onTime);
  turnOffAll();
  delay(offTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Fading Functions                                                  //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * fadeUp()
 *
 * Uses PWM to turn on a LED (specified by pin) from 0% PWM to 100% PWM over time (specified by duration)
 *
 * Devides 1.0 by the specified duration to get the fading step size in updateFadingStepSize()
 *
 * In addition - fadeUp() can be interrupted by a change in knob state / mode.
 *
 * @param pin - the pin on the arduino to fadeUp
 * @param duration - the time in milliseconds to go from 0% to 100% brightness
 */
void fadeUp(const byte pin, int duration) {
  updateFadingStepSize(duration);
  for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    pwm(pin, fractionOn);
    
  }
  digitalWrite(pin, HIGH); // make sure pin is at full power before leaving fadeUp()
}

/**
 * fadeUpAll()
 *
 * Uses PWM to turn on all LED (pins 2 -15 if connected to LEDs) from 0% PWM to 100% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeUpAll() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeUpAll() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeUpAll() can be interrupted by a change in knob state / mode.
 *
 * @param duration - the time in milliseconds to go from 0% to 100% brightness
 */
void fadeUpAll(int duration) {
  for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(fractionOn);
    updateFadingStepSize(duration);
  }
  turnOnAll();
}

/**
 * fadeUpArray()
 *
 * Uses PWM to turn on the pins connected to LEDs specified in a arrayName from 0% PWM to 100% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeUpArray() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeUpArray() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeUpArray() can be interrupted by a change in knob state / mode.
 *
 * @param arrayName - the name of the array to fadeUp - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 0% to 100% brightness
 */
void fadeUpArray(const byte arrayName[], const byte arrayLength, int duration) {
  for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(arrayName, arrayLength, fractionOn);
    updateFadingStepSize(duration);
  }
  turnOnArray(arrayName, arrayLength);
}

/**
 * fadeUpArrayInOrder()
 *
 * Uses PWM to turn on the pins connected to LEDs specified in a arrayName from 0% PWM to 100% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeUpArrayInOrder() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeUpArrayInOrder() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeUpArrayInOrder() can be interrupted by a change in knob state / mode.
 *
 * The difference between fadeUpArrayInOrder and fadeUpArray is that fadeUpArrayInOrder fades up the each pin in the array
 * individually while fadeUpArray fades up all the pins in an array at the same time.
 *
 * @param arrayName - the name of the array to fadeUp - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 0% to 100% brightness
 *
 */
void fadeUpArrayInOrder(const byte arrayName[], const byte arrayLength, int duration) {
  for (byte index = 0; index < arrayLength; index++) {
    byte pin = arrayName[index];
    fadeUp(pin, duration);
  }
}
/**
 * fadeDown()
 *
 * Uses PWM to turn off a LED (specified by pin) from 100% PWM to 0% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeDown() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeDown() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeDown() can be interrupted by a change in knob state / mode.
 *
 * @param pin - the pin on the arduino to fadeDown
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 */
void fadeDown(const byte pin, int duration) {
  for (float fractionOn = 1; fractionOn > 0; fractionOn -= fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(pin, fractionOn);
    updateFadingStepSize(duration);
  }
}

/**
 * fadeDownAll()
 *
 * Uses PWM to turn off all LED (pins 2 -15 if connected to LEDs) from 100% PWM to 0% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeDownAll() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeDownAll() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeDownAll() can be interrupted by a change in knob state / mode.
 *
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 */
void fadeDownAll(int duration) {
  for (float fractionOn = 1; fractionOn > 0; fractionOn -= fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(fractionOn);
    updateFadingStepSize(duration);
  }
}

/**
 * fadeDownArray()
 *
 * Uses PWM to turn off the pins connected to LEDs specified in a arrayName from 100% PWM to 0% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeDownArray() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeUpArray() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeDownArray() can be interrupted by a change in knob state / mode.
 *
 * @param arrayName - the name of the array to fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 */
void fadeDownArray(const byte arrayName[], const byte arrayLength, int duration) {
  for (float fractionOn = 1; fractionOn > 0; fractionOn -= fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(arrayName, arrayLength, fractionOn);
    updateFadingStepSize(duration);
  }
}

/**
 * fadeDownArrayInOrder()
 *
 * Uses PWM to turn off the pins connected to LEDs specified in a arrayName from 100% PWM to 0% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeDownArrayInOrder() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeDownArrayInOrder() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeDownArrayInOrder() can be interrupted by a change in knob state / mode.
 *
 * The difference between fadeDownArrayInOrder and fadeDownArray is that fadeDownArrayInOrder fades down each pin in the array
 * individually while fadeDownArray fades down all the pins in an array at the same time.
 *
 * @param arrayName - the name of the array to fadDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 *
 */
void fadeDownArrayInOrder(const byte arrayName[], const byte arrayLength, int duration) {
  for (byte index = 0; index < arrayLength; index++) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    byte pin = arrayName[index];
    fadeDown(pin, duration);
  }
}

/**
 * blinkArrayInOrder()
 *
 * Uses PWM to fadeUp and then fadeDown the pins connected to LEDs specified in a arrayName from 0% PWM to 100% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() blinkArrayInOrder allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to blinkArrayInOrder() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - blinkArrayInOrder() can be interrupted by a change in knob state / mode.
 *
 * @param arrayName - the name of the array to fadeUp and then fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 0% to 100% brightness
 */
void blinkArrayInOrder(const byte arrayName[], const byte arrayLength, int duration) {
  for (byte index = 0; index < arrayLength; index++) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    byte pin = arrayName[index];
    fadeUp(pin, duration);
    fadeDown(pin, duration);
  }
}

/**
 * blinkTwoArraysInOrder()
 *
 * Uses PWM to turn fadeUp and then fadeDown the pins connected to LEDs specified in two array arrayName from 0% PWM to 100% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() blinkTwoArraysInOrder() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to blinkTwoArraysInOrder() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - blinkTwoArraysInOrder() can be interrupted by a change in knob state / mode.
 *
 * Note: array1 and array2 must have the same number of elements.
 *
 * @param array1 - the name of the first array to fadeUp and then fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
 * @param array2 - the name of the second array to fadeUp and then fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray2[] = {2, 3, 4, 5, 6, 7, 8};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 0% to 100% brightness
 */
void blinkTwoArraysInOrder(const byte array1[], const byte array2[], const byte arrayLength, int duration) {
  for (byte index = 0; index < arrayLength; index++) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    const byte ledSet[] = {array1[index], array2[index]};
    fadeUpArray(ledSet, sizeof(ledSet), duration);
    fadeDownArray(ledSet, sizeof(ledSet), duration);
  }
}

/**
 * fadeUpTwoArraysInOrder()
 *
 * Uses PWM to turn on the pins connected to LEDs specified in a arrayName from 100% PWM to 0% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeUpTwoArraysInOrder() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeUpTwoArraysInOrder() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeUpTwoArraysInOrder() can be interrupted by a change in knob state / mode.
 *
 * The difference between fadeUpTwoArraysInOrder and fadeUpTwoArrays is that fadeUpArrayInOrder fades up each pin in the array
 * individually while fadeUpTwoArrays fades up all the pins in an array at the same time.
 *
 * Note: array1 and array2 must have the same number of elements.
 *
 * @param array1 - the name of the first array to fadeUp - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
 * @param array2 - the name of the second array to fadeUp - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray2[] = {2, 3, 4, 5, 6, 7, 8};
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 *
 */
void fadeUpTwoArraysInOrder(const byte array1[], const byte array2[], const byte arrayLength, int duration) {
  for (byte index = 0; index < arrayLength; index++) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    const byte ledSet[] = {array1[index], array2[index]};
    fadeUpArray(ledSet, sizeof(ledSet), duration);
  }
}

/**
 * fadeDownTwoArraysInOrder()
 *
 * Uses PWM to turn off the pins connected to LEDs specified in a arrayName from 100% PWM to 0% PWM over time (specified by duration)
 *
 * Like delayUsingKnob() fadeDownTwoArraysInOrder() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to fadeDownTwoArraysInOrder() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - fadeDownTwoArraysInOrder() can be interrupted by a change in knob state / mode.
 *
 * The difference between fadeDownTwoArraysInOrder and fadeDownTwoArrays is that fadeDownArrayInOrder fades down each pin in the array
 * individually while fadeDownTwoArrays fades down all the pins in an array at the same time.
 *
 * Note: array1 and array2 must have the same number of elements.
 *
 * @param array1 - the name of the first array to fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
 * @param array2 - the name of the second array to fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray2[] = {2, 3, 4, 5, 6, 7, 8};
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 *
 */
void fadeDownTwoArraysInOrder(const byte array1[], const byte array2[], const byte arrayLength, int duration) {
  for (byte index = 0; index < arrayLength; index++) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    const byte ledSet[] = {array1[index], array2[index]};
    fadeDownArray(ledSet, sizeof(ledSet), duration);
  }
}

/**
 *  crossFade()
 *
 * Cross fading allows you to have a pair of LEDs fading in opposite directions.
 * One LED will fade from dark to bright, while at the same time the other LED
 * will fade from bright to dark
 *
 * crossFade() uses PWM to fade the pins connected to LEDs specified in a downPin and upPin over time (specified by duration)
 *
 * Like delayUsingKnob() crossFade() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to crossFade() a pin based on the position of the knob (value of pin A5)
 *
 * In addition - crossFade() can be interrupted by a change in knob state / mode.
 *
 * @param downPin - the number of the pin to fadeDown
 * @param upPin - the number of the pin to fadeUp
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 */
void crossFade(const byte downPin, const byte upPin, int duration) {
  for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    int upOnTime = getOnTimeUsingFraction(fractionOn);
    int downOnTime = getOnTimeUsingFraction(1 - fractionOn);

    digitalWrite(upPin, HIGH);
    digitalWrite(downPin, HIGH);

    if (upOnTime < downOnTime) {
      delayMicroseconds(upOnTime);
      digitalWrite(upPin, LOW);
      delayMicroseconds(downOnTime - upOnTime);
      digitalWrite(downPin, LOW);
      delayMicroseconds(pwmPeriod - downOnTime);
    } else {
      delayMicroseconds(downOnTime);
      digitalWrite(downPin, LOW);
      delayMicroseconds(upOnTime - downOnTime);
      digitalWrite(upPin, LOW);
      delayMicroseconds(pwmPeriod - upOnTime);
    }

    updateFadingStepSize(duration);
  }
  digitalWrite(upPin, HIGH);
}

/**
 * crossFadeArrays()
 *
 * Cross fading allows you to have a pair of LEDs fading in opposite directions.
 * One LED will fade from dark to bright, while at the same time the other LED
 * will fade from bright to dark
 *
 * crossFadeArrays() uses PWM to fade the pins connected to LEDs specified in a downArrayName and upArrayName over time (specified by duration)
 *
 * Like delayUsingKnob() crossFadeArrays() allows knob position to scale the value duration which effectively scales
 * the  amount of time it takes to crossFadeArrays() a array based on the position of the knob (value of pin A5)
 *
 * In addition - crossFadeArrays() can be interrupted by a change in knob state / mode.
 *
 * Note: downArrayName and upArrayName must have the same number of elements.
 *
 * @param downArrayName - the name of the array to fadeDown - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
 * @param downArrayLength - the number of elements in DownArrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param upArrayName the name of the second array to fadeUp - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray2[] = {2, 3, 4, 5, 6, 7, 8};
 * @param upArrayLength - the number of elements in upArrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 * @param duration - the time in milliseconds to go from 100% to 0% brightness
 */
void crossFadeArrays(const byte downArrayName[], const byte downArrayLength, const byte upArrayName[], const byte upArrayLength, int duration) {
  for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    int upOnTime = getOnTimeUsingFraction(fractionOn);
    int downOnTime = getOnTimeUsingFraction(1 - fractionOn);

    turnOnArray(downArrayName, downArrayLength);
    turnOnArray(upArrayName, upArrayLength);

    if (upOnTime < downOnTime) {
      delayMicroseconds(upOnTime);
      turnOffArray(upArrayName, upArrayLength);
      delayMicroseconds(downOnTime - upOnTime);
      turnOffArray(downArrayName, downArrayLength);
      delayMicroseconds(pwmPeriod - downOnTime);
    } else {
      delayMicroseconds(downOnTime);
      turnOffArray(downArrayName, downArrayLength);
      delayMicroseconds(upOnTime - downOnTime);
      turnOffArray(upArrayName, upArrayLength);
      delayMicroseconds(pwmPeriod - upOnTime);
    }

    updateFadingStepSize(duration);
  }
  turnOnArray(upArrayName, upArrayLength);
}

void fadeUpPartial(const byte pin, int duration, int delayNum, float fractionFrom, float fractionTo) {
  updateFadingStepSize(duration);
  for (float fractionOn = fractionFrom; fractionOn < fractionTo; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    pwm(pin, fractionOn);
    //updateStepSize(duration);
  }
}

void fadeDownPartial(const byte pin, int duration, int delayNum, float fractionFrom, float fractionTo) {
  updateFadingStepSize(duration);
  for (float fractionOn = fractionFrom; fractionOn > fractionTo; fractionOn -= fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }
    pwm(pin, fractionOn);
    //updateStepSize(duration);
  }
}

void fadeUpArrayPartial(const byte arrayName[], const byte arrayLength, int duration, float fractionFrom, float fractionTo)
{
  for (float fractionOn = fractionFrom; fractionOn < fractionTo; fractionOn += fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(arrayName, arrayLength, fractionOn);
    updateFadingStepSize(duration);
  }
  turnOnArray(arrayName, arrayLength);
}

void fadeDownArrayPartial(const byte arrayName[], const byte arrayLength, int duration, float fractionFrom, float fractionTo)
{
  for (float fractionOn = fractionFrom; fractionOn > fractionTo; fractionOn -= fadingStepSize) {
    int mode = findMode();
    if (mode != prevMode)
    {
      return; // check to see if still in the mode it started in - if not then leave the function now
    }

    pwm(arrayName, arrayLength, fractionOn);
    updateFadingStepSize(duration);
  }
}



//Devides 1.0 by the specified duration to get the fading step size in updateFadingStepSize()
void updateFadingStepSize(int duration){
  if(duration <= 0)
  {
    duration = 1;
  }
  fadingStepSize = ((1.0/duration) * (10));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Helper Functions                                                  //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * pwm(allLedPins)
 *
 * The function that actually does the software pwm of pins 2-15
 * Essentially turns all pins on for a very brief period of time then turns them off for a very brief period of time.
 * The amount of time that the LEDs should be on is calculated by the helper function getOnTimeUsingFraction().
 *
 * @param fractionOn - the fractional amount the the LEDs should be on - for example if they should be at 50% brightness,
 * then fractionOn should be passed in as 0.50
 */
void pwm(float fractionOn) {
  int onTime = getOnTimeUsingFraction(fractionOn);
  int offTime = pwmPeriod - onTime;

  turnOnAll();
  delayMicroseconds(onTime);
  turnOffAll();
  delayMicroseconds(offTime);
}

/**
* pwm(aSpecificLedPin)
*
* The function that actually does the software pwm of a LED pin
* Essentially turns the specified LED pin on for a very brief period of time then turns it off for a very brief period of time.
* The amount of time that the LED should be on is calculated by the helper function getOnTimeUsingFraction().
*
* @param pin - the specific LED pin to pwm
* @param fractionOn - the fractional amount the the LEDs should be on - for example if they should be at 50% brightness,
* then fractionOn should be passed in as 0.50
*/
void pwm(const byte pin, float fractionOn) {
  int onTime = getOnTimeUsingFraction(fractionOn);
  int offTime = pwmPeriod - onTime;

  digitalWrite(pin, HIGH);
  delayMicroseconds(onTime);
  digitalWrite(pin, LOW);
  delayMicroseconds(offTime);
}

/**
* pwm(arrayName, arrayLength, fractionOn)
*
* Like pwm(allLedPins) pwm(arrayName)does a software pwm of the LEDs pins specified in arrayName.
* pwm(arrayName) Essentially turns the specified LED pin on for a very brief period of time then turns it off for a very
* brief period of time. The amount of time that the LED should be on is calculated by the helper function
* getOnTimeUsingFraction().
*
* @param arrayName - the name of the array to pwm - must be declared in YourInitials_LightSculpture.ino as a
*        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
* @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
*        For example - sizeof(exampleArray)
* @param fractionOn - the fractional amount the the LEDs should be on - for example if they should be at 50% brightness,
* then fractionOn should be passed in as 0.50
*/
void pwm(const byte arrayName[], const byte arrayLength, float fractionOn) {
  int onTime = getOnTimeUsingFraction(fractionOn);
  int offTime = pwmPeriod - onTime;
  // pwmPeriod  is a constant it is equal to 10000;

  turnOnArray(arrayName, arrayLength);
  delayMicroseconds(onTime);
  turnOffArray(arrayName, arrayLength);
  delayMicroseconds(offTime);
}

/**
 * turnOnArray()
 *
 * Turns on LEDs connected to pins specified in arrayName at full brightness with no delay
 *
 * @param arrayName - the name of the array to turnOn - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 */
 void turnOnArray(const byte arrayName[], const byte arrayLength) {
  for (byte index = 0; index < arrayLength; index++) {
    digitalWrite(arrayName[index], HIGH);
  }
}

/**
 * turnOffArray()
 *
 * Turns off LEDs connected to pins specified in arrayName with no delay
 *
 * @param arrayName - the name of the array to turnOff - must be declared in YourInitials_LightSculpture.ino as a
 *        For example - const byte exampleArray1[] = {9, 10, 11, 12, 13, 14, 15};
 * @param arrayLength - the number of elements in arrayName - you can count them or use the example here when calling the function fadeUpArray
 *        For example - sizeof(exampleArray)
 */
void turnOffArray(const byte arrayName[], const byte arrayLength) {
  for (byte index = 0; index < arrayLength; index++) {
    digitalWrite(arrayName[index], LOW);
  }
}

/**
 * getOnTimeUsingFraction()
 * 
 * Corrects the fraction on so that we do not percive the change in brightness
 */
int getOnTimeUsingFraction(float fractionOn) {
  fractionOn = pow(fractionOn, 3.0);
  int onTime = fractionOn * pwmPeriod;
  return onTime;
}

/**
 * turnOnAll()
 *
 * Turns on LEDs connected to pins 2-15 at full brightness with no delay
 */
void turnOnAll() {
  for (byte ledNum = 2; ledNum <= 15; ledNum++) {
    digitalWrite(ledNum, HIGH);
  }
}

/**
 * turnOffAll()
 *
 * Turns off LEDs connected to pins 2-15 with no delay
 */
void turnOffAll() {
  for (byte ledNum = 2; ledNum <= 15; ledNum++) {
    digitalWrite(ledNum, LOW);
  }
}
