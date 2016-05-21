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

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
