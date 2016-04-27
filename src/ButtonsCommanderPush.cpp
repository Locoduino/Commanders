/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Push button with debounce.>
*************************************************************/

#include "ButtonsCommanderPush.hpp"

ButtonsCommanderPush ButtonsCommanderPush::Empty = ButtonsCommanderPush(UNDEFINED_ID);

ButtonsCommanderPush::ButtonsCommanderPush() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->buttonPin = (GPIO_pin_t)DP_INVALID;
	this->lastButtonState = LOW;
	this->IdAddCounter = 0;
	this->IdLoopCounter = 0;
	this->IdSize = 0;
	this->pId = NULL;
	this->lastDebounceTime = 0;
	this->debounceDelay = 50;
}

ButtonsCommanderPush::ButtonsCommanderPush(unsigned long inId) : ButtonsCommanderPush()
{
	this->IdSize = 1;
	this->pId = new unsigned long[1];

	this->AddId(inId);
}

void ButtonsCommanderPush::begin(int inButtonPin, byte inIdNumber)
{	
	//CHECKPIN(inButtonPin, "ButtonsCommanderPush::begin");

#ifdef DEBUG_MODE
	if (this->IdSize > 0)
	{
		Serial.println(F("ButtonsCommanderPush::begin : the constructor has already defined only 1 ID !"));
		return;
	}
#endif
	this->buttonPin = Arduino_to_GPIO_pin(inButtonPin);

	pinMode2f(this->buttonPin, INPUT_PULLUP);
	this->IdSize = inIdNumber;
	this->pId = new unsigned long[inIdNumber];
}

// Returns the index of the new added position.
void ButtonsCommanderPush::AddId(unsigned long inId)
{
#ifdef DEBUG_MODE
	if (this->IdAddCounter == this->IdSize)
	{
		Serial.println(F("ButtonsCommanderPush::AddId : Too many Ids for this push button !"));
		return;
	}
#endif

	this->pId[this->IdAddCounter++] = inId;
}

unsigned long ButtonsCommanderPush::loop()
{
	unsigned long foundID = UNDEFINED_ID;

	if (this->buttonPin == DP_INVALID)
		return foundID;

	// read the state of the switch into a local variable:
	int reading = digitalRead2f(this->buttonPin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH), and you've waited 
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

			// only toggle the state if the new button state is LOW
			if (this->buttonState == LOW)
			{
				foundID = this->GetCurrentLoopId();
				eventType = COMMANDERS_EVENT_SELECTED;
				eventData = 0;
				Commander::RaiseEvent(this->GetCurrentLoopId(), COMMANDERS_EVENT_SELECTED, 0);
				this->IdLoopCounter++;
				if (this->IdLoopCounter >= this->IdAddCounter)
					this->IdLoopCounter = 0;
#ifdef DEBUG_MODE
				Serial.print(F("ButtonsCommanderPush currentloopid:"));
				Serial.println(this->IdLoopCounter, DEC);
#endif

			}
		}
		this->lastDebounceTime = 0;    
	}
  
	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	lastButtonState = reading;
	return foundID;
}
