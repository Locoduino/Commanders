/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Switch button with debounce.>
*************************************************************/

//#include "UniversalAccessoryDecoder.h"
#include "ButtonsCommanderSwitch.hpp"

ButtonsCommanderSwitch::ButtonsCommanderSwitch(int inIdNumber) : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->IdSize = inIdNumber;
	this->pId = new IdPin[this->IdSize];
	this->IdAddCounter = 0;
	this->IdLoopCounter = 0;

	this->debounceDelay = 50;
}

void ButtonsCommanderSwitch::Setup()
{
	for (int i = 0; i < this->IdAddCounter; i++)
	{
		IdPin *id = &(this->pId[i]);
		if (id->Pin == DP_INVALID)
			continue;

		pinMode2f(id->Pin, INPUT_PULLUP);
	}
}

// Returns the index of the new added position.
void ButtonsCommanderSwitch::AddId(unsigned long inId, int inPin)
{
#ifdef DEBUG_MODE
	//Accessory::CHECKDCC(inDccId, inDccIdAccessory, "Accessory constructor");
#endif

	CHECKPIN(inPin, "ButtonsCommanderSwitch::AddDccId");

#ifdef DEBUG_MODE
	if (this->IdAddCounter == this->IdSize)
	{
		Serial.println(F("ButtonsCommanderSwitch::AddId : Too many Id for this switch !"));
		return;
	}
#endif

	this->pId[this->IdAddCounter].Id = inId;
	this->pId[this->IdAddCounter].Pin = Arduino_to_GPIO_pin(inPin);
	this->pId[this->IdAddCounter].buttonState = 0;
	this->pId[this->IdAddCounter].lastButtonState = LOW;
	this->pId[this->IdAddCounter++].lastDebounceTime = 0;

	pinMode2f(this->pId[this->IdAddCounter].Pin, INPUT_PULLUP);
}

unsigned long ButtonsCommanderSwitch::Loop()
{
	if (this->IdLoopCounter >= this->IdAddCounter)
		this->IdLoopCounter = 0;

	IdPin *id = &(this->pId[this->IdLoopCounter++]);
	if (id->Pin == DP_INVALID)
		return false;

	// read the state of the switch into a local variable:
	int reading = digitalRead2f(id->Pin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != id->lastButtonState)
	{
		// reset the debouncing timer
		id->lastDebounceTime = millis();
	}

	bool haveFound = false;

	if (id->lastDebounceTime > 0 && (millis() - id->lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != id->buttonState)
		{
			id->buttonState = reading;

			// only toggle the state if the new button state is HIGH
			if (id->buttonState == HIGH)
			{
				this->IdState = this->IdLoopCounter-1;
				haveFound = true;
			}
		}
		id->lastDebounceTime = 0;
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	id->lastButtonState = reading;

	return haveFound;
}
