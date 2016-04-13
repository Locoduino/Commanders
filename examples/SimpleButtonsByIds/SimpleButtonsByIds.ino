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
	ButtonsCommanderPush *pLeft = new ButtonsCommanderPush(102UL); //id. UL is to force an unsigned long as constant.
	ButtonsCommanderPush *pDc = new ButtonsCommanderPush(103UL);
	ButtonsCommanderPush *pRight = new ButtonsCommanderPush(104UL);
	ButtonsCommanderPush *pEPS = new ButtonsCommanderPush(105UL);
	ButtonsCommanderPush *pTJD = new ButtonsCommanderPush(106UL);

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
		Serial.print(F("Commander id found : "));
		Serial.print(eventID, DEC);
		Serial.print(F(" / "));
		
		switch (Commanders::GetLastEventType())
		{
		case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));		break;
		case COMMANDERS_EVENT_SELECTED:		Serial.println(F("SELECTED"));	break;
		case COMMANDERS_EVENT_ABSOLUTEMOVE:	
			Serial.print(F("ABSOLUTEMOVE : "));	
			Serial.println(Commanders::GetLastEventData(), DEC);
			break;
		case COMMANDERS_EVENT_RELATIVEMOVE:	
			Serial.print(F("RELATIVEMOVE : "));	
			Serial.println(Commanders::GetLastEventData(), DEC);
			break;
		}
	}
}
