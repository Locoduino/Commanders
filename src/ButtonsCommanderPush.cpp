/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Push button with debounce.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER

ButtonsCommanderPush::ButtonsCommanderPush() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->buttonPin = (GPIO_pin_t)DP_INVALID;
	this->lastButtonState = LOW;
	this->lastDebounceTime = 0;
	this->debounceDelay = 50;
}

void ButtonsCommanderPush::begin(unsigned long inId, int inButtonPin, COMMANDERS_EVENT_TYPE inEventType, int inData)
{	
	this->Id = inId;
	this->buttonPin = Arduino_to_GPIO_pin(inButtonPin);

	pinMode2f(this->buttonPin, INPUT_PULLUP);

	this->AddEvent(inId, inEventType, inData);
}

// Returns the index of the new added position.
void ButtonsCommanderPush::AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inData)
{
	Event *pEvent = new Event();
	pEvent->Id = inId;
	pEvent->EventType = inEventType;
	pEvent->Data = inData;
	this->Events.AddItem(pEvent);
}

unsigned long ButtonsCommanderPush::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->Events.pFirst == NULL)
		Serial.println(F("This push button have no ID defined : call begin() !"));
#endif
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
				foundID = this->Events.pCurrentItem->Obj->Id;
				Commanders::RaiseEvent(foundID, 
					this->Events.pCurrentItem->Obj->EventType,
					this->Events.pCurrentItem->Obj->Data);

				this->Events.NextCurrent();
			}
		}
		this->lastDebounceTime = 0;    
	}
  
	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	this->lastButtonState = reading;
	return foundID;
}
#endif