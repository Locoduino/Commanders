//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitch_H__
#define __buttonsCommanderSwitch_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#include "Chain.hpp"

//-------------------------------------------------------------------

struct EventPin
{
	GPIO_pin_t Pin;
	unsigned long Id;
	COMMANDERS_EVENT_TYPE Event;
	int Data;

	int buttonState;       // the current reading from the input pin
	int lastButtonState;   // the previous reading from the input pin
	unsigned long lastDebounceTime;  // the last time the output pin was toggled
};

class ButtonsCommanderSwitch : public ButtonsCommanderButton
{
 private:
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	CHAINEDLIST<EventPin> EventPins;

public:
	ButtonsCommanderSwitch();
	
	void begin();
	void AddEvent(int inPin, unsigned long inId, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_TOGGLE, int inData = 0);
	unsigned long loop();
};

#define SWITCH_BUTTON(name, pin, ID) \
	ButtonsCommanderSwitch *name = new ButtonsCommanderSwitch(); \
	name->AddEvent(pin, ID); \
	name->begin(); \
	macro_buttons.Add(name);

#define SWITCH_BUTTON_2ID_MOTOR(name, pin1, ID1, pin2, ID2) \
	ButtonsCommanderSwitch *name = new ButtonsCommanderSwitch(); \
	name->AddEvent(pin1, ID1, COMMANDERS_EVENT_MOVELEFT); \
	name->AddEvent(pin2, ID2, COMMANDERS_EVENT_MOVERIGHT); \
	name->begin(); \
	macro_buttons.Add(name);

#define SWITCH_BUTTON_2ID_LIGHT(name, pin1, ID1, pin2, ID2) \
	ButtonsCommanderSwitch *name = new ButtonsCommanderSwitch(); \
	name->AddEvent(pin1, ID1, COMMANDERS_EVENT_MOVESTOP); \
	name->AddEvent(pin2, ID2, COMMANDERS_EVENT_MOVELEFT); \
	name->begin(); \
	macro_buttons.Add(name);

#define SWITCH_BUTTON_ADDID(name, pin, ID) \
	name->AddEvent(pin, ID);

#define SWITCH_BUTTON_ADDID_EVENT(name, pin, ID, EVENT, data) \
	name->AddEvent(pin, ID, EVENT, data);

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
