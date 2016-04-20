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
project: <Commanders>
author: <Thierry PARIS>
description: <I2C commander demo>
*************************************************************/

#include "Commanders.h"

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(inId, DEC);
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

void setup()
{
	COMMANDERS_SET_STATUSLED(13);
	COMMANDERS_SET_EVENTHANDLER(CommandersEventHandler);

	END_COMMANDERS_SETUP;

	TextInterpreter TI;

	TI.SendString("127/a/200");
}

void loop()
{
	COMMANDERS_LOOP;
}

//-----------------------------------------
//			end ino PART
//-----------------------------------------
