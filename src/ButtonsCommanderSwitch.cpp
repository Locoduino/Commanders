/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Switch button with debounce.>
*************************************************************/

#include <Commanders.h>
#include <stdint.h>
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

		pCurr->Obj->LastButtonState = HIGH;
		pCurr->Obj->LastDebounceTime = 0;

		pCurr = pCurr->pNext;
	}
}

unsigned long ButtonsCommanderSwitch::GetId(GPIO_pin_t inPin, COMMANDERS_EVENT_TYPE *apEvent, int *apData) const
{
	CMDRSCHAINEDLISTITEM<EventPin> *pCurr = this->EventPins.pFirst;
	while (pCurr != NULL)
	{
		if (inPin == pCurr->Obj->Pin)
		{
			if (apEvent != NULL)
				*apEvent = pCurr->Obj->Event;
			if (apData != NULL)
				*apData = pCurr->Obj->Data;
			return pCurr->Obj->Id;
		}

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
	pIdpin->LastButtonState = HIGH;
	pIdpin->LastDebounceTime = 0;
	this->EventPins.AddItem(pIdpin);

	return pIdpin;
}

unsigned long ButtonsCommanderSwitch::loopOnePin(unsigned long inId, GPIO_pin_t inPin, unsigned long inPreviousId,unsigned long inDebounceDelay, 
																				byte *inpLastPinState, unsigned long *inpLastDebounceTime, bool inSendEvent)
{
	unsigned long haveFound = UNDEFINED_ID;

	if (inPin != DP_INVALID)
	{
		// read the state of the switch into a local variable:
		int pinState = digitalRead2f(inPin);

		// check to see if you just pressed the button 
		// (i.e. the input went from HIGH to LOW, inverted by INPUT_PULLUP), and you've waited 
		// long enough since the last press to ignore any noise:  

		// If the switch changed, due to noise or pressing:
		if (pinState != *inpLastPinState)
		{
			// reset the debouncing timer
#ifdef COMMANDERS_DEBUG_MODE
			Serial.print(F("Pin "));
			Serial.print(GPIO_to_Arduino_pin(inPin));
			Serial.print(pinState == HIGH ? F(" high") : F(" low"));
			Serial.println(F(" debounced !"));
#endif
			*inpLastPinState = pinState;
			*inpLastDebounceTime = millis();
			return UNDEFINED_ID;
		}

		if (*inpLastDebounceTime > 0 && (millis() - *inpLastDebounceTime) > inDebounceDelay)
		{
			// whatever the pinState is at, it's been there for longer
			// than the debounce delay, so take it as the actual current state:

			if (pinState == LOW)
			{
				haveFound = inId;

				// raise the event for the new pin.
				if (inSendEvent)
					Commanders::RaiseEvent(inId, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
			}
			else
			{
				// raise the event for the old pin.
				if (inSendEvent && inPreviousId != UNDEFINED_ID)
					Commanders::RaiseEvent(inPreviousId, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
			}

			*inpLastDebounceTime = 0;
		}

		// save the pinState.  Next time through the loop,
		// it'll be the lastButtonState:
		*inpLastPinState = pinState;
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
			int pinState = digitalRead2f(pCurr->Obj->Pin);

			if (pinState == LOW)
			{
				this->lastSelectedPin = pCurr->Obj->Pin;
				Commanders::RaiseEvent(pCurr->Obj->Id, pCurr->Obj->Event, pCurr->Obj->Data);
			}
		}

		pCurr = pCurr->pNext;
	}
}

unsigned long ButtonsCommanderSwitch::loop()
{
	if (this->EventPins.pFirst == NULL)
	{
#ifdef COMMANDERS_DEBUG_MODE
		if (this->debounceDelay != UINT32_MAX) // If the error message has not been yet shown...
		{
			Serial.println(F("This switch button have no ID defined : call AddEvent() and begin() !"));
			// use it as a debug flag !
			this->debounceDelay = UINT32_MAX;	// The error message has been shown...
		}
#endif
		return UNDEFINED_ID;
	}

	EventPin *pCurr = this->EventPins.pCurrentItem->Obj;

	unsigned long haveFound = ButtonsCommanderSwitch::loopOnePin(pCurr->Id, pCurr->Pin, UNDEFINED_ID,	this->debounceDelay, &pCurr->LastButtonState, &pCurr->LastDebounceTime, false);
	
	if (haveFound != UNDEFINED_ID)
	{
		this->lastSelectedPin = pCurr->Pin;
		Commanders::RaiseEvent(haveFound, pCurr->Event, pCurr->Data);
	}

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
		Serial.print(F(" / Debounce delay: "));
		Serial.print(this->debounceDelay);
		Serial.println(F(""));

		pCurr = pCurr->pNext;
	}
}
#endif
#endif
#endif
