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
};

class ButtonsCommanderSwitch : public ButtonsCommanderButton
{
 private:
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	CMDRSCHAINEDLIST<EventPin> EventPins;
	GPIO_pin_t lastSelectedPin;

	int lastButtonState;   // the previous reading from the current input pin
	unsigned long lastDebounceTime;  // the last time the current output pin was toggled

public:
	ButtonsCommanderSwitch();
	
	void begin();
	EventPin *AddEvent(unsigned long inId, int inPin, COMMANDERS_EVENT_TYPE inEvent = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	void beforeFirstLoop();
	unsigned long loop();
	unsigned long GetId(GPIO_pin_t inPin) const;

	static unsigned long loopOnePin(GPIO_pin_t inPin, unsigned long inId, unsigned long inPreviousId,
		unsigned long *apDebounceDelay, GPIO_pin_t *apLastSelectedPin,
		int *apLastButtonState, unsigned long *apLastDebounceTime, bool inSendEvent = true);

#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
