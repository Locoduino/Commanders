//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitchOnePin_H__
#define __buttonsCommanderSwitchOnePin_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

//-------------------------------------------------------------------

class ButtonsCommanderSwitchOnePin : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t Pin;
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	GPIO_pin_t lastSelectedPin;

	int lastButtonState;   // the previous reading from the current input pin
	unsigned long lastDebounceTime;  // the last time the current output pin was toggled
	
public:
	ButtonsCommanderSwitchOnePin();
	
	void begin(unsigned long inId, int inPin);
	void beforeFirstLoop();
	unsigned long loop();

#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
