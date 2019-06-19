/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Encoder returning a current value, or a moving direction.>
*************************************************************/

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERENCODER

ButtonsCommanderEncoder::ButtonsCommanderEncoder() : ButtonsCommanderButton(UNDEFINED_ID)
{
}

void ButtonsCommanderEncoder::begin(unsigned long inId, int inPin1, int inPin2, byte inMoveIncrement, int inStartingValue, int inMinimum, int inMaximum)
{
	this->Id = inId;

	this->pin1 = Arduino_to_GPIO_pin(inPin1);
	this->pin2 = Arduino_to_GPIO_pin(inPin2);
	pinMode2f(this->pin1, INPUT_PULLUP);
	pinMode2f(this->pin2, INPUT_PULLUP);
	//digitalWrite2f(this->pin1, HIGH); //turn pullup resistor on
	//digitalWrite2f(this->pin2, HIGH); //turn pullup resistor on

	this->moveIncrement = inMoveIncrement;
#ifdef VISUALSTUDIO
	this->moveIncrement = 1;	// easier to test...
#endif
	this->incrementPosition = 0;
	this->lastEncoded = 0;
	this->mini = inMinimum;
	this->maxi = inMaximum;
	this->currentValue = inStartingValue;
	this->startingCurrentValue = inStartingValue;
}

unsigned long ButtonsCommanderEncoder::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->Id == UNDEFINED_ID || this->pin1 == DP_INVALID || this->pin2 == DP_INVALID)
	{
		if (this->lastEncoded != 32767) // If the error message has not been yet shown...
		{
			Serial.println(F("This encoder have no ID or pins defined : call begin() !"));
			// use it as a debug flag !
			this->lastEncoded = 32767;	// The error message has been shown...
		}
	}
#endif

	if (this->Id == UNDEFINED_ID || this->pin1 == DP_INVALID || this->pin2 == DP_INVALID)
	{
		return UNDEFINED_ID;
	}

	int MSB = digitalRead2f(this->pin1); //MSB = most significant bit
	int LSB = digitalRead2f(this->pin2); //LSB = least significant bit

	int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	if (encoded == 0)
		return UNDEFINED_ID;
	int sum = (this->lastEncoded << 2) | encoded; //adding it to the previous encoded value

	int8_t inc = 0;
	if (sum == 13 || sum == 4 || sum == 2 || sum == 11) inc = 1;
	if (sum == 14 || sum == 7 || sum == 1 || sum ==  8) inc = -1;
	
#ifdef COMMANDERS_DEBUG_MODE
	if (inc != 0)
	{
		Serial.print(F("Encoder move of "));
		Serial.print(inc);
		Serial.print(F(" : "));
		Serial.println(this->currentValue+inc);
	}
#endif

	this->lastEncoded = encoded; //store this value for next time

	if (inc == 0)
		return UNDEFINED_ID;

	if (this->moveIncrement > 1)
	{
		this->incrementPosition += inc;
		if (abs(this->incrementPosition) < this->moveIncrement)
		{
			return UNDEFINED_ID;	// needs to move more to obtain a position change...
		}

		this->incrementPosition = 0;	// moves have been made enough to change position !
	}

	if (this->mini != this->maxi)
	{
		// If the encoder has been defined with a mini/maxi interval,
		// move the value and return it !
		this->currentValue += inc;

		if (this->currentValue > this->maxi)
			this->currentValue = this->maxi;

		if (this->currentValue < this->mini)
			this->currentValue = this->mini;

		return Commanders::RaiseEvent(this->GetId(), COMMANDERS_EVENT_MOVEPOSITION, this->currentValue);
	}

	// if no interval defined, just return the move direction.
	return Commanders::RaiseEvent(this->GetId(), 
									COMMANDERS_EVENT_MOVE, 
									inc > 0 ? COMMANDERS_MOVE_MORE : COMMANDERS_MOVE_LESS);
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderEncoder::printCommander()
{
	Serial.print(F("    Encoder - Pin1:"));
	Serial.print(GPIO_to_Arduino_pin(this->pin1));
	Serial.print(F(" / Pin2: "));
	Serial.print(GPIO_to_Arduino_pin(this->pin2));
	Serial.print(F(" / Mini: "));
	Serial.print(this->maxi);
	Serial.print(F(" / Maxi: "));
	Serial.println(this->maxi);
}
#endif
#endif
#endif
