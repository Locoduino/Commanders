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

CommanderEvent ButtonsCommanderKeyboard::Loop()
{
	//this->UnselectLastLoop();
	if (this->key == 0)
		return EmptyEvent;

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
		CommanderEvent ret(this->GetId(), COMMANDERS_EVENT_SELECTED, 0);
		return ret;
	}

	return EmptyEvent;
}

void ButtonsCommanderKeyboard::EndLoop()
{
}
