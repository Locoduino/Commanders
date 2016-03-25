/*************************************************************
project: <Dc/Dcc Controler>
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

unsigned long ButtonsCommanderKeyboard::Loop()
{
	//this->UnselectLastLoop();
	if (this->key == 0)
		return UNDEFINED_ID;

	if (lastLoopKey == this->key)
	{
		//this->SelectLastLoop();
		lastLoopKey = 0;
#ifdef DEBUG_MODE
		Serial.print(F("Keyboard "));
		char str[3];
		str[0] = key;
		str[1] = 0;
		Serial.print(str);
		Serial.println(F(" pressed "));
#endif
		Commander::RaiseEvent(this->GetId(), COMMANDERS_EVENT_SELECTED, 0);
		eventType = COMMANDERS_EVENT_SELECTED;
		eventData = 0;
		return this->GetId();
	}

	return UNDEFINED_ID;
}

void ButtonsCommanderKeyboard::EndLoop()
{
}
