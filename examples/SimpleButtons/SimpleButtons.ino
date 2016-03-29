/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample>
*************************************************************/

#include "Commanders.h"

/* kDCC_INTERRUPT values :
Board			int.0	int.1	int.2	int.3	int.4	int.5
Uno, Ethernet	2		3
Mega2560		2		3		21		20		19		18
Leonardo		3		2		0		1		7
*/
#define kDCC_INTERRUPT            5

ButtonsCommander buttons;
DccCommander dcc;

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(DCCID(inId), DEC);
	Serial.print(F(" / "));
	Serial.print(DCCACTIVATION(inId), DEC);
	Serial.print(F(" / "));
	switch (inEvent)
	{
	case COMMANDERS_EVENT_NONE:		Serial.println(F("NONE"));		break;
	case COMMANDERS_EVENT_SELECTED:		Serial.println(F("SELECTED"));		break;
	case COMMANDERS_EVENT_ABSOLUTEMOVE:	
		Serial.print(F("ABSOLUTEMOVE : "));	
		Serial.println(inData, DEC);
		break;
	case COMMANDERS_EVENT_RELATIVEMOVE:	
		Serial.print(F("RELATIVEMOVE : "));	
		Serial.println(inData, DEC);
		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders_StartSetup(CommandersEventHandler);

	// Buttons setups

	buttons.Setup();
	ButtonsCommanderPush *pLeft = new ButtonsCommanderPush(DCCINT(124, 1));
	ButtonsCommanderPush *pDc = new ButtonsCommanderPush(DCCINT(126, 1));
	ButtonsCommanderPush *pRight = new ButtonsCommanderPush(DCCINT(128, 1));
	ButtonsCommanderPush *pEPS = new ButtonsCommanderPush(DCCINT(130, 1));
	ButtonsCommanderPush *pTJD = new ButtonsCommanderPush(DCCINT(132, 1));

	pLeft->Setup(24);
	pDc->Setup(26);
	pRight->Setup(28);
	pEPS->Setup(30);
	pTJD->Setup(32);

	buttons.Add(pLeft);
	buttons.Add(pDc);
	buttons.Add(pRight);
	buttons.Add(pEPS);
	buttons.Add(pTJD);

#ifdef VISUALSTUDIO
	ButtonsCommanderKeyboard *pKeyA = new ButtonsCommanderKeyboard(100UL);
	ButtonsCommanderKeyboard *pKeyB = new ButtonsCommanderKeyboard(200UL);
	pKeyA->Setup('A');
	pKeyB->Setup('B');
	buttons.Add(pKeyA);
	buttons.Add(pKeyB);
#endif

	// Accessories setups

	dcc.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dcc.SetStatusLedPin(13);

	Commanders_EndSetup();
}

void loop()
{
	Commanders_Loop();
}
