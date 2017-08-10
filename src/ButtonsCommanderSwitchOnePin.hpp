//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitchOnePin_H__
#define __buttonsCommanderSwitchOnePin_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

/** A switch can be used in a On/Off way. In this case only one Arduino pin is connected to the switch.
The event raised is always of type COMMANDERS_EVENT_MOVE and data is COMMANDERS_MOVE_ON or COMMANDERS_MOVE_OFF.

Events thrown:

         reason            |   id      |         type          | data
---------------------------|-----------|-----------------------|--------------------
pin state to HIGH          | button id | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_ON
previous pin state to LOW  | button id | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_OFF
*/
class ButtonsCommanderSwitchOnePin : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t Pin;
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	GPIO_pin_t lastSelectedPin;

	int lastButtonState;   // the previous reading from the current input pin
	unsigned long lastDebounceTime;  // the last time the current output pin was toggled
	
public:
	/** Default constructor.*/
	ButtonsCommanderSwitchOnePin();
	
	/** Initialize the instance.
	@param inId		id of the button.
	@param inPin	Arduino pin connected to the switch button.
	*/
	void begin(unsigned long inId, int inPin);
	/** This function will be called after sketch Setup(), but before the first call to loop().
	It will be called only one time.*/
	void beforeFirstLoop();
	/** Main loop function. */
	unsigned long loop();

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
