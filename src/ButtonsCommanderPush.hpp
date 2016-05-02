//-------------------------------------------------------------------
#ifndef __buttonsCommanderPush_H__
#define __buttonsCommanderPush_H__
//-------------------------------------------------------------------

#include "Commanders.h"
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
	
	CHAINLIST<Event> Events;

 public:
	ButtonsCommanderPush();
	ButtonsCommanderPush(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_TOGGLE, int inData = 0);

	void begin(int inButtonPin);
	void AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_TOGGLE, int inData = 0);
	unsigned long loop();

	inline int GetPin() const { return GPIO_to_Arduino_pin(this->buttonPin); }
	inline GPIO_pin_t GetPin2() const { return this->buttonPin; }
	inline int GetState() const { return this->buttonState; }
	
	static ButtonsCommanderPush Empty;
};

#define PUSH_BUTTON(name, pin, ID) \
	ButtonsCommanderPush *name = new ButtonsCommanderPush(ID); \
	name->begin(pin); \
	macro_buttons.Add(name);

#define PUSH_BUTTON_EVENT(name, pin, ID, Event, data) \
	ButtonsCommanderPush *name = new ButtonsCommanderPush(inId, ID, Event, data); \
	name->begin(pin); \
	macro_buttons.Add(name);

#define PUSH_BUTTON_ADDID(name, pin, ID) \
	name->AddEvent(ID);

#define PUSH_BUTTON_ADDID_EVENT(name, pin, ID, EVENT, data) \
	name->AddEvent(ID, EVENT, data);


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
