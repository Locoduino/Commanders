/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Encoder returning a current value.>
*************************************************************/

#include "ButtonsCommanderEncoder.hpp"

ButtonsCommanderEncoder::ButtonsCommanderEncoder(unsigned long inId, int inStartingValue, int inMinimum, int inMaximum, bool inAssociatedPushButton) : ButtonsCommanderButton(inId)
{
	this->mini = inMinimum;
	this->maxi = inMaximum;
	this->currentValue = inStartingValue;

	if (inAssociatedPushButton)
	{
		this->pPush = new ButtonsCommanderPush((byte)1);
	}
	else
	{
		this->pPush = 0;
	}
}

void ButtonsCommanderEncoder::Setup(int inPin1, int inPin2, int inMoveAccuracy)
{
	this->pin1 = Arduino_to_GPIO_pin(inPin1);
	this->pin2 = Arduino_to_GPIO_pin(inPin2);
	pinMode2f(this->pin1, INPUT);
	pinMode2f(this->pin2, INPUT);
	digitalWrite2f(this->pin1, HIGH); //turn pullup resistor on
	digitalWrite2f(this->pin2, HIGH); //turn pullup resistor on	this->moveAccuracy = inMoveAccuracy - 1;
	this->lastEncoded = 0;
	if (this->moveAccuracy <= 0)
		this->moveAccuracy = 1;
}

BasicsCommanderEvent ButtonsCommanderEncoder::Loop()
{
	int MSB = digitalRead2f(this->pin1); //MSB = most significant bit
	int LSB = digitalRead2f(this->pin2); //LSB = least significant bit

	int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	if (encoded == 0)
		return EmptyEvent;
	int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

	if (sum == 13 || sum == 4 || sum == 2 || sum == 11) this->currentValue++;
	if (sum == 14 || sum == 7 || sum == 1 || sum ==  8) this->currentValue--;
	
	if (this->currentValue > this->maxi)
		this->currentValue = this->maxi;

	if (this->currentValue < this->mini)
		this->currentValue = this->mini;

	lastEncoded = encoded; //store this value for next time
	Commander::RaiseEvent(this->GetId(), COMMANDERS_EVENT_RELATIVEMOVE, this->currentValue);
	return BasicsCommanderEvent(this->GetId(), COMMANDERS_EVENT_RELATIVEMOVE, this->currentValue);
}
