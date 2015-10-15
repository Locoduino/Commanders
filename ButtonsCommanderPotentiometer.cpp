/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Potentiometer returning a current value.>
*************************************************************/

//#include "UniversalAccessoryDecoder.h"
#include "ButtonsCommanderPotentiometer.hpp"

ButtonsCommanderPotentiometer::ButtonsCommanderPotentiometer(unsigned long inId, int inMinimum, int inMaximum) : ButtonsCommanderButton(inId)
{
	this->mini = inMinimum;
	this->maxi = inMaximum;
}

void ButtonsCommanderPotentiometer::Setup(int inPin, int inMoveAccuracy)
{
	this->pin = inPin;
	this->moveAccuracy = inMoveAccuracy - 1;
	if (this->moveAccuracy <= 0)
		this->moveAccuracy = 1;

	int val = analogRead(pin);
	this->currentValue = map(val, 0, 1023, this->mini, this->maxi);
}

unsigned long ButtonsCommanderPotentiometer::Loop()
{
	int val = analogRead(pin);

	val = map(val, 0, 1023, this->mini, this->maxi);

	if (val < this->currentValue - this->moveAccuracy || val > this->currentValue + this->moveAccuracy)
	{
#ifdef DEBUG_MODE
		Serial.print(F("Potentiometer new value : "));
		Serial.println(val, DEC);
#endif
		this->currentValue = val;
		return this->GetId();
	}

	return UNDEFINED_ID;
}
