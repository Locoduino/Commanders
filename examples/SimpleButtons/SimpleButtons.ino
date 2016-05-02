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
	DccCommander::printEvent(inId, inEvent, inData);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::StartSetup(CommandersEventHandler);

	// Buttons setups

	buttons.begin();
	ButtonsCommanderPush *pLeft = new ButtonsCommanderPush(DCCINT(124, 1));
	ButtonsCommanderPush *pDc = new ButtonsCommanderPush(DCCINT(126, 1));
	ButtonsCommanderPush *pRight = new ButtonsCommanderPush(DCCINT(128, 1));
	ButtonsCommanderPush *pEPS = new ButtonsCommanderPush(DCCINT(130, 1));
	ButtonsCommanderPush *pTJD = new ButtonsCommanderPush(DCCINT(132, 1));

	pLeft->begin(24);
	pDc->begin(26);
	pRight->begin(28);
	pEPS->begin(30);
	pTJD->begin(32);

	buttons.Add(pLeft);
	buttons.Add(pDc);
	buttons.Add(pRight);
	buttons.Add(pEPS);
	buttons.Add(pTJD);

#ifdef VISUALSTUDIO
	ButtonsCommanderKeyboard *pKeyA = new ButtonsCommanderKeyboard(100UL);
	ButtonsCommanderKeyboard *pKeyB = new ButtonsCommanderKeyboard(200UL);
	pKeyA->begin('A');
	pKeyB->begin('B');
	buttons.Add(pKeyA);
	buttons.Add(pKeyB);
#endif

	// Accessories setups

	dcc.begin(0x00, 0x00, kDCC_INTERRUPT);
	Commanders::SetStatusLedPin(13);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
