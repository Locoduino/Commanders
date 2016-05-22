/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Encoder returning a current value, or a moving direction.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER

ButtonsCommanderEncoder::ButtonsCommanderEncoder() : ButtonsCommanderButton(UNDEFINED_ID)
{
}

void ButtonsCommanderEncoder::begin(int inPin1, int inPin2, unsigned long inId, int inStartingValue, int inMinimum, int inMaximum)
{
	this->Id = inId;

	this->pin1 = Arduino_to_GPIO_pin(inPin1);
	this->pin2 = Arduino_to_GPIO_pin(inPin2);
	pinMode2f(this->pin1, INPUT);
	pinMode2f(this->pin2, INPUT);
	digitalWrite2f(this->pin1, HIGH); //turn pullup resistor on
	digitalWrite2f(this->pin2, HIGH); //turn pullup resistor on

	this->lastEncoded = 0;
	this->mini = inMinimum;
	this->maxi = inMaximum;
	this->currentValue = inStartingValue;
	this->startingCurrentValue = inStartingValue;
}

unsigned long ButtonsCommanderEncoder::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->Id == UNDEFINED_ID)
		Serial.println(F("This encoder have no ID defined : call begin() !"));
#endif

	int MSB = digitalRead2f(this->pin1); //MSB = most significant bit
	int LSB = digitalRead2f(this->pin2); //LSB = least significant bit

	int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	if (encoded == 0)
		return UNDEFINED_ID;
	int sum = (this->lastEncoded << 2) | encoded; //adding it to the previous encoded value

	char inc = 0;
	if (sum == 13 || sum == 4 || sum == 2 || sum == 11) inc = 1;
	if (sum == 14 || sum == 7 || sum == 1 || sum ==  8) inc = -1;
	
	this->lastEncoded = encoded; //store this value for next time

	if (this->mini != this->maxi)
	{
		// If the encoder has been defined with a mini/maxi interval,
		// move the value and return it !
		this->currentValue += inc;

		if (this->currentValue > this->maxi)
			this->currentValue = this->maxi;

		if (this->currentValue < this->mini)
			this->currentValue = this->mini;

		return Commander::RaiseEvent(this->GetId(), COMMANDERS_EVENT_MOVEPOSITION, this->currentValue);
	}

	// if no interval defined, just return the move direction.
	return Commander::RaiseEvent(this->GetId(), 
									COMMANDERS_EVENT_MOVE, 
									inc > 0 ? COMMANDERS_MOVE_MORE : COMMANDERS_MOVE_LEFT);
}
#endif