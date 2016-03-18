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
	Commanders_StartSetup();

	// Buttons setups

	buttons.Setup();
	ButtonsCommanderPush *pLeft = new ButtonsCommanderPush(102UL); //id. UL is to force an unsigned long as constant.
	ButtonsCommanderPush *pDc = new ButtonsCommanderPush(103UL);
	ButtonsCommanderPush *pRight = new ButtonsCommanderPush(104UL);
	ButtonsCommanderPush *pEPS = new ButtonsCommanderPush(105UL);
	ButtonsCommanderPush *pTJD = new ButtonsCommanderPush(106UL);

	pLeft->Setup(24);  // pin number
	pDc->Setup(26);
	pRight->Setup(28);
	pEPS->Setup(30);
	pTJD->Setup(32);

	buttons.Add(pLeft);
	buttons.Add(pDc);
	buttons.Add(pRight);
	buttons.Add(pEPS);
	buttons.Add(pTJD);

	Commanders_EndSetup();
}

void loop()
{
	CommanderEvent event = BasicsCommanders_Loop();

	if (event.ID != UNDEFINED_ID)
	{	
		Serial.print(F("Commander id found : "));
		Serial.print(event.ID, DEC);
		Serial.print(F(" / "));
		
		switch (event.Event)
		{
		case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));		break;
		case COMMANDERS_EVENT_SELECTED:		Serial.println(F("SELECTED"));	break;
		case COMMANDERS_EVENT_ABSOLUTEMOVE:	
			Serial.print(F("ABSOLUTEMOVE : "));	
			Serial.println(event.Data, DEC);
			break;
		case COMMANDERS_EVENT_RELATIVEMOVE:	
			Serial.print(F("RELATIVEMOVE : "));	
			Serial.println(event.Data, DEC);
			break;
		}
	}
}
