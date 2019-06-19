/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Switch button with one pin only with debounce.>
*************************************************************/

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

ButtonsCommanderSwitchTwoPins::ButtonsCommanderSwitchTwoPins() : ButtonsCommanderButton(UNDEFINED_ID)
{
	this->debounceDelay = 50;
	this->Pin1 = DP_INVALID;
	this->Pin2 = DP_INVALID;
}

void ButtonsCommanderSwitchTwoPins::begin(unsigned long inId1, int inPin1, unsigned long inId2, int inPin2)
{
	this->Pin1 = Arduino_to_GPIO_pin(inPin1);
	this->Id = inId1;
	this->lastButtonState1 = HIGH;
	this->lastDebounceTime1 = 0;

	this->Pin2 = Arduino_to_GPIO_pin(inPin2);
	this->Id2 = inId2;
	this->lastButtonState2 = HIGH;
	this->lastDebounceTime2 = 0;

	pinMode2f(this->Pin1, INPUT_PULLUP);
	pinMode2f(this->Pin2, INPUT_PULLUP);
}

void ButtonsCommanderSwitchTwoPins::beforeFirstLoop()
{
	if (this->Pin1 != DP_INVALID)
	{
		// Initialize first switch state at start
		int reading = digitalRead2f(this->Pin1);
		this->lastButtonState1 = reading;

		if (reading == LOW)
		{
			Commanders::RaiseEvent(this->Id, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
		}
		else
		{
			Commanders::RaiseEvent(this->Id, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
			reading = digitalRead2f(this->Pin2);
			this->lastButtonState2 = reading;
			Commanders::RaiseEvent(this->Id2, COMMANDERS_EVENT_MOVE, reading == LOW ? COMMANDERS_MOVE_ON : COMMANDERS_MOVE_OFF);
		}
	}
}

unsigned long ButtonsCommanderSwitchTwoPins::loop()
{
	bool changed = ButtonsCommanderSwitch::HavePinStateChanged(this->Pin1, this->debounceDelay, &this->lastButtonState1, &this->lastDebounceTime1);
	if (changed)
		Serial.println(F("1 changed !"));

	if (changed == true && this->lastButtonState1 == LOW)
	{
		Serial.println(F("this->lastButtonState1 = LOW"));
		Commanders::RaiseEvent(this->Id2, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
		Commanders::RaiseEvent(this->Id, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
		return this->Id;
	}

	changed = ButtonsCommanderSwitch::HavePinStateChanged(this->Pin2, this->debounceDelay, &this->lastButtonState2, &this->lastDebounceTime2);
	if (changed)
		Serial.println(F("2 changed !"));

	if (changed == true && this->lastButtonState2 == LOW)
	{
		Serial.println(F("this->lastButtonState2 = LOW"));
		Commanders::RaiseEvent(this->Id, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
		Commanders::RaiseEvent(this->Id2, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
		return this->Id2;
	}

	return UNDEFINED_ID;
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
	Serial.print(F(" / Debounce delay: "));
	Serial.print(this->debounceDelay);
	Serial.println(F(""));
}
#endif
#endif
#endif
