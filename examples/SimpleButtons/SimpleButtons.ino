/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample with push buttons>
*************************************************************/

#include "Commanders.h"

ButtonsCommanderPush Left;
ButtonsCommanderPush Dc;
ButtonsCommanderPush Right;
ButtonsCommanderPush EPS;
ButtonsCommanderPush TJD;
	
void ReceiveHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
}

void setup()
{
	Commanders::StartSetup(ReceiveHandler, LED_BUILTIN);

	Left.begin(24, 100);
	Dc.begin(26, 101);
	Right.begin(28, 102);
	EPS.begin(30, 103);
	TJD.begin(32, 104);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
