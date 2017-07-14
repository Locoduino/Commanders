//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitchTwoPins_H__
#define __buttonsCommanderSwitchTwoPins_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

//-------------------------------------------------------------------

class ButtonsCommanderSwitchTwoPins : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t Pin1;
	GPIO_pin_t Pin2;
	// unsigned long Id1;  / use base id instead...
	unsigned long Id2;

	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	GPIO_pin_t lastSelectedPin;

	int lastButtonState;   // the previous reading from the current input pin
	unsigned long lastDebounceTime;  // the last time the current output pin was toggled
	
public:
	ButtonsCommanderSwitchTwoPins();
	
	void begin(unsigned long inId1, int inPin1, unsigned long inId2, int inPin2);
	void beforeFirstLoop();
	unsigned long loop();
	inline unsigned long GetId(GPIO_pin_t inPin) const { return inPin == this->Pin1 ? this->Id : (inPin == this->Pin2 ? this->Id2 : UNDEFINED_ID); }

#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
