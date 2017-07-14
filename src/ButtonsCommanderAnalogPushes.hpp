//-------------------------------------------------------------------
#ifndef __buttonsCommanderAnalogPushes_H__
#define __buttonsCommanderAnalogPushes_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES
#include "ButtonsCommanderAnalogPushesItem.hpp"

//-------------------------------------------------------------------
// A push button is a hardware device giving a time limited impulsion.
// It will return one Dccid at a time, passing through the whole list of DccIds.
// The analog pushes variant use one analog pin with some pushes connected 
// through resistors.
//
// Sample for five pushes.
// |	     |	       |	     |	       |
// /	     /	       /	     /	       / Pushes
// |	     |	       |	     |	       |
// |  3kOhms | 1kOhms  | 620 Ohms| 330 Ohms|
// +---***---+---***---+---***---+---***---+------> Analog pin
//
// When one push button is pressed, the value read on the analog pin depends of the 
// resistor addition from the button to the pin.
// It is important that only one button was pushed at a time.
// It is possible to use more or less buttons, but resistor values have to be computed
// to avoid confusing analog values between near pushes...
//-------------------------------------------------------------------

class ButtonsCommanderAnalogPushes : public ButtonsCommanderButton
{
private:
	int analogPin;			// Number of the pushbutton analog pin. This is a normal number, not an arduino2 GPIO_pin_t.
	uint8_t number;			// Number of buttons
	uint8_t lastButtonPressed; // last button pressed number
	int lastButtonState;	// the previous reading from the input pin
	int readingTolerancy;	// if the value is between 'button value-tolerancy' and 'button value+tolerancy' , this is good !

	// List of items which must be allocated by the constructor.
	unsigned char size;
	ButtonsCommanderAnalogPushesItem *pButtons;

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers

public:
	ButtonsCommanderAnalogPushes();

	void begin(int inButtonPin, uint8_t inNumber, unsigned long *inpIds, int *inpButtonValues, int inTolerancy = 20);
	unsigned long loop();
	void EndLoop();
	ButtonsCommanderAnalogPushesItem *GetItem(uint8_t inNumber) { return &(this->pButtons[inNumber]); }
	ButtonsCommanderButton* GetFromId(unsigned long inId);

	inline uint8_t GetLastButtonPressed() const { return this->lastButtonPressed; }
#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
