/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Switch button with debounce.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

ButtonsCommanderSwitch::ButtonsCommanderSwitch() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->debounceDelay = 50;
}

void beginItem(EventPin *inpIdPin)
{
	if (inpIdPin->Pin != DP_INVALID)
		pinMode2f(inpIdPin->Pin, INPUT_PULLUP);
}

void ButtonsCommanderSwitch::begin()
{
	CMDRSCHAIN_ENUMERATE(EventPin, this->EventPins, beginItem);
}

// Returns the index of the new added position.
void ButtonsCommanderSwitch::AddEvent(unsigned long inId, int inPin, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	EventPin *pIdpin = new EventPin();
	pIdpin->Pin = Arduino_to_GPIO_pin(inPin);
	pIdpin->Id = inId;
	pIdpin->Event = inEvent;
	pIdpin->Data = inData;
	this->EventPins.AddItem(pIdpin);

	pinMode2f(pIdpin->Pin, INPUT_PULLUP);
}

unsigned long ButtonsCommanderSwitch::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->EventPins.pFirst == NULL)
		Serial.println(F("This switch button have no ID defined : call AddEvent() and begin() !"));
#endif

	// read the state of the switch into a local variable:
	int reading = digitalRead2f(this->EventPins.pCurrentItem->Obj->Pin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != this->EventPins.pCurrentItem->Obj->lastButtonState)
	{
		// reset the debouncing timer
		this->EventPins.pCurrentItem->Obj->lastDebounceTime = millis();
	}

	unsigned long haveFound = UNDEFINED_ID;

	if (this->EventPins.pCurrentItem->Obj->lastDebounceTime > 0 && (millis() - this->EventPins.pCurrentItem->Obj->lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->EventPins.pCurrentItem->Obj->buttonState)
		{
			this->EventPins.pCurrentItem->Obj->buttonState = reading;

			// only toggle the state if the new button state is HIGH
			if (this->EventPins.pCurrentItem->Obj->buttonState == HIGH)
			{
				haveFound = this->EventPins.pCurrentItem->Obj->Id;
				Commanders::RaiseEvent(haveFound, 
					this->EventPins.pCurrentItem->Obj->Event,
					this->EventPins.pCurrentItem->Obj->Data);
			}
		}
		this->EventPins.pCurrentItem->Obj->lastDebounceTime = 0;
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	this->EventPins.pCurrentItem->Obj->lastButtonState = reading;

	this->EventPins.NextCurrent();

	return haveFound;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderSwitch::printCommander()
{
	Serial.println(F("    Switch"));

	CMDRSCHAINEDLISTITEM<EventPin> *pCurr = this->EventPins.pFirst;
	while (pCurr != NULL)
	{
		Serial.print(F("        Event - Pin: "));
		Serial.print(GPIO_to_Arduino_pin(pCurr->Obj->Pin));
		Serial.print(F(" / Id: "));
		Serial.print(pCurr->Obj->Id);
		Serial.print(F(" / Event type: "));
		Commanders::printEventType(pCurr->Obj->Event, true);
		Commanders::printEventData(pCurr->Obj->Event, pCurr->Obj->Data);
		Serial.println(F(""));

		pCurr = pCurr->pNext;
	}
}
#endif
#endif
#endif
