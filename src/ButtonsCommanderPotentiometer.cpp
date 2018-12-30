/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Potentiometer returning a current value.>
*************************************************************/

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER

ButtonsCommanderPotentiometer::ButtonsCommanderPotentiometer() : ButtonsCommanderButton(UNDEFINED_ID)
{
}

void ButtonsCommanderPotentiometer::begin(unsigned long inId, int inPin, int inMinimum, int inMaximum, int inMoveAccuracy)
{
	this->Id = inId;
	this->mini = inMinimum;
	this->maxi = inMaximum;
	this->pin = inPin;
	this->moveAccuracy = inMoveAccuracy - 1;
	if (this->moveAccuracy <= 0)
		this->moveAccuracy = 1;

	digitalWrite(this->pin, HIGH);
	int val = analogRead(this->pin);
	this->currentValue = map(val, 0, ANALOG_LIMIT, this->mini, this->maxi);
}

unsigned long ButtonsCommanderPotentiometer::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->Id == UNDEFINED_ID || this->pin == DP_INVALID)
	{
		if (this->moveAccuracy != 32767) // If the error message has not been yet shown...
		{
			// use it as a debug flag !
			Serial.println(F("This potentiometer have no ID or pin defined : call begin() !"));
			this->moveAccuracy = 32767;	// The error message has been shown...
		}
	}
#endif

	if (this->Id == UNDEFINED_ID || this->pin == DP_INVALID)
	{
		return UNDEFINED_ID;
	}

	int val = analogRead(this->pin);
#ifdef COMMANDERS_DEBUG_MODE
	//Serial.print(F("Potentiometer real value : "));
	//Serial.println(val, DEC);
#endif

	val = map(val, 0, ANALOG_LIMIT, this->mini, this->maxi);

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

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderPotentiometer::printCommander()
{
	Serial.print(F("    Potentiometer - Pin:"));
	Serial.print(this->pin);
	Serial.print(F(" / Mini: "));
	Serial.print(this->mini);
	Serial.print(F(" / Maxi: "));
	Serial.print(this->maxi);
	Serial.print(F(" / Accuracy: "));
	Serial.println(this->moveAccuracy);
}
#endif
#endif
#endif
