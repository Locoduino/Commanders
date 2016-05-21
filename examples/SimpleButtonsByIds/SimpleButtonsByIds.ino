/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample with buttons>
*************************************************************/

#include "Commanders.h"

ButtonsCommanderPush Left;
ButtonsCommanderPush Dc;
ButtonsCommanderPush Right;
ButtonsCommanderPush EPS;
ButtonsCommanderPush TJD;
	
void setup()
{
	Commanders::StartSetup();
	Commanders::SetStatusLedPin(LED_BUILTIN);

	Left.begin(24, 100);
	Dc.begin(26, 101);
	Right.begin(28, 102);
	EPS.begin(30, 103);
	TJD.begin(32, 104);

	Commanders::EndSetup();
}

void loop()
{
	unsigned long eventID = Commanders::loop();

	if (eventID != UNDEFINED_ID)
	{	
		Commanders::printEvent(eventID, Commanders::GetLastEventType(), Commanders::GetLastEventData());
	}
}
