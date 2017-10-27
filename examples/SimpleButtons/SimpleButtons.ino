/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample with push buttons, event handler alternative>
*************************************************************/

#include "Commanders.h"

#ifdef NO_BUTTONSCOMMANDER
#error To be able to compile this sample,the line #define NO_BUTTONSCOMMANDER must be commented in Commanders.h
#endif

ButtonsCommanderPush Left;
ButtonsCommanderPush Dc;
ButtonsCommanderPush Right;
ButtonsCommanderPush EPS;
ButtonsCommanderPush TJD;
	
void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	COMMANDERS_PRINT_EVENT(inId, inEvent, inData);
}

void setup()
{
	Commanders::begin(ReceiveEvent, LED_BUILTIN);

	Left.begin(4, 100);	// Arduino pin 4, id 100
	Dc.begin(6, 101);
	Right.begin(8, 102);
	EPS.begin(10, 103);
	TJD.begin(11, 104);
}

void loop()
{
	Commanders::loop();
}
