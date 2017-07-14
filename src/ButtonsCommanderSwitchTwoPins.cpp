/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Switch button with one pin only with debounce.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

ButtonsCommanderSwitchTwoPins::ButtonsCommanderSwitchTwoPins() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->debounceDelay = 50;
	this->lastSelectedPin = DP_INVALID;
}

void ButtonsCommanderSwitchTwoPins::begin(unsigned long inId1, int inPin1, unsigned long inId2, int inPin2)
{
	this->Pin1 = Arduino_to_GPIO_pin(inPin1);
	this->Id = inId1;
	this->Pin2 = Arduino_to_GPIO_pin(inPin2);
	this->Id2 = inId2;

	pinMode2f(this->Pin1, INPUT_PULLUP);
	pinMode2f(this->Pin2, INPUT_PULLUP);
}

void ButtonsCommanderSwitchTwoPins::beforeFirstLoop()
{
	if (this->Pin1 != DP_INVALID)
	{
		// Initialize first switch state at start
		int reading = digitalRead2f(this->Pin1);

		if (reading == LOW)
		{
			this->lastSelectedPin = this->Pin1;
			Commanders::RaiseEvent(this->Id, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
		}
		else
		{
			Commanders::RaiseEvent(this->Id, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
			reading = digitalRead2f(this->Pin2);
			if (reading == LOW)
				this->lastSelectedPin = this->Pin2;
			Commanders::RaiseEvent(this->Id2, COMMANDERS_EVENT_MOVE, reading == LOW ? COMMANDERS_MOVE_ON : COMMANDERS_MOVE_OFF);
		}

		this->lastButtonState = reading;
	}
}

unsigned long ButtonsCommanderSwitchTwoPins::loop()
{
	unsigned long haveFound = ButtonsCommanderSwitch::loopOnePin(this->Pin1, this->Id, this->GetId(this->lastSelectedPin), 
		&this->debounceDelay, &this->lastSelectedPin,
		&this->lastButtonState, &this->lastDebounceTime);

	if (haveFound != UNDEFINED_ID)
		return haveFound;

	return ButtonsCommanderSwitch::loopOnePin(this->Pin2, this->Id2, this->GetId(this->lastSelectedPin),
		&this->debounceDelay, &this->lastSelectedPin,
		&this->lastButtonState, &this->lastDebounceTime);
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderSwitchTwoPins::printCommander()
{
	Serial.print(F("    SwitchTwoPins        Pin1: "));
	Serial.print(GPIO_to_Arduino_pin(this->Pin1));
	Serial.print(F(" / Id1: "));
	Serial.print(this->Id);
	Serial.print(F(" / Pin2: "));
	Serial.print(GPIO_to_Arduino_pin(this->Pin2));
	Serial.print(F(" / Id2: "));
	Serial.print(this->Id2);
	Serial.println(F(""));
}
#endif
#endif
#endif
