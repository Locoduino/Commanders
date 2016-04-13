//-------------------------------------------------------------------
#ifndef __Commanders_HPP__
#define __Commanders_HPP__
//-------------------------------------------------------------------

#include "Commanders.h"

class Commanders
{
private:
	static COMMANDERS_EVENT_TYPE lastEventType;
	static int lastEventData;

public:
	static void StartSetup(CommandersEventHandlerFunction func, int inStatusLedPin = 0);
	static void StartSetup(int inStatusLedPin = 0);
	static inline void SetEventHandler(CommandersEventHandlerFunction func) { Commander::EventHandler = func; }
	static inline void SetStatusLedPin(int inStatusLedPin) { Commander::StatusLedPin = Arduino_to_GPIO_pin(inStatusLedPin); pinMode2f(Commander::StatusLedPin, OUTPUT); }
	static void EndSetup();
	static unsigned long loop();

	static inline COMMANDERS_EVENT_TYPE GetLastEventType() { return lastEventType; }
	static inline int GetLastEventData() { return lastEventData; }
	static inline void SetLastEventType(COMMANDERS_EVENT_TYPE inEvent) { lastEventType = inEvent; lastEventData = 0; }
	static inline void SetLastEventData(int inData) { lastEventData = inData; }
};

#endif
