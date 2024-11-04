/** @file LightSculpture.h */
/* Light Sculpture Architecture - Written by Kyle Stewart - Updated 04302018*/

#pragma once

#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                  Fading Functions                                                  //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void delayLightCheck(int duration);

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

void pwm(float fractionOn);
void pwm(const byte pin, float fractionOn);
void pwm(const byte arrayName[], const byte arrayLength, float fractionOn);
int getOnTimeUsingFraction(float fractionOn);
void updateStepSize(int duration);
int getOnTimeUsingFraction(float fractionOn);
void turnOnArray(const byte arrayName[], const byte arrayLength);
void turnOffArray(const byte arrayName[], const byte arrayLength);
void turnOnAll();
void turnOffAll();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                   Mode Selection                                                   //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setupLightSculpture();
void runNightLight();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                                   Knob Functions                                                   //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pwmUsingKnob();
void updateKnobValue();
void updateOffSwitch();
int getOnTimeUsingKnob();
float gammaCorrect(float ratio);
void updateMode();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
//                                        Global Variable Declarations (Nouns)                                        //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum Modes {offMode, lightShowMode, nightLightMode};
extern Modes mode;
