/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Switch button with debounce.>
*************************************************************/

#include "ButtonsCommanderSwitch.hpp"

ButtonsCommanderSwitch::ButtonsCommanderSwitch() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->debounceDelay = 50;
}

void beginItem(const IdPin &inIdPin)
{
	if (inIdPin.Pin != DP_INVALID)
		pinMode2f(inIdPin.Pin, INPUT_PULLUP);
}

void ButtonsCommanderSwitch::begin()
{
	CHAIN_ENUMERATE(IdPin, this->IdPins, beginItem);
}

// Returns the index of the new added position.
void ButtonsCommanderSwitch::AddId(int inPin, unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	IdPin idpin;
	idpin.Pin = Arduino_to_GPIO_pin(inPin);
	idpin.Id = inId;
	idpin.Event = inEvent;
	idpin.Data = inData;
	this->IdPins.AddItem(idpin);

	pinMode2f(idpin.Pin, INPUT_PULLUP);
}

unsigned long ButtonsCommanderSwitch::loop()
{
	// read the state of the switch into a local variable:
	int reading = digitalRead2f(this->IdPins.pCurrentItem->Obj.Pin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != this->IdPins.pCurrentItem->Obj.lastButtonState)
	{
		// reset the debouncing timer
		this->IdPins.pCurrentItem->Obj.lastDebounceTime = millis();
	}

	unsigned long haveFound = UNDEFINED_ID;

	if (this->IdPins.pCurrentItem->Obj.lastDebounceTime > 0 && (millis() - this->IdPins.pCurrentItem->Obj.lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->IdPins.pCurrentItem->Obj.buttonState)
		{
			this->IdPins.pCurrentItem->Obj.buttonState = reading;

			// only toggle the state if the new button state is HIGH
			if (this->IdPins.pCurrentItem->Obj.buttonState == HIGH)
			{
				haveFound = this->IdPins.pCurrentItem->Obj.Id;
				eventType = this->IdPins.pCurrentItem->Obj.Event;
				eventData = this->IdPins.pCurrentItem->Obj.Data;
				Commander::RaiseEvent(haveFound, eventType, eventData);
			}
		}
		this->IdPins.pCurrentItem->Obj.lastDebounceTime = 0;
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	this->IdPins.pCurrentItem->Obj.lastButtonState = reading;

	this->IdPins.NextCurrent();

	return haveFound;
}
