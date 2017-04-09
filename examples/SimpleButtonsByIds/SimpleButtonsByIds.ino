/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample with buttons, loop alternative>
*************************************************************/

#include "Commanders.h"

ButtonsCommanderPush Left;
ButtonsCommanderPush Dc;
ButtonsCommanderPush Right;
ButtonsCommanderPush EPS;
ButtonsCommanderPush TJD;
	
void setup()
{
	Commanders::begin(LED_BUILTIN);

	Left.begin(4, 100);		// Arduino pin 4, id 100
	Dc.begin(6, 101);
	Right.begin(8, 102);
	EPS.begin(10, 103);
	TJD.begin(11, 104);
}

void loop()
{
	unsigned long eventID = Commanders::loop();

	if (eventID != UNDEFINED_ID)
	{	
		COMMANDERS_PRINT_EVENT(eventID, Commanders::GetLastEventType(), Commanders::GetLastEventData());
	}
}
