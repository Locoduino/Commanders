//-------------------------------------------------------------------
#ifndef __buttonsCommanderAnalogPushes_H__
#define __buttonsCommanderAnalogPushes_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES
#include "ButtonsCommanderAnalogPushesItem.hpp"

/**
A push button is a hardware device giving a time limited impulsion.
It will return one Dccid at a time, passing through the whole list of DccIds.
The analog pushes variant use one analog pin with some pushes connected 
through resistors.

\verbatim
Sample for five pushes.
+		 +		   +		 +		   + GND
|	     |	       |	     |	       |
/	     /	       /	     /	       / Pushes
|	     |	       |	     |	       |
| 3kOhms | 1kOhms  | 620 Ohms| 330 Ohms|
|	     |	       |	     |	       |
+--***---+---***---+---***---+---***---+------> Analog pin
                                       |
									   *
									   * 10kOhms
									   *
									   |
									   + 5v
\endverbatim

When one push button is pressed, the value read on the analog pin depends of the 
resistor addition from the button to the pin.
It is important that only one button was pushed at a time.
It is possible to use more or less buttons, but resistor values have to be computed
to avoid confusing analog values between near pushes...

This class is a list of ButtonsCommanderAnalogPushesItem, basically a push button.
Each item has its own id, analog value and tolerance interval.

Events thrown:

reason               |   id      |                 type          | data
---------------------|-----------|-------------------------------|------
pin state to LOW     | push id | COMMANDERS_EVENT_MOVEPOSITIONID | 0
*/
class ButtonsCommanderAnalogPushes : public ButtonsCommanderButton
{
private:
	int analogPin;			// Number of the pushbutton analog pin. This is a normal number, not an arduino2 GPIO_pin_t.
	uint8_t number;			// Number of buttons
	uint8_t lastButtonPressed; // last button pressed number
	int lastButtonState;	// the previous reading from the input pin
	int readingTolerancy;	// if the value is between 'button value-tolerance' and 'button value+tolerance' , this is good !

	// List of items which must be allocated by the constructor.
	unsigned char size;
	ButtonsCommanderAnalogPushesItem *pButtons;

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers

public:
	/** Default constructor.*/
	ButtonsCommanderAnalogPushes();

	/** Initialize the instance.
	@param inButtonPin	Analog Arduino pin connected to this group of pushes.
	@param inNumber		number of push buttons.
	@param inpIds		Array of ids of each button.
	@param inpButtonValues	array of analog values, one for each button.
	@param inTolerancy	tolerancy of the read analog value to identify the correct push button.
	*/
	void begin(int inButtonPin, uint8_t inNumber, unsigned long *inpIds, int *inpButtonValues, int inTolerancy = 20);
	/** Main loop function. */
	unsigned long loop();
	/** Reinitialize the list for future use. Called at the end of the loop, if an event has been raised.*/
	void EndLoop();
	/** Gets one push item.*/
	ButtonsCommanderAnalogPushesItem *GetItem(uint8_t inNumber) { return &(this->pButtons[inNumber]); }
	/** Gets one push item through its id.*/
	ButtonsCommanderButton* GetFromId(unsigned long inId);

	/**Gets the last push button pressed.*/
	inline uint8_t GetLastButtonPressed() const { return this->lastButtonPressed; }
#ifdef COMMANDERS_PRINT_COMMANDERS
	/** Print this Commander on the console.
	@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
