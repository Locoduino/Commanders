//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitch_H__
#define __buttonsCommanderSwitch_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH
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
	CMDRSCHAINEDLIST<EventPin> EventPins;

public:
	ButtonsCommanderSwitch();
	
	void begin();
	void AddEvent(unsigned long inId, int inPin, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	unsigned long loop();

#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
