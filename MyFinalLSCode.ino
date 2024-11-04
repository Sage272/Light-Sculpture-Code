#include "LightSculptureLib.h"


const byte blues[] = {6, 8, 10, 11, 12, 7, 9, 5};
const byte greens[] = {2, 3, 4};
const byte whites[] = {13, 14};
int count = 0;
static int wCount = 0;
boolean wasTurnOffMode = false;
boolean wasNormalMode = false;
boolean wasNightMode = false;
boolean wasCalmMode = false;

void setup() {
  // put your setup code here, to run once:
  setupLightSculpture();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(findMode() == 0)
  {
    if(wasNormalMode)
    {
      turnOff();
    }
    wasNormalMode = false;
    wasNightMode = false;
    wasCalmMode = false;
    wasTurnOffMode = true;
    turnOffAll();
  }
  if(findMode() == 1)
  {
    setPrevMode();//DO NOT DELETE! it is essential for exiting the functions when you change modes
    wasNormalMode = true;
    if(wasTurnOffMode || count == 0 || wasNightMode || wasCalmMode)
    {
      turnOn();
    }
    wasTurnOffMode = false;
    wasNightMode = false;
    wasCalmMode = false;
    normalMode();
  }
  if(findMode() == 2)
  {
    setPrevMode();//DO NOT DELETE! it is essential for exiting the functions when you change modes
    wasCalmMode = true;
    if(wasNormalMode)
    {
      turnOff();
    }
    wasTurnOffMode = false;
    wasNightMode = false;
    wasNormalMode = false;
    waterDroplets(wCount);
    wCount++;
  }
  if(findMode() == 3)
  {
    if(wasNormalMode)
    {
      turnOff();
    }
    wasNightMode = true;
    wasNormalMode = false;
    wasCalmMode = false;
    wasTurnOffMode = false;
    nightLight();
    
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
  fadeUpPartial(blues[ind], 1500, delayNum, 0.0, 0.5);
  fadeDownPartial(blues[ind], 1500, delayNum, 0.5, 0.0);
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
  if(count % 9 == 0  && count != 0)
  {
    look();
  }
  else if(count % 6 == 0)
  {
    blinkEye();
  }
  else if(count % 507 == 0)
  {
    cut();
  }
  else if(count % 49 == 0)
  {
    greenRotate();
  }
  breathe();
}

//has the green LEDs "look" back and forth
void look(){
//  fadeDown(2, 100);
//  fadeDown(3, 100);
//  fadeDown(4, 100);
  fadeDownArray(greens, 3, 50);
  delay(500);
  digitalWrite(3, HIGH);
  if(findMode() != 1) return;
  delay(1500);
  if(findMode() != 1) return;
  digitalWrite(3, LOW);
  delay(500);
  if(findMode() != 1) return;
  digitalWrite(4, HIGH);
  delay(1500);
  if(findMode() != 1) return;
  digitalWrite(4, LOW);
  delay(500);
  if(findMode() != 1) return;
  digitalWrite(3, HIGH);
  delay(1500);
  if(findMode() != 1) return;
  digitalWrite(3, LOW);
  delay(500);
  fadeUpArray(greens, 3, 50);
//  fadeUp(2, 100);
//  fadeUp(3, 100);
//  fadeUp(4, 100);
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
  const byte bluesAndWhites[] = {6, 8, 10, 11, 12, 7, 9, 5, 13, 14};
  const byte arrayLength = 10;
  fadeDownArrayPartial(bluesAndWhites, arrayLength, 3000, 1.0, 0.5);
  fadeUpArrayPartial(bluesAndWhites, arrayLength, 3000, 0.5, 1.0);
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
