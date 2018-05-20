/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample with buttons, loop alternative>
*************************************************************/

#ifdef NO_BUTTONSCOMMANDER
#error To be able to compile this sample,the line #define NO_BUTTONSCOMMANDER must be commented in Commanders.h
#endif

#if defined(ARDUINO_ARCH_ESP32)
#define LED_BUILTIN 2
#endif

#include "Commanders.h"

ButtonsCommanderPush Left;
ButtonsCommanderPush Dc;
ButtonsCommanderPush Right;
ButtonsCommanderPush pushEPS;
ButtonsCommanderPush pushTJD;
	
void setup()
{
	Commanders::begin(LED_BUILTIN);

	Left.begin(100, 4);		// Arduino pin 4, id 100
	Dc.begin(101, 6);
	Right.begin(102, 8);
	pushEPS.begin(103, 10);
	pushTJD.begin(104, 11);
}

void loop()
{
	unsigned long eventID = Commanders::loop();

	if (eventID != UNDEFINED_ID)
	{	
		COMMANDERS_PRINT_EVENT(eventID, Commanders::GetLastEventType(), Commanders::GetLastEventData());
	}
}
