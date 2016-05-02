/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample>
*************************************************************/

#include "Commanders.h"

ButtonsCommander buttons;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::StartSetup();

	// Buttons setups

	buttons.begin();
	ButtonsCommanderPush *pLeft = new ButtonsCommanderPush(102);
	ButtonsCommanderPush *pDc = new ButtonsCommanderPush(103);
	ButtonsCommanderPush *pRight = new ButtonsCommanderPush(104);
	ButtonsCommanderPush *pEPS = new ButtonsCommanderPush(105);
	ButtonsCommanderPush *pTJD = new ButtonsCommanderPush(106);

	pLeft->begin(24);  // pin number
	pDc->begin(26);
	pRight->begin(28);
	pEPS->begin(30);
	pTJD->begin(32);

	buttons.Add(pLeft);
	buttons.Add(pDc);
	buttons.Add(pRight);
	buttons.Add(pEPS);
	buttons.Add(pTJD);

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
