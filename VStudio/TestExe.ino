/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample with push buttons, event handler alternative>
*************************************************************/

#include "Commanders.h"

#ifdef NO_BUTTONSCOMMANDER
#error To be able to compile this sample,the line #define NO_BUTTONSCOMMANDER must be commented in Commanders.h
#endif

#if defined(ARDUINO_ARCH_ESP32)
#define LED_BUILTIN 2
#endif

ButtonsCommanderPush Left;
ButtonsCommanderPush Dc;
ButtonsCommanderPush Right;
ButtonsCommanderPush pushEPS;
ButtonsCommanderPush pushTJD;

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	//COMMANDERS_PRINT_EVENT(inId, inEvent, inData);
}

void setup()
{
	Commanders::begin(ReceiveEvent, LED_BUILTIN);

	Left.begin(100, 4);	// Arduino pin 4, id 100
	Dc.begin(101, 6);
	Right.begin(102, 8);
	pushEPS.begin(103, 10);
	pushTJD.begin(104, 11);
}

void loop()
{
	Commanders::loop();
}
