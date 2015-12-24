// UAD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "windows.h"
#include "ButtonsCommanderKeyboard.hpp"

void setup();
void loop();

int _tmain(int argc, _TCHAR* argv[])
{
	setup();

	DWORD        mode;
	HANDLE       hstdin;
	INPUT_RECORD inrec;
	DWORD        count;

	/* Set the console mode to no-echo, raw input, */
	/* and no window or mouse events.              */
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hstdin == INVALID_HANDLE_VALUE
		|| !GetConsoleMode(hstdin, &mode)
		|| !SetConsoleMode(hstdin, 0))
		return 0;

	while (true)
	{
		FlushConsoleInputBuffer(hstdin);

		/* Get a single key RELEASE */
		ReadConsoleInput(hstdin, &inrec, 1, &count);

		if (inrec.EventType == KEY_EVENT && inrec.Event.KeyEvent.bKeyDown)
			ButtonsCommanderKeyboard::lastLoopKey = inrec.Event.KeyEvent.wVirtualKeyCode;

		loop();
	}

	/* Restore the original console mode */
	SetConsoleMode(hstdin, mode);

	return 0;
}

//-----------------------------------------
//			ino PART
//-----------------------------------------
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
	case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));			break;
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

	ButtonsCommanderKeyboard *pKeyA = new ButtonsCommanderKeyboard(100UL);
	ButtonsCommanderKeyboard *pKeyB = new ButtonsCommanderKeyboard(200UL);
	pKeyA->Setup('A');
	pKeyB->Setup('B');
	buttons.Add(pKeyA);
	buttons.Add(pKeyB);

	// Accessories setups

	dcc.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dcc.SetStatusLedPin(13);

	BasicsCommanders_EndSetup();
}

void loop()
{
	BasicsCommanders_Loop();
}

//-----------------------------------------
//			end ino PART
//-----------------------------------------
