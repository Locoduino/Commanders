//-------------------------------------------------------------------
#ifndef __buttonsCommanderPush_H__
#define __buttonsCommanderPush_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPUSH
#include "Chain.hpp"

/** Event for the push button event list.*/
struct Event
{
	unsigned long Id; /**<id for this event.*/
	COMMANDERS_EVENT_TYPE EventType; /**<Event type.*/
	int Data; /**<Associated data.*/
};

/**
A push button is a hardware device giving a time limited impulsion.
It will return one id at a time, passing through the whole list of Ids.

Events thrown:

reason           |   id      |    type    | data
-----------------|-----------|------------|------
pin state to LOW | button id | event type | data
*/
class ButtonsCommanderPush : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t buttonPin;	// the number of the pushbutton pin
	byte lastButtonState;	// the previous reading from the input pin

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	
	CMDRSCHAINEDLIST<Event> Events;

 public:
	/** Default constructor.*/
	ButtonsCommanderPush();

	/** Initialize the instance.
	@param inId		id of the button.
	@param inButtonPin	Arduino pin connected to the push button.
	@param inEventType	Event type used when the button is pressed. Default COMMANDERS_EVENT_MOVEPOSITIONID.
	@param inData		Associated data when the push button is pressed. Default 0.
	*/
	void begin(unsigned long inId, int inButtonPin, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	/** A push button can throw different events when it is pressed. This function add a new event in the list.
	When the first press on push button is done, the main id/event type/data Event defined by begin(à) function will be thrown. 
	At the following press, the first event of the list is thrown, and so on at each new press until the end of the list.
	At the end, the next event is the main one defined by begin.
	@param inId			id of the button.
	@param inEventType	Event type used when the button is pressed. Default COMMANDERS_EVENT_MOVEPOSITIONID.
	@param inData		Associated data when the push button is pressed. Default 0.
	*/
	void AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	/** Main loop function. */
	unsigned long loop();

	/** Gets the pin number connected to the push button.
	@return Pin number.
	*/
	inline int GetPin() const { return GPIO_to_Arduino_pin(this->buttonPin); }
	/** Gets the pin number connected to the push button, in DIO2 library format.
	@return Pin number.
	*/
	inline GPIO_pin_t GetDIO2Pin() const { return this->buttonPin; }
	
#ifdef COMMANDERS_PRINT_COMMANDERS
	/** Print this Commander on the console.
	@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
