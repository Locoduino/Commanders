/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Composite push button array on analog pin with debounce.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES

ButtonsCommanderAnalogPushes::ButtonsCommanderAnalogPushes() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->analogPin = 0;
	this->lastButtonState = LOW;

	this->lastDebounceTime = 0;
	this->debounceDelay = 50;

	this->size = 0;
}

void ButtonsCommanderAnalogPushes::begin(int inButtonPin, uint8_t inNumberOfItems, unsigned long *inpIds, int *inpButtonValues, int inTolerancy)
{	
	this->size = inNumberOfItems;
	this->pButtons = new ButtonsCommanderAnalogPushesItem[this->size];

	this->analogPin = inButtonPin;
	this->readingTolerancy = inTolerancy;

	for (int i = 0; i < this->size; i++)
	{
#ifdef COMMANDERS_DEBUG_MODE
		if (inpButtonValues[i] < 0 || inpButtonValues[i] > 1023)
		{
			Serial.print(F("Analog push buttons. Invalid value "));
			Serial.print(inpButtonValues[i]);
			Serial.print(F(" for button "));
			Serial.print(i);
			Serial.println(F(". Value must betwwen 0 and 1023 !"));
		}
#endif
		this->pButtons[i].begin(inpIds[i], inpButtonValues[i], inTolerancy);
	}

	pinMode(this->analogPin, INPUT);
}

ButtonsCommanderButton* ButtonsCommanderAnalogPushes::GetFromId(unsigned long inId)
{
	for (int i = 0; i < this->size; i++)
		if (this->pButtons[i].GetId() == inId)
			return &(this->pButtons[i]);

	return 0;
}			 

unsigned long ButtonsCommanderAnalogPushes::loop()
{
	unsigned long foundID = UNDEFINED_ID;

	if (this->analogPin == 0)
		return foundID;

	// read the state of the switch into a local variable:
	int reading = analogRead(this->analogPin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the button changed, due to noise or pressing:
	if (reading < this->lastButtonState - this->readingTolerancy || reading > this->lastButtonState + this->readingTolerancy)
	{
		// reset the debouncing timer
		this->lastDebounceTime = millis();
	}

	if (this->lastDebounceTime > 0 && (millis() - this->lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading < this->buttonState - this->readingTolerancy || reading > this->buttonState + this->readingTolerancy)
		{
			this->buttonState = reading;

			for (int i = 0; i < this->size; i++)
				if (this->pButtons[i].IsPushed(reading))
				{
					foundID = this->pButtons[i].GetId();
					Commanders::RaiseEvent(foundID, COMMANDERS_EVENT_MOVEPOSITIONID, 0);
#ifdef COMMANDERS_DEBUG_MODE
					Serial.print(F("Analog push button "));
					Serial.print(i);
					Serial.println(F(" pressed"));
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

void ButtonsCommanderAnalogPushes::EndLoop()
{
	this->lastButtonPressed = -1;
}
#endif
#endif