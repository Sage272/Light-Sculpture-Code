/* Light Sculpture Architecture - Written by Kyle Stewart and YOUR_NAME - Updated 04302018*/

#include "LightSculpture.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                        Global Variable Declarations (Nouns)                                        //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const byte blues[] = {6, 8, 10, 11, 12, 7, 9, 5};
const byte greens[] = {2, 3, 4};
const byte whites[] = {13, 14};
int count = 0;
static int wCount = 0;
boolean wasTurnOffMode = false;
boolean wasNormalMode = false;
boolean wasNightMode = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                   Main Functions                                                   //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  setupLightSculpture();
  //turnOn();
}

void loop() {
  updateMode();
  if (mode == offMode) 
  {
    if(wasNormalMode)
    {
      turnOff();
    }
    wasNormalMode = false;
    wasNightMode = false;
    wasTurnOffMode = true;
    turnOffAll();
  }
  if (mode == lightShowMode) 
  {
    wasNormalMode = true;
    if(wasTurnOffMode || count == 0)
    {
      wasTurnOffMode = false;
      wasNightMode = false;
      turnOn();
    }
    //cut();
    //greenRotate();
    //waterDroplets(wCount);
    //wCount++;
    normalMode();
  }
  if (mode == nightLightMode)
  {
    if(wasNormalMode)
    {
      turnOff();
    }
    wasNightMode = true;
    wasNormalMode = false;
    runNightLight();
    
  }
  count++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Display Functions                                                 //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//party mode
void boringMode()
{
  const byte boringArr[] = {14, 6, 10, 12, 9, 5, 7, 11, 8, 3, 4, 2, 13};
  for(int i = 0; i < 14; i++)
  {
    digitalWrite(boringArr[i], HIGH);
    delay(100);
  }
  delay(500);
  for(int i = 13; i >= 0; i--)
  {
    digitalWrite(boringArr[i], LOW);
    delay(100);
  }
  delay(500);
}

//calm mode
void waterDroplets(int wCount)
{
  int ind = random(8);
  static int e;
  if(wCount == 1)
  {
    e = ind;
  }
  else
  {
    while (e == ind)
    {
      ind = random(8);
    }
    e = ind;
  }
  int delayNum = 500;
  fadeUpAndDownPatial(blues[ind], 1500, delayNum, 0.5);
  delay(delayNum/2);
}

//runs when the LS turns on and intitaly turns them on
void turnOn(){
  int delayVar = 125;
  const byte onArr[] = {8, 10, 11, 12, 7, 9};
  digitalWrite(14, HIGH);
  delay(delayVar);
  digitalWrite(6, HIGH);
  delay(delayVar);
  for(int i = 0; i < 6; i +=2)
  {
    digitalWrite(onArr[i], HIGH);
    digitalWrite(onArr[i + 1], HIGH);
    delay(delayVar);
  }
  digitalWrite(5, HIGH);
  delay(delayVar);
  digitalWrite(13, HIGH);
  delay(delayVar + 250);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(delayVar);
  digitalWrite(2, HIGH);
  delay(delayVar);
}

//turn off animation, turns all LEDs off
void turnOff(){
  turnOnAll();
  int delayVar = 125;
  const byte offArr[] = {8, 10, 11, 12, 7, 9};
  digitalWrite(2, LOW);
  delay(delayVar);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  delay(delayVar + 250);
  digitalWrite(13, LOW);
  delay(delayVar);
  digitalWrite(5, LOW);
  for(int i = 5; i > 0; i -= 2)
  {
    digitalWrite(offArr[i], LOW);
    digitalWrite(offArr[i - 1], LOW);
    delay(delayVar);
  }
  digitalWrite(6, LOW);
  delay(delayVar);
  digitalWrite(14, LOW);
  delay(delayVar);
}

//party mode
//uses a static int "count" to do specific funtions on only some run throughs
void normalMode()
{
  turnOnArray(greens, 3);
  breathe();
  if(count % 17 == 0  && count != 0)
  {
    look();
  }
  else if(count % 13 == 0)
  {
    blinkEye();
  }
  else if(count % 5027 == 0)
  {
    cut();
  }
  else if(count % 4089 == 0)
  {
    greenRotate();
  }
  breathe();
}

//has the green LEDs "look" back and forth
void look(){
  digitalWrite(3, LOW);
  delay(1500);
  digitalWrite(3, HIGH);
  delay(500);
  digitalWrite(4, LOW);
  delay(1500);
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(3, LOW);
  delay(1500);
  digitalWrite(3, HIGH);
}

//blinks the eye by turning some LEDs off
void blinkEye()
{
  int delayBig = 40;
  int delayMid = delayBig/2;
  int delaySm = 5;
  digitalWrite(5, LOW);
  delay(delayBig);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  delay(delaySm);
  digitalWrite(2, LOW);
  delay(delayMid);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  delay(delayBig);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  delay(delayBig * 10);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(delayBig);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  delay(delayMid);
  digitalWrite(2, HIGH);
  delay(delaySm);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
  delay(delayBig);
  digitalWrite(5, HIGH);
}

//has the blue LEDs "breathe" / fade down patialy then back up 
void breathe(){
  const byte arrayName[] = {6, 8, 10, 11, 12, 7, 9, 5, 13, 14};
  const byte arrayLength = 10;
  int duration = 3000;
  float fadingStepSize = getFadingStepSize(duration);
  for (float fractionOn = 1; fractionOn > 0.5; fractionOn -= fadingStepSize) {
    updateMode();
    if (mode != lightShowMode) return;

    pwm(arrayName, arrayLength, fractionOn);
    //updateStepSize(duration);
  }
  for (float fractionOn = 0.5; fractionOn < 1; fractionOn += fadingStepSize) {
    updateMode();
    if (mode != lightShowMode) return;

    pwm(arrayName, arrayLength, fractionOn);
    //updateStepSize(duration);
  }
  turnOnArray(arrayName, arrayLength);
}

void fadeUpAndDownPatial(const byte pin, int duration, int delayNum, float fraction) {
  float fadingStepSize = getFadingStepSize(2 * (duration/2));
  for (float fractionOn = 0; fractionOn < fraction; fractionOn += fadingStepSize) {
    updateMode();
    if (mode != lightShowMode) return; // check to see if still in lightShowMode - if not then leave the function now

    pwm(pin, fractionOn);
    //updateStepSize(duration);
  }
  //delay(delayNum);
  fadingStepSize = getFadingStepSize(duration);
  for (float fractionOn = fraction; fractionOn > 0; fractionOn -= fadingStepSize) {
    updateMode();
    if (mode != lightShowMode) return;

    pwm(pin, fractionOn);
    //updateStepSize(duration);
  }
  //digitalWrite(pin, HIGH); // make sure pin is at full power before leaving fadeUp()
}

void greenRotate()
{
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(greens[i], HIGH);
  }
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(greens[i], LOW);
    delay(250);
    digitalWrite(greens[i], HIGH);
  }
}

void cut()
{
  //{6, 8, 10, 11, 12, 7, 9, 5};
  digitalWrite(greens[1], HIGH);
  digitalWrite(greens[2], HIGH);
  for(int i = 1; i < 7; i++)
  {
    digitalWrite(blues[i], HIGH);
  }
  
}

/*for(int i = 2; i <=15; i++)
  {
    digitalWrite(i, HIGH);
  }*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Example Functions                                                 //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int arrLength = sizeof(exampleRightToLeftArray);
//void runLightShow() {
//  ExampleFunctionOne(100, 100,3); // First Simple Display
//  ExampleFunctionTwo(100, 100,3); // Second Simple Display
//  ExampleFunctionThree(100,100,3); //Third Simple Display
//  ExampleFunctionFour(100,100,3); //Fourth Simple Display
//  ExampleFunctionFive(100,100,3); //Fifth Simple Display
//  ExampleFunctionSix(100,100,3); //Sixth Simple Display
//  ExampleFunctionSeven(100,100,3); //Seventh Simple Display
//}
//
//void ExampleFunctionOne(int onTime, int offTime, int repeatCount) {
//// Turns on LEDs from left to right
//  int count;
//  int index;
//  int numberOfElements;
//    
//  numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array 
//
//  for (count = 0; count < repeatCount; count++)
//  {
//    for(index = 0; index <= numberOfElements; index++)
//    {
//        digitalWrite(exampleLeftToRightArray[index],HIGH);
//        delay(onTime);
//        digitalWrite(exampleLeftToRightArray[index],LOW);
//        delay(offTime);
//    }
//  }
//}
//
//void ExampleFunctionTwo(int onTime, int offTime, int repeatCount) {
//// Turns on LEDs from right to left
//    int count;
//    int index;
//    int numberOfElements;
//    
//    numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array 
//    
//    for (count = 0; count < repeatCount; count++)
//    {
//        for(index = 0; index <= numberOfElements; index++)
//        {
//            digitalWrite(exampleRightToLeftArray[index],HIGH);
//            delay(onTime);
//            digitalWrite(exampleRightToLeftArray[index],LOW);
//            delay(offTime);
//        }
//    }
//}
//
//void ExampleFunctionThree(int onTime, int offTime, int repeatCount) {
//// Fades up and down an array of LEDs
//    int count;
//    int numberOfElements;
//
//    numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array
//
//    for (count = 0; count < repeatCount; count++)
//    {
//        fadeUpArray(exampleRightToLeftArray,numberOfElements,onTime);
//        delayUsingKnob(5*onTime);
//        fadeDownArray(exampleRightToLeftArray,numberOfElements,onTime);
//        delayUsingKnob(5*onTime);
//    }
//}
//
//void ExampleFunctionFour(int onTime, int offTime, int repeatCount) {
//// Fades up each LED in an array in order, then fades them down in order
//    int count;
//    int numberOfElements;
//
//    numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array
//
//    for (count = 0; count < repeatCount; count++)
//    {
//        fadeUpArrayInOrder(exampleRightToLeftArray,numberOfElements,onTime);
//        delayUsingKnob(5*onTime);
//        fadeDownArrayInOrder(exampleRightToLeftArray,numberOfElements,onTime);
//        delayUsingKnob(5*onTime);
//    }
//}
//
//void ExampleFunctionFive(int onTime, int offTime, int repeatCount) {
//// Fades up and then down each LED in an array in order
//    int count;
//    int numberOfElements;
//
//    numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array
//
//    for (count = 0; count < repeatCount; count++)
//    {
//        blinkArrayInOrder(exampleRightToLeftArray,numberOfElements,onTime);
//        delayUsingKnob(5*onTime);
//        blinkArrayInOrder(exampleLeftToRightArray,sizeof(exampleLeftToRightArray),onTime);
//        delayUsingKnob(5*onTime);
//    }
//}
//
//void ExampleFunctionSix(int onTime, int offTime, int repeatCount) {
//// Cross Fades an array of LEDs. Fades up the first LED while fading down the second LED
//    int count;
//    int numberOfElements;
//
//    numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array
//
//    for (count = 0; count < repeatCount; count++)
//    {
//        crossFadeArrays(exampleLeftToRightArray, sizeof(exampleLeftToRightArray),exampleRightToLeftArray, sizeof(exampleRightToLeftArray),onTime);
//        delayUsingKnob(5*onTime);
//        crossFadeArrays(exampleRightToLeftArray, sizeof(exampleRightToLeftArray),exampleLeftToRightArray, sizeof(exampleLeftToRightArray),onTime);
//        delayUsingKnob(5*onTime);
//    }
//}
//
//void ExampleFunctionSeven(int onTime, int offTime, int repeatCount) {
//// Smoother version of Cross Fading an array of LEDs.
//// ExampleSix started with the exampleLeftToRightArray off, then it turned it on to full brightness with no delay and then
//// started cross fading the arrays
////
//// This example will first fade up the Fades up exampleLeftToRightArray, so that they dont just flash on, then do the cross
//// fading, then it will fade down the LED's
//
//    int count;
//    int numberOfElements;
//
//    numberOfElements = sizeof(exampleRightToLeftArray); // uses built in function sizeof to count the number of elements in an array
//
//    fadeUpArray(exampleLeftToRightArray, sizeof(exampleLeftToRightArray), onTime);
//    for (count = 0; count < repeatCount; count++)
//    {
//        crossFadeArrays(exampleLeftToRightArray, sizeof(exampleLeftToRightArray),exampleRightToLeftArray, sizeof(exampleRightToLeftArray),onTime);
//        delayUsingKnob(5*onTime);
//        crossFadeArrays(exampleRightToLeftArray, sizeof(exampleRightToLeftArray),exampleLeftToRightArray, sizeof(exampleLeftToRightArray),onTime);
//        delayUsingKnob(5*onTime);
//    }
//    fadeDownArray(exampleLeftToRightArray, sizeof(exampleLeftToRightArray),onTime);
//    delayUsingKnob(8*offTime);
//}
