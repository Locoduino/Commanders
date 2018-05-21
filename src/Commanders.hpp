//-------------------------------------------------------------------
#ifndef __Commanders_HPP__
#define __Commanders_HPP__
//-------------------------------------------------------------------

#include <Commanders.h>

typedef void(*CommandersEventHandlerFunction)(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

/**
This is the main class of the library. All data and functions are static.
There is no way to instantiate this class.

To handle events from external hardware of buses, there is two ways:

An event handler can be defined. This function will be called as soon as an event is thrown by any commander.
The second way is to get event id, type and data from the Commanders.loop() function inside the main loop
of the sketch.

The two ways cannot be used together...
*/
class Commanders
{
private:
	static COMMANDERS_EVENT_TYPE lastEventType;
	static int lastEventData;
	static uint8_t lastConfigAddress;
	static unsigned long StartStatusLed;
	static CommandersEventHandlerFunction EventHandler;

public:
	/** This pin will start to blink at each event thrown. */
	static GPIO_pin_t StatusLedPin;
	/** Blinking delay for the status led. */
	static unsigned int BlinkDelay;

	/** Initialize the instance.
	@param inStatusLedPin	This pin will start to blink at each event thrown. Default -1 for no status led.
	@param inBlinkDelay		Blinking delay for the status led. Default is 1000ms.
	*/
	static inline void begin(int inStatusLedPin = -1, unsigned int inBlinkDelay = 1000)
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.println(F(""));
		Serial.println(F("Commanders V1.60.2"));
		Serial.println(F("Created by Thierry Paris."));
		Serial.println(F("(c) Locoduino 2016-2018"));
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

	/** Initialize the instance.
	@param func				Event handler function called when an event is thrown.
	@param inStatusLedPin	This pin will start to blink at each event thrown. Default -1 for no status led.
	@param inBlinkDelay		Blinking delay for the status led. Default is 1000ms.
	*/
	static inline void begin(CommandersEventHandlerFunction func, int inStatusLedPin = -1, unsigned int inBlinkDelay = 1000)
	{
		Commanders::EventHandler = func;
		begin(inStatusLedPin, inBlinkDelay);
	}

	/** Main loop function.
	This function will call all necessary internal loop functions for each commander activated...
	@return Id of the activated item. Use GetLastEvent* functions to get the complete event.
	*/
	static unsigned long loop();

	/** Get the last event type thrown.
	@return Type of the event.
	*/
	static inline COMMANDERS_EVENT_TYPE GetLastEventType() { return lastEventType; }
	/** Get the last event data thrown.
	@return Data associated with the last event.
	*/
	static inline int GetLastEventData() { return lastEventData; }
	/** Force the kind of last event type.
	@param inEvent New event type.
	@remark Undocumented internal function.
	*/
	static inline void SetLastEventType(COMMANDERS_EVENT_TYPE inEvent) { lastEventType = inEvent; lastEventData = 0; }
	/** Force the data associated with the last event type.
	@remark Undocumented internal function.
	*/
	static inline void SetLastEventData(int inData) { lastEventData = inData; }
	/** Get the last config address used.
	@return last config address used.
	@remark Not yet implemented.
	*/
	static inline uint8_t GetLastConfigAddress() { return lastConfigAddress; }
	/** Start blinking the status led. */
	static void StatusBlink();
	/** Raise a new event.
	@param inId Id of the new event.
	@param inEvent Type of the new event. Default is COMMANDERS_EVENT_MOVEPOSITIONID.
	@param inData Associated data to the event type. Default is 0.
	@return Id of this event.
	*/
	static unsigned long RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
#if defined(COMMANDERS_DEBUG_MODE) || defined(COMMANDERS_PRINT_COMMANDERS)
	/** Print on the console the given moving type in plain English.
	@remark Only available if COMMANDERS_DEBUG_MODE or COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	static void printEventMoveType(COMMANDERS_MOVE_TYPE inMoveType);
	/** Print on the console the given associated data.
	@remark Only available if COMMANDERS_DEBUG_MODE or COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	static void printEventData(COMMANDERS_EVENT_TYPE inEventType, int inEventData);
	/** Print on the console the given type in plain English.
	@remark Only available if COMMANDERS_DEBUG_MODE or COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	static void printEventType(COMMANDERS_EVENT_TYPE inEventType, bool inDataFollow);
	/** Print on the console the given event in plain English.
	@remark Only available if COMMANDERS_DEBUG_MODE or COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
	/** This define is empty if COMMANDERS_DEBUG_MODE or COMMANDERS_PRINT_COMMANDERS is not defined. */
	#define COMMANDERS_PRINT_EVENT(id, type, data)		Commanders::printEvent(id, type, data);
#else
	#define COMMANDERS_PRINT_EVENT(id, type, data)
#endif
};

#endif
