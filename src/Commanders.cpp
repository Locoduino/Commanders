/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Commanders.h"

CommandersEventHandlerFunction Commanders::EventHandler = 0;
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
	Commanders::SetLastEventType(inEvent);
	Commanders::SetLastEventData(inData);
	if (Commanders::EventHandler != NULL)
		Commanders::EventHandler(inId, inEvent, inData);

	return inId;
}

#ifdef COMMANDERS_DEBUG_MODE
void Commanders::printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(inId, DEC);
	Serial.print(F(" / "));
	switch (inEventType)
	{
	case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));		break;
	case COMMANDERS_EVENT_TOGGLE:		Serial.println(F("TOGGLE"));	break;
	case COMMANDERS_EVENT_MOVE:
		Serial.print(F("MOVE "));
		switch ((COMMANDERS_MOVE_TYPE)inEventData)
		{
		case COMMANDERS_MOVE_MORE:		Serial.println(F("MORE"));		break;
		case COMMANDERS_MOVE_LESS:		Serial.println(F("LESS"));		break;
		case COMMANDERS_MOVE_STOP:		Serial.println(F("STOP"));		break;
		case COMMANDERS_MOVE_LEFT:		Serial.println(F("LEFT"));		break;
		case COMMANDERS_MOVE_RIGHT:		Serial.println(F("RIGHT"));		break;
		case COMMANDERS_MOVE_CENTER:	Serial.println(F("CENTER"));	break;
		case COMMANDERS_MOVE_TOP:		Serial.println(F("TOP"));		break;
		case COMMANDERS_MOVE_BOTTOM:	Serial.println(F("BOTTOM"));	break;
		case COMMANDERS_MOVE_STRAIGHT:	Serial.println(F("STRAIGHT"));	break;
		case COMMANDERS_MOVE_DIVERGE:	Serial.println(F("DIVERGE"));	break;
		case COMMANDERS_MOVE_ON:		Serial.println(F("ON"));		break;
		case COMMANDERS_MOVE_OFF:		Serial.println(F("OFF"));		break;
		}
		break;
	case COMMANDERS_EVENT_MOVEPOSITION:
		Serial.print(F("MOVEPOSITION : "));
		Serial.println(inEventData, DEC);
		break;
	case COMMANDERS_EVENT_MOVEPOSITIONID:
		Serial.println(F("MOVEPOSITIONID "));
		break;
	case COMMANDERS_EVENT_MOVEPOSITIONINDEX:
		Serial.print(F("MOVEPOSITIONINDEX : "));
		Serial.println(inEventData, DEC);
		break;
	case COMMANDERS_EVENT_CONFIG:
		Serial.print(F("CONFIG : "));
		Serial.print(COMMANDERSCONFIGADDRESS(inEventData), DEC);
		Serial.print(F(" / "));
		Serial.println(COMMANDERSCONFIGVALUE(inEventData), DEC);
		break;
	}
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

	return Commander::loops();
}
