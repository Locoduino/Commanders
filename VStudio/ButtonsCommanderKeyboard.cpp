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

void ButtonsCommanderKeyboard::begin(unsigned long inId, int inKey, COMMANDERS_EVENT_TYPE inEventType, int inData)
{
	this->Id = inId; 
	this->key = inKey; 
	ArduiEmulator::ArduinoForm::lastKeyPressed = 0;

	if (inId != UNDEFINED_ID)
		this->AddEvent(inId, inEventType, inData);
}

// Returns the index of the new added position.
void ButtonsCommanderKeyboard::AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inData)
{
	KeybdEvent *pEvent = new KeybdEvent();
	pEvent->Id = inId;
	pEvent->EventType = inEventType;
	pEvent->Data = inData;
	this->Events.AddItem(pEvent);
}

unsigned long ButtonsCommanderKeyboard::loop()
{
	unsigned long foundID = UNDEFINED_ID;

#ifdef COMMANDERS_DEBUG_MODE
	if (this->Events.pFirst == NULL)
		Serial.println(F("This keyboard button have no ID defined : call begin() !"));
#endif
	//this->UnselectLastLoop();
	if (this->key == 0)
		return foundID;

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
		foundID = this->Events.pCurrentItem->Obj->Id;
		Commanders::RaiseEvent(foundID,
			this->Events.pCurrentItem->Obj->EventType,
			this->Events.pCurrentItem->Obj->Data);

		this->Events.NextCurrent();
	}

	return foundID;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderKeyboard::printCommander()
{
	Serial.print(F("    Keyboard - Key:"));
	Serial.println(this->key);

	CMDRSCHAINEDLISTITEM<KeybdEvent> *pCurr = this->Events.pFirst;
	while (pCurr != NULL)
	{
		Serial.print(F("        Event Id: "));
		Serial.print(pCurr->Obj->Id);
		Serial.print(F(" / Event type: "));
		Commanders::printEventType(pCurr->Obj->EventType, true);
		Commanders::printEventData(pCurr->Obj->EventType, pCurr->Obj->Data);
		Serial.println(F(""));

		pCurr = pCurr->pNext;
	}
}
#endif
#endif