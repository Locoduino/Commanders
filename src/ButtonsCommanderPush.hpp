//-------------------------------------------------------------------
#ifndef __buttonsCommanderPush_H__
#define __buttonsCommanderPush_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPUSH
#include "Chain.hpp"

struct Event
{
	unsigned long Id;
	COMMANDERS_EVENT_TYPE EventType;
	int Data;
};

//-------------------------------------------------------------------
// A push button is a hardware device giving a time limited impulsion.
// It will return one id at a time, passing through the whole list of Ids.
//-------------------------------------------------------------------

class ButtonsCommanderPush : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t buttonPin;	// the number of the pushbutton pin
	int buttonState;		// the current reading from the input pin
	int lastButtonState;	// the previous reading from the input pin

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	
	CMDRSCHAINEDLIST<Event> Events;

 public:
	ButtonsCommanderPush();

	void begin(unsigned long inId, int inButtonPin, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	void AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	unsigned long loop();

	inline int GetPin() const { return GPIO_to_Arduino_pin(this->buttonPin); }
	inline GPIO_pin_t GetDIO2Pin() const { return this->buttonPin; }
	inline int GetState() const { return this->buttonState; }
	
	static ButtonsCommanderPush Empty;
};

//-------------------------------------------------------------------
#endif
#endif
#endif
