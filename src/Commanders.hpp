//-------------------------------------------------------------------
#ifndef __Commanders_HPP__
#define __Commanders_HPP__
//-------------------------------------------------------------------

#include "Commanders.h"

typedef void(*CommandersEventHandlerFunction)(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

class Commanders
{
private:
	static COMMANDERS_EVENT_TYPE lastEventType;
	static int lastEventData;
	static uint8_t lastConfigAddress;
	static unsigned long StartStatusLed;

public:
	static CommandersEventHandlerFunction EventHandler;
	static GPIO_pin_t StatusLedPin;
	static unsigned int BlinkDelay;

	static inline void begin(int inStatusLedPin = -1, unsigned int inBlinkDelay = 1000)
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.println(F(""));
		Serial.println(F("Commanders V1.23"));
		Serial.println(F("Developed by Thierry Paris."));
		Serial.println(F("(c) Locoduino 2016"));
		Serial.println(F(""));

		Serial.println(F("*** Setup Commanders started."));
#endif
		if (inStatusLedPin != -1)
		{
			StatusLedPin = Arduino_to_GPIO_pin(inStatusLedPin);
			pinMode2f(StatusLedPin, OUTPUT);
			BlinkDelay = inBlinkDelay;
		}
	}

	static inline void begin(CommandersEventHandlerFunction func, int inStatusLedPin = -1, unsigned int inBlinkDelay = 1000)
	{
		Commanders::EventHandler = func;
		begin(inStatusLedPin, inBlinkDelay);
	}

	static unsigned long loop();

	static inline COMMANDERS_EVENT_TYPE GetLastEventType() { return lastEventType; }
	static inline int GetLastEventData() { return lastEventData; }
	static inline void SetLastEventType(COMMANDERS_EVENT_TYPE inEvent) { lastEventType = inEvent; lastEventData = 0; }
	static inline void SetLastEventData(int inData) { lastEventData = inData; }
	static inline uint8_t GetLastConfigAddress() { return lastConfigAddress; }
	static void StatusBlink();
	static unsigned long RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	//static inline void AddDelayedEvent(unsigned long inDelay, unsigned long inId, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
#if defined(COMMANDERS_DEBUG_MODE) || defined(COMMANDERS_PRINT_COMMANDERS)
	static void printEventMoveType(COMMANDERS_MOVE_TYPE inMoveType);
	static void printEventData(COMMANDERS_EVENT_TYPE inEventType, int inEventData);
	static void printEventType(COMMANDERS_EVENT_TYPE inEventType, bool inDataFollow);
	static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
#endif
#ifdef COMMANDERS_PRINT_COMMANDERS
	static void printCommanders();
#endif
};

#endif
