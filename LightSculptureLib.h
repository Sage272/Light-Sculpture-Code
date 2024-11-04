#pragma once

#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Fading Functions                                                  //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void delayUsingKnob(int duration);

void fadeUp(const byte pin, int duration);
void fadeUpAll(int duration);
void fadeUpArray(const byte arrayName[], const byte arrayLength, int duration);
void fadeUpArrayInOrder(const byte arrayName[], const byte arrayLength, int duration);

void fadeDown(const byte pin, int duration);
void fadeDownAll(int duration);
void fadeDownArray(const byte arrayName[], const byte arrayLength, int duration);
void fadeDownArrayInOrder(const byte arrayName[], const byte arrayLength, int duration);

void blinkArrayInOrder(const byte arrayName[], const byte arrayLength, int duration);
void blinkTwoArraysInOrder(const byte array1[],const byte array2[], const byte arrayLength, int duration);

void fadeUpTwoArraysInOrder(const byte array1[],const byte array2[], const byte arrayLength, int duration);
void fadeDownTwoArraysInOrder(const byte array1[],const byte array2[], const byte arrayLength, int duration);

void crossFade(const byte downPin, const byte upPin, int duration);
void crossFadeArrays(const byte downArrayName[], const byte downArrayLength, const byte upArrayName[], const byte upArrayLength, int duration);

void fadeUpPartial(const byte pin, int duration, int delayNum, float fractionFrom, float fractionTo);
void fadeDownPartial(const byte pin, int duration, int delayNum, float fractionFrom, float fractionTo);

void fadeUpArrayPartial(const byte arrayName[], const byte arrayLength, int duration, float fractionFrom, float fractionTo);
void fadeDownArrayPartial(const byte arrayName[], const byte arrayLength, int duration, float fractionFrom, float fractionTo);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                   Mode Selection                                                   //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setupLightSculpture();
void updateMode();
void updateOffSwitch();
void runNightLight();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Helper Functions                                                  //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getFadingStepSize(int duration);
void pwm(float fractionOn);
void pwm(const byte pin, float fractionOn);
void pwm(const byte arrayName[], const byte arrayLength, float fractionOn);
int getOnTimeUsingFraction(float fractionOn);
void updateFadingStepSize(int duration);
//void updateStepSize(int duration);
int getOnTimeUsingFraction(float fractionOn);
void turnOnArray(const byte arrayName[], const byte arrayLength);
void turnOffArray(const byte arrayName[], const byte arrayLength);
void turnOnAll();
void turnOffAll();
void updateKnobValue();
int findMode();
void setPrevMode();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                   Mode Selection                                                   //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setupLightSculpture();
//void runNightLight();
void nightLight();
void updateKnobValue();
