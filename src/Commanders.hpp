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
	static byte lastConfigAddress;
	static unsigned long StartStatusLed;

public:
	static CommandersEventHandlerFunction EventHandler;
	static GPIO_pin_t StatusLedPin;
	static unsigned int BlinkDelay;

	static inline void SetEventHandler(CommandersEventHandlerFunction func) { Commanders::EventHandler = func; }
	static inline void SetStatusLedPin(int inStatusLedPin, unsigned int inBlinkDelay = 1000) 
	{
		StatusLedPin = Arduino_to_GPIO_pin(inStatusLedPin); 
		pinMode2f(StatusLedPin, OUTPUT); 
		BlinkDelay = inBlinkDelay; 
	}
	static unsigned long loop();

	static inline COMMANDERS_EVENT_TYPE GetLastEventType() { return lastEventType; }
	static inline int GetLastEventData() { return lastEventData; }
	static inline void SetLastEventType(COMMANDERS_EVENT_TYPE inEvent) { lastEventType = inEvent; lastEventData = 0; }
	static inline void SetLastEventData(int inData) { lastEventData = inData; }
	static inline byte GetLastConfigAddress() { return lastConfigAddress; }

	static void StatusBlink();
	static unsigned long RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);
	//static inline void AddDelayedEvent(unsigned long inDelay, unsigned long inId, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_TOGGLE, int inData = 0);
#ifdef COMMANDERS_DEBUG_MODE
	static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
#endif
};

#endif
