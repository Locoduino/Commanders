//-------------------------------------------------------------------
#ifndef __buttonsCommanderAnalogPushes_H__
#define __buttonsCommanderAnalogPushes_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#include "ButtonsCommanderAnalogPushesItem.hpp"

#define PUSHES(list, nb)	((ButtonsCommanderAnalogPushes *) list[nb])
#define PUSHESITEM(list, nb, item)	(((ButtonsCommanderAnalogPushes *) list[nb])->GetItem(item))

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
	byte number;			// Number of buttons
	byte lastButtonPressed; // last button pressed number
	int buttonState;		// the current reading from the input pin
	int lastButtonState;	// the previous reading from the input pin
	int readingTolerancy;	// if the value is between 'button value-tolerancy' and 'button value+tolerancy' , this is good !

	// List of items which must be allocated by the constructor.
	unsigned char size;
	ButtonsCommanderAnalogPushesItem *pButtons;

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers

public:
	ButtonsCommanderAnalogPushes(byte inNumber);

	void begin(int inButtonPin, unsigned long *inpIds, int *inpButtonValues, int inTolerancy = 20);
	unsigned long loop();
	void EndLoop();
	ButtonsCommanderAnalogPushesItem *GetItem(byte inNumber) { return &(this->pButtons[inNumber]); }
	ButtonsCommanderButton* GetFromId(unsigned long inId);

	inline byte GetLastButtonPressed() const { return this->lastButtonPressed; }
};

#define ANALOGPUSH_BUTTON(name, pin, number, IDS, values) \
	ButtonsCommanderAnalogPushes *name = new ButtonsCommanderAnalogPushes(number); \
	name->begin(pin, IDS, values); \
	macro_buttons.Add(name);

#define ANALOGPUSH_BUTTON_TOL(name, pin, number, IDS, values, tol) \
	ButtonsCommanderAnalogPushes *name = new ButtonsCommanderAnalogPushes(number); \
	name->begin(pin, IDS, values, tol); \
	macro_buttons.Add(name);

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
