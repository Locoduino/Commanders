/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include <Commanders.h>
#include "EventStack.hpp"

CommandersEventHandlerFunction Commanders::EventHandler = NULL;
COMMANDERS_EVENT_TYPE Commanders::lastEventType;
int Commanders::lastEventData;
GPIO_pin_t Commanders::StatusLedPin = DP_INVALID;
unsigned int Commanders::BlinkDelay = 1000;
unsigned long Commanders::StartStatusLed = 0;
bool FirstLoop = true;

void Commanders::StatusBlink()
{
	if (Commanders::StatusLedPin != DP_INVALID && Commanders::StartStatusLed == 0)
	{
		Commanders::StartStatusLed = millis();
		digitalWrite2f(Commanders::StatusLedPin, HIGH);
	}
}

unsigned long Commanders::RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
#ifdef VISUALSTUDIO
	ArduiEmulator::ArduinoForm::_eventLog("Sent", inId, inEvent, inData);
#endif

	Commanders::StatusBlink();
	if (*(Commanders::EventHandler) != NULL)
	{
		Commanders::SetLastEventType(inEvent);
		Commanders::SetLastEventData(inData);
		Commanders::EventHandler(inId, inEvent, inData);
		return inId;
	}

	EventStack::EventsStack.PushEvent(inId, inEvent, inData);
	return UNDEFINED_ID;
}

#if defined(COMMANDERS_DEBUG_MODE) || defined(COMMANDERS_PRINT_COMMANDERS)

void Commanders::printEventMoveType(COMMANDERS_MOVE_TYPE inMoveType)
{
	switch (inMoveType)
	{
	case COMMANDERS_MOVE_MORE:		Serial.print(F("MORE"));		break;
	case COMMANDERS_MOVE_LESS:		Serial.print(F("LESS"));		break;
	case COMMANDERS_MOVE_STOP:		Serial.print(F("STOP"));		break;
	case COMMANDERS_MOVE_LEFT:		Serial.print(F("LEFT"));		break;
	case COMMANDERS_MOVE_RIGHT:		Serial.print(F("RIGHT"));		break;
	case COMMANDERS_MOVE_CENTER:	Serial.print(F("CENTER"));		break;
	case COMMANDERS_MOVE_TOP:		Serial.print(F("TOP"));			break;
	case COMMANDERS_MOVE_BOTTOM:	Serial.print(F("BOTTOM"));		break;
	case COMMANDERS_MOVE_STRAIGHT:	Serial.print(F("STRAIGHT"));	break;
	case COMMANDERS_MOVE_DIVERGE:	Serial.print(F("DIVERGE"));		break;
	case COMMANDERS_MOVE_ON:		Serial.print(F("ON"));			break;
	case COMMANDERS_MOVE_OFF:		Serial.print(F("OFF"));			break;
	}
}

void Commanders::printEventData(COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	switch (inEventType)
	{
	case COMMANDERS_EVENT_MOVE:
		printEventMoveType((COMMANDERS_MOVE_TYPE)inEventData);
		break;
	case COMMANDERS_EVENT_MOVEPOSITION:
		Serial.print(inEventData, DEC);
		break;
	case COMMANDERS_EVENT_MOVEPOSITIONINDEX:
		Serial.print(inEventData, DEC);
		break;
	case COMMANDERS_EVENT_CONFIG:
		Serial.print(COMMANDERSCONFIGADDRESS(inEventData), DEC);
		Serial.print(F(" / "));
		Serial.print(COMMANDERSCONFIGVALUE(inEventData), DEC);
		break;
	}
}

void Commanders::printEventType(COMMANDERS_EVENT_TYPE inEventType, bool inDataFollow)
{
	switch (inEventType)
	{
	case COMMANDERS_EVENT_NONE:				Serial.print(F("NONE"));				break;
	case COMMANDERS_EVENT_TOGGLE:			Serial.print(F("TOGGLE"));			break;
	case COMMANDERS_EVENT_MOVE:				Serial.print(F("MOVE"));				break;
	case COMMANDERS_EVENT_MOVEPOSITION:		Serial.print(F("MOVEPOSITION"));		break;
	case COMMANDERS_EVENT_MOVEPOSITIONID:	Serial.print(F("MOVEPOSITIONID"));		break;
	case COMMANDERS_EVENT_MOVEPOSITIONINDEX:Serial.print(F("MOVEPOSITIONINDEX"));	break;
	case COMMANDERS_EVENT_CONFIG:			Serial.print(F("CONFIG"));				break;
	}

	if (inDataFollow)
		switch (inEventType)
		{
		case COMMANDERS_EVENT_MOVE:
		case COMMANDERS_EVENT_MOVEPOSITION:
		case COMMANDERS_EVENT_MOVEPOSITIONINDEX:
		case COMMANDERS_EVENT_CONFIG:
			Serial.print(F(" "));
			break;
		}
}

void Commanders::printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(inId, DEC);
	Serial.print(F(" / "));
	printEventType(inEventType, true);
	printEventData(inEventType, inEventData);
	Serial.println(F(""));
}
#endif

unsigned long Commanders::loop()
{
	if (FirstLoop)
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.println(F("*** Setup Commanders Finished."));
#endif
		Commander::BeforeFirstLoops();
		FirstLoop = false;
	}

	if (Commanders::StatusLedPin != DP_INVALID && 
		Commanders::StartStatusLed > 0 && 
		millis() - Commanders::StartStatusLed > Commanders::BlinkDelay)
	{
		digitalWrite2f(Commanders::StatusLedPin, LOW);
		Commanders::StartStatusLed = 0;
	}

#ifndef NO_EVENTSSEQUENCER
	EventsSequencer::loops();
#endif

	unsigned long id;
	id = Commander::loops();

	if (*(Commanders::EventHandler) == NULL)
	{
		COMMANDERS_EVENT_TYPE type;
		int data;

		byte event = EventStack::EventsStack.GetPendingEventIndex();
		if (event < EVENT_MAXNUMBER)
		{
			EventStack::EventsStack.GetEvent(event, &id, &type, &data);
			Commanders::SetLastEventType(type);
			Commanders::SetLastEventData(data);
		}
	}

#ifdef COMMANDERS_DEBUG_MODE
	if (id != UNDEFINED_ID)
		printEvent(id, Commanders::GetLastEventType(), Commanders::GetLastEventData());
#endif

	return id;
}
