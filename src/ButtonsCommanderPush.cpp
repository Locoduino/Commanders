/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Basic push button with debounce.>
*************************************************************/

#include "ButtonsCommanderPush.hpp"

ButtonsCommanderPush ButtonsCommanderPush::Empty = ButtonsCommanderPush(UNDEFINED_ID);

ButtonsCommanderPush::ButtonsCommanderPush(byte inIdNumber) : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->buttonPin = (GPIO_pin_t)DP_INVALID;
	this->lastButtonState = LOW;
	this->IdSize = inIdNumber;
	this->pId = new unsigned long[this->IdSize];
	this->IdAddCounter = 0;
	this->IdLoopCounter = 0;

	this->lastDebounceTime = 0;
	this->debounceDelay = 50;
}

ButtonsCommanderPush::ButtonsCommanderPush(unsigned long inId) : ButtonsCommanderPush((byte)1)
{
	this->AddId(inId);
}

void ButtonsCommanderPush::Setup(int inButtonPin)
{	
	CHECKPIN(inButtonPin, "ButtonsCommanderPush::Setup");

	this->buttonPin = Arduino_to_GPIO_pin(inButtonPin);

	pinMode2f(this->buttonPin, INPUT_PULLUP);
}

// Returns the index of the new added position.
void ButtonsCommanderPush::AddId(unsigned long inId)
{
#ifdef DEBUG_MODE
	//Accessory::CHECKDCC(inDccId, inDccIdAccessory, "Accessory constructor");
#endif

#ifdef DEBUG_MODE
	if (this->IdAddCounter == this->IdSize)
	{
		Serial.println(F("ButtonsCommanderPush::AddId : Too many Ids for this push button !"));
		return;
	}
#endif

	this->pId[this->IdAddCounter] = inId;
}

unsigned long ButtonsCommanderPush::Loop()
{
	if (this->buttonPin == DP_INVALID)
		return UNDEFINED_ID;

	unsigned long haveChanged = UNDEFINED_ID;
	
	// read the state of the switch into a local variable:
	int reading = digitalRead2f(this->buttonPin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != this->lastButtonState)
	{
		// reset the debouncing timer
		this->lastDebounceTime = millis();
	}

	if (this->lastDebounceTime > 0 && (millis() - this->lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->buttonState)
		{
			this->buttonState = reading;

			// only toggle the state if the new button state is HIGH
			if (this->buttonState == LOW)
			{
				haveChanged = this->GetId();
				Commander::EventHandler(this->GetId(), COMMANDERS_EVENT_SELECTED, 0);
			}
		}
		this->lastDebounceTime = 0;    
	}
  
	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	lastButtonState = reading;
	return haveChanged;
}

void ButtonsCommanderPush::EndLoop()
{
	this->IdLoopCounter++;
	if (this->IdLoopCounter >= this->IdAddCounter)
		this->IdLoopCounter = 0;
}
