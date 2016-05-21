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

void beginItem(const EventPin &inIdPin)
{
	if (inIdPin.Pin != DP_INVALID)
		pinMode2f(inIdPin.Pin, INPUT_PULLUP);
}

void ButtonsCommanderSwitch::begin()
{
	CHAIN_ENUMERATE(EventPin, this->EventPins, beginItem);
}

// Returns the index of the new added position.
void ButtonsCommanderSwitch::AddEvent(int inPin, unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	EventPin idpin;
	idpin.Pin = Arduino_to_GPIO_pin(inPin);
	idpin.Id = inId;
	idpin.Event = inEvent;
	idpin.Data = inData;
	this->EventPins.AddItem(idpin);

	pinMode2f(idpin.Pin, INPUT_PULLUP);
}

unsigned long ButtonsCommanderSwitch::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->EventPins.pFirst == NULL)
		Serial.println(F("This switch button have no ID defined : call AddEvent() and begin() !"));
#endif

	// read the state of the switch into a local variable:
	int reading = digitalRead2f(this->EventPins.pCurrentItem->Obj.Pin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != this->EventPins.pCurrentItem->Obj.lastButtonState)
	{
		// reset the debouncing timer
		this->EventPins.pCurrentItem->Obj.lastDebounceTime = millis();
	}

	unsigned long haveFound = UNDEFINED_ID;

	if (this->EventPins.pCurrentItem->Obj.lastDebounceTime > 0 && (millis() - this->EventPins.pCurrentItem->Obj.lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->EventPins.pCurrentItem->Obj.buttonState)
		{
			this->EventPins.pCurrentItem->Obj.buttonState = reading;

			// only toggle the state if the new button state is HIGH
			if (this->EventPins.pCurrentItem->Obj.buttonState == HIGH)
			{
				haveFound = this->EventPins.pCurrentItem->Obj.Id;
				eventType = this->EventPins.pCurrentItem->Obj.Event;
				eventData = this->EventPins.pCurrentItem->Obj.Data;
				Commander::RaiseEvent(haveFound, eventType, eventData);
			}
		}
		this->EventPins.pCurrentItem->Obj.lastDebounceTime = 0;
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	this->EventPins.pCurrentItem->Obj.lastButtonState = reading;

	this->EventPins.NextCurrent();

	return haveFound;
}
