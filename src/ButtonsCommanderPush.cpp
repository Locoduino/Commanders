/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Push button with debounce.>
*************************************************************/

#include <Commanders.h>
#include <stdint.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPUSH

ButtonsCommanderPush::ButtonsCommanderPush() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->buttonPin = (GPIO_pin_t)DP_INVALID;
	this->lastButtonState = HIGH;
	this->lastDebounceTime = 0;
	this->debounceDelay = 50;
}

void ButtonsCommanderPush::begin(unsigned long inId, int inButtonPin, COMMANDERS_EVENT_TYPE inEventType, int inData)
{	
	this->Id = inId;
	this->buttonPin = Arduino_to_GPIO_pin(inButtonPin);

	pinMode2f(this->buttonPin, INPUT_PULLUP);

	if (inId != UNDEFINED_ID)
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
	if (this->Events.pFirst == NULL)
	{
#ifdef COMMANDERS_DEBUG_MODE
		if (this->debounceDelay != UINT32_MAX) // If the error message has not been yet shown...
		{
			Serial.println(F("This push button have no ID defined : call begin() !"));
			// use it as a debug flag !
			this->debounceDelay = UINT32_MAX;	// The error message has been shown...
		}
#endif
		return UNDEFINED_ID;
	}

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
		if (reading == this->lastButtonState)
		{
			// only toggle the state if the new button state is LOW
			if (reading == LOW)
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

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderPush::printCommander()
{
	Serial.print(F("    Push - Pin :"));
	Serial.println(this->GetPin());

	CMDRSCHAINEDLISTITEM<Event> *pCurr = this->Events.pFirst;
	while (pCurr != NULL) 
	{ 
		Serial.print(F("        Event Id: "));
		Serial.print(pCurr->Obj->Id);
		Serial.print(F(" / Event type: "));
		Commanders::printEventType(pCurr->Obj->EventType, true);
		Commanders::printEventData(pCurr->Obj->EventType, pCurr->Obj->Data);
		Serial.print(F(" / Debounce delay: "));
		Serial.print(this->debounceDelay);
		Serial.println(F(""));

		pCurr = pCurr->pNext;
	}
}
#endif
#endif
#endif
