/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Potentiometer returning a current value.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER

ButtonsCommanderPotentiometer::ButtonsCommanderPotentiometer() : ButtonsCommanderButton(UNDEFINED_ID)
{
}

void ButtonsCommanderPotentiometer::begin(int inPin, unsigned long inId, int inMinimum, int inMaximum, int inMoveAccuracy)
{
	this->Id = inId;
	this->mini = inMinimum;
	this->maxi = inMaximum;
	this->pin = inPin;
	this->moveAccuracy = inMoveAccuracy - 1;
	if (this->moveAccuracy <= 0)
		this->moveAccuracy = 1;

	int val = analogRead(this->pin);
	this->currentValue = map(val, 0, 1023, this->mini, this->maxi);
}

unsigned long ButtonsCommanderPotentiometer::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->Id == UNDEFINED_ID)
		Serial.println(F("This potentiometer have no ID defined : call begin() !"));
#endif

	int val = analogRead(this->pin);

	val = map(val, 0, 1023, this->mini, this->maxi);

	if (val < this->currentValue - this->moveAccuracy || val > this->currentValue + this->moveAccuracy)
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("Potentiometer new value : "));
		Serial.println(val, DEC);
#endif
		this->currentValue = val;
		return Commanders::RaiseEvent(this->GetId(), COMMANDERS_EVENT_MOVEPOSITION, val);
	}

	return UNDEFINED_ID;
}
#endif
#endif
