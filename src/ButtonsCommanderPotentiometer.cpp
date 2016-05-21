/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Potentiometer returning a current value.>
*************************************************************/

#include "ButtonsCommanderPotentiometer.hpp"

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

	int val = analogRead(pin);
	this->currentValue = map(val, 0, 1023, this->mini, this->maxi);
}

unsigned long ButtonsCommanderPotentiometer::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->Id == UNDEFINED_ID)
		Serial.println(F("This potentiometer have no ID defined : call begin() !"));
#endif

	int val = analogRead(pin);

	val = map(val, 0, 1023, this->mini, this->maxi);

	if (val < this->currentValue - this->moveAccuracy || val > this->currentValue + this->moveAccuracy)
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("Potentiometer new value : "));
		Serial.println(val, DEC);
#endif
		this->currentValue = val;
		eventType = COMMANDERS_EVENT_ABSOLUTEMOVE;
		eventData = val;
		Commander::RaiseEvent(this->GetId(), COMMANDERS_EVENT_ABSOLUTEMOVE, val);
		return this->GetId();
	}

	return UNDEFINED_ID;
}
