/*************************************************************
project: <Basics Commanders>
author: <Thierry PARIS>
description: <Demo sample>
*************************************************************/

#include "BasicsCommanders.h"

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
	Serial.print(DCCACCESSORY(inId), DEC);
	Serial.print(F(" / "));
	switch (inEvent)
	{
	case EVENT_NONE:		Serial.println(F("NONE"));		break;
	case EVENT_SELECTED:		Serial.println(F("SELECTED"));		break;
	case EVENT_ABSOLUTEMOVE:	
		Serial.print(F("ABSOLUTEMOVE : "));	
		Serial.println(inData, DEC);
		break;
	case EVENT_RELATIVEMOVE:	
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
	BasicsCommanders_StartSetup(CommandersEventHandler);

	// Buttons setups

	buttons.Setup();
	ButtonsCommanderPush *pLeft = new ButtonsCommanderPush(20UL);
	ButtonsCommanderPush *pDc = new ButtonsCommanderPush(21UL);
	ButtonsCommanderPush *pRight = new ButtonsCommanderPush(22UL);
	ButtonsCommanderPush *pEPS = new ButtonsCommanderPush(23UL);
	ButtonsCommanderPush *pTJD = new ButtonsCommanderPush(24UL);

	pLeft->Setup(30);
	pDc->Setup(34);
	pRight->Setup(32);
	pEPS->Setup(36);
	pTJD->Setup(38);

	buttons.Add(pLeft);
	buttons.Add(pDc);
	buttons.Add(pRight);
	buttons.Add(pEPS);
	buttons.Add(pTJD);

#ifdef VISUALC
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

	BasicsCommanders_EndSetup();
}

void loop()
{
	BasicsCommanders_Loop();
}