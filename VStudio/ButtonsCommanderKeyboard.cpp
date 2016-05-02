/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Keyboard button.>
*************************************************************/

#include "ButtonsCommanderKeyboard.hpp"

#include "conio.h"

int ButtonsCommanderKeyboard::lastLoopKey;

ButtonsCommanderKeyboard::ButtonsCommanderKeyboard(unsigned long inId) : ButtonsCommanderButton(inId)
{
	this->key = 0;
}

unsigned long ButtonsCommanderKeyboard::loop()
{
	//this->UnselectLastLoop();
	if (this->key == 0)
		return UNDEFINED_ID;

	if (lastLoopKey == this->key)
	{
		//this->SelectLastLoop();
		lastLoopKey = 0;
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("Keyboard "));
		char str[3];
		str[0] = key;
		str[1] = 0;
		Serial.print(str);
		Serial.println(F(" pressed "));
#endif
		Commander::RaiseEvent(this->GetId(), COMMANDERS_EVENT_TOGGLE, 0);
		eventType = COMMANDERS_EVENT_TOGGLE;
		eventData = 0;
		return this->GetId();
	}

	return UNDEFINED_ID;
}

void ButtonsCommanderKeyboard::EndLoop()
{
}
