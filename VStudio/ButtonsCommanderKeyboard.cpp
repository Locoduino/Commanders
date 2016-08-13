/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Keyboard button.>
*************************************************************/

#include "../src/Commanders.h"
#include "ButtonsCommanderKeyboard.hpp"

#ifndef NO_BUTTONSCOMMANDER

#include "conio.h"

ButtonsCommanderKeyboard::ButtonsCommanderKeyboard() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->key = 0;
	ArduiEmulator::ArduinoForm::lastKeyPressed = 0;
}

unsigned long ButtonsCommanderKeyboard::loop()
{
	//this->UnselectLastLoop();
	if (this->key == 0)
		return UNDEFINED_ID;

	if (ArduiEmulator::ArduinoForm::lastKeyPressed == this->key)
	{
		//this->SelectLastLoop();
		ArduiEmulator::ArduinoForm::lastKeyPressed = 0;
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("Keyboard "));
		char str[3];
		str[0] = key;
		str[1] = 0;
		Serial.print(str);
		Serial.println(F(" pressed "));
#endif
		return Commanders::RaiseEvent(this->GetId(), COMMANDERS_EVENT_TOGGLE, 0);
	}

	return UNDEFINED_ID;
}

void ButtonsCommanderKeyboard::EndLoop()
{
}
#endif