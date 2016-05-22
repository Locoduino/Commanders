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

public:
	static CommandersEventHandlerFunction EventHandler;

	static void StartSetup(CommandersEventHandlerFunction func, int inStatusLedPin = 0, unsigned int inBlinkDelay = 1000);
	static void StartSetup(int inStatusLedPin = 0, unsigned int inBlinkDelay = 1000);
	static inline void SetEventHandler(CommandersEventHandlerFunction func) { Commanders::EventHandler = func; }
	static inline void SetStatusLedPin(int inStatusLedPin, unsigned int inBlinkDelay = 1000) 
	{
		Commander::StatusLedPin = Arduino_to_GPIO_pin(inStatusLedPin); 
		pinMode2f(Commander::StatusLedPin, OUTPUT); 
		Commander::BlinkDelay = inBlinkDelay; 
	}
	static void EndSetup();
	static unsigned long loop();

	static inline COMMANDERS_EVENT_TYPE GetLastEventType() { return lastEventType; }
	static inline int GetLastEventData() { return lastEventData; }
	static inline void SetLastEventType(COMMANDERS_EVENT_TYPE inEvent) { lastEventType = inEvent; lastEventData = 0; }
	static inline void SetLastEventData(int inData) { lastEventData = inData; }
	static inline byte GetLastConfigAddress() { return lastConfigAddress; }
	//static inline void AddDelayedEvent(unsigned long inDelay, unsigned long inId, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_TOGGLE, int inData = 0);
#ifdef COMMANDERS_DEBUG_MODE
	static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
#endif
};

#endif
