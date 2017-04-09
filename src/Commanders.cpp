/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Commanders.h"
#include "EventStack.hpp"

CommandersEventHandlerFunction Commanders::EventHandler = NULL;
COMMANDERS_EVENT_TYPE Commanders::lastEventType;
int Commanders::lastEventData;
GPIO_pin_t Commanders::StatusLedPin = DP_INVALID;
unsigned int Commanders::BlinkDelay = 1000;
unsigned long Commanders::StartStatusLed = 0;

#ifdef COMMANDERS_DEBUG_MODE
	bool FirstLoop = true;
#endif

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
	}
	else
		EventStack::EventsStack.RaiseEvent(inId, inEvent, inData);

	return inId;
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
	case COMMANDERS_EVENT_NONE:				Serial.println(F("NONE"));				break;
	case COMMANDERS_EVENT_TOGGLE:			Serial.println(F("TOGGLE"));			break;
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

#ifdef COMMANDERS_PRINT_COMMANDERS
void Commanders::printCommanders()
{
	Serial.println(F("********* Commanders List"));
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != NULL)
	{
		pCurr->printCommander();
		pCurr = pCurr->pNextCommander;
	}
	Serial.println(F("********* End of List"));
}
#endif

unsigned long Commanders::loop()
{
#ifdef COMMANDERS_DEBUG_MODE
	if (FirstLoop)
	{
		Serial.println(F("*** Setup Commanders Finished."));
		FirstLoop = false;
	}
#endif

	if (Commanders::StatusLedPin != DP_INVALID && 
		Commanders::StartStatusLed > 0 && 
		millis() - Commanders::StartStatusLed > Commanders::BlinkDelay)
	{
		digitalWrite2f(Commanders::StatusLedPin, LOW);
		Commanders::StartStatusLed = 0;
	}

	unsigned long id;
	id = Commander::loops();

	if (*(Commanders::EventHandler) == NULL)
	{
		COMMANDERS_EVENT_TYPE type;
		int data;

		byte event = EventStack::EventsStack.GetPendingEvent();
		if (event < EVENT_MAXNUMBER)
		{
			EventStack::EventsStack.PushEvent(event, &id, &type, &data);
			Commanders::SetLastEventType(type);
			Commanders::SetLastEventData(data);
		}
	}
	return id;
}
