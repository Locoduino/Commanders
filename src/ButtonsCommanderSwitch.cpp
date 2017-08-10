/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Switch button with debounce.>
*************************************************************/

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

ButtonsCommanderSwitch::ButtonsCommanderSwitch() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->debounceDelay = 50;
	this->lastSelectedPin = DP_INVALID;
}

void beginItem(EventPin *inpIdPin)
{
	if (inpIdPin->Pin != DP_INVALID)
	{
		pinMode2f(inpIdPin->Pin, INPUT_PULLUP);
	}
}

void ButtonsCommanderSwitch::begin()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->EventPins.pFirst == NULL)
		Serial.println(F("This switch button have no ID defined : begin() must be called AFTER AddEvent !"));
#endif

	CMDRSCHAINEDLISTITEM<EventPin> *pCurr = this->EventPins.pFirst;
	while (pCurr != NULL)
	{
		beginItem(pCurr->Obj);

		int reading = digitalRead2f(pCurr->Obj->Pin);

		if (reading == LOW)
			this->lastSelectedPin = pCurr->Obj->Pin;

		pCurr = pCurr->pNext;
	}
}

unsigned long ButtonsCommanderSwitch::GetId(GPIO_pin_t inPin) const
{
	CMDRSCHAINEDLISTITEM<EventPin> *pCurr = this->EventPins.pFirst;
	while (pCurr != NULL)
	{
		if (inPin == pCurr->Obj->Pin)
			return pCurr->Obj->Id;

		pCurr = pCurr->pNext;
	}
	return UNDEFINED_ID;
}

// Returns the index of the new added position.
EventPin *ButtonsCommanderSwitch::AddEvent(unsigned long inId, int inPin, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	EventPin *pIdpin = new EventPin();
	pIdpin->Pin = Arduino_to_GPIO_pin(inPin);
	pIdpin->Id = inId;
	pIdpin->Event = inEvent;
	pIdpin->Data = inData;
	this->EventPins.AddItem(pIdpin);

	return pIdpin;
}

unsigned long ButtonsCommanderSwitch::loopOnePin(GPIO_pin_t inPin, unsigned long inId, unsigned long inPreviousId,
	unsigned long *apDebounceDelay, GPIO_pin_t *apLastSelectedPin,
	int *apLastButtonState, unsigned long *apLastDebounceTime, bool inSendEvent)
{
	unsigned long haveFound = UNDEFINED_ID;

	if (inPin != DP_INVALID)
	{
		// read the state of the switch into a local variable:
		int reading = digitalRead2f(inPin);

		// check to see if you just pressed the button 
		// (i.e. the input went from HIGH to LOW, inverted by INPUT_PULLUP), and you've waited 
		// long enough since the last press to ignore any noise:  

		// If the switch changed, due to noise or pressing:
		if (reading != *apLastButtonState)
		{
			// reset the debouncing timer
			*apLastButtonState = reading;
			*apLastDebounceTime = millis();
			return haveFound;
		}

		if (*apLastDebounceTime > 0 && (millis() - *apLastDebounceTime) > *apDebounceDelay)
		{
			// whatever the reading is at, it's been there for longer
			// than the debounce delay, so take it as the actual current state:

			// if the button state has changed:
			if (reading == LOW)
			{
				// raise the event for the new pin.
				if (inSendEvent)
					Commanders::RaiseEvent(inId, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
				//haveFound = inId;
				*apLastSelectedPin = inPin;
			}
			else
			{
				if (inSendEvent)
					Commanders::RaiseEvent(inPreviousId, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
			}
			*apLastDebounceTime = 0;
		}

		// save the reading.  Next time through the loop,
		// it'll be the lastButtonState:
		*apLastButtonState = reading;
	}

	return haveFound;		 
}

void ButtonsCommanderSwitch::beforeFirstLoop()
{
	CMDRSCHAINEDLISTITEM<EventPin> *pCurr = this->EventPins.pFirst;
	while (pCurr != NULL)
	{
		if (pCurr->Obj->Pin != DP_INVALID)
		{
			// Initialize first switch state at start
			int reading = digitalRead2f(pCurr->Obj->Pin);

			Commanders::RaiseEvent(pCurr->Obj->Id, COMMANDERS_EVENT_MOVE, reading == LOW ? COMMANDERS_MOVE_ON : COMMANDERS_MOVE_OFF);

			if (reading == LOW)
				this->lastSelectedPin = pCurr->Obj->Pin;

			this->lastButtonState = reading;
		}

		pCurr = pCurr->pNext;
	}

}

unsigned long ButtonsCommanderSwitch::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->EventPins.pFirst == NULL)
		Serial.println(F("This switch button have no ID defined : call AddEvent() and begin() !"));
#endif

	unsigned long haveFound = ButtonsCommanderSwitch::loopOnePin(this->EventPins.pCurrentItem->Obj->Pin, this->EventPins.pCurrentItem->Obj->Id, 
		this->GetId(this->lastSelectedPin),
		&this->debounceDelay, &this->lastSelectedPin,
		&this->lastButtonState, &this->lastDebounceTime, false);

	if (haveFound != UNDEFINED_ID)
		Commanders::RaiseEvent(haveFound, this->EventPins.pCurrentItem->Obj->Event, this->EventPins.pCurrentItem->Obj->Data);

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
