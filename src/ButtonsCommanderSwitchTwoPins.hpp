//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitchTwoPins_H__
#define __buttonsCommanderSwitchTwoPins_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERSWITCH

/** A switch can be used in a A/B way. In this case only two Arduino pins are connected to the switch.
The event raised is always of type COMMANDERS_EVENT_MOVE and data is COMMANDERS_MOVE_ON or COMMANDERS_MOVE_OFF,
but the id used depends on the activated pin. Two events are always thrown by Commanders :
the activation of one pin, and the disactvation of the other !

Events thrown:

         reason            |   id       |         type          | data
---------------------------|------------|-----------------------|--------------------
pin1 state to HIGH         | button id1 | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_ON
previous pin2 state to LOW | button id2 | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_OFF
pin2 state to HIGH         | button id2 | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_ON
previous pin1 state to LOW | button id1 | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_OFF
*/
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
	/** Default constructor.*/
	ButtonsCommanderSwitchTwoPins();
	
	/** Initialize the instance.
	@param inId1	id of the first position.
	@param inPin1	Arduino pin connected to the switch button for the first position.
	@param inId2	id of the second position.
	@param inPin2	Arduino pin connected to the switch button for the second position.
	*/
	void begin(unsigned long inId1, int inPin1, unsigned long inId2, int inPin2);
	/** This function will be called after sketch Setup(), but before the first call to loop().
	It will be called only one time.*/
	void beforeFirstLoop();
	/** Main loop function. */
	unsigned long loop();
	/** Gets the right id according to the activated pin.*/
	inline unsigned long GetId(GPIO_pin_t inPin) const { return inPin == this->Pin1 ? this->Id : (inPin == this->Pin2 ? this->Id2 : UNDEFINED_ID); }

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
