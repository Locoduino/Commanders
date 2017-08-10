//-------------------------------------------------------------------
#ifndef __buttonsCommanderEncoder_H__
#define __buttonsCommanderEncoder_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERENCODER

class ButtonsCommanderPush;


/** This class describes a rotary encoder.
A rotary or "shaft" encoder is an angular measuring device. It is used to precisely measure rotation
of motors or to create wheel controlers (knobs) that can turn infinitely (with no end stop like a potentiometer has).
Some of them are also equipped with a pushbutton when you press on the axis (like the ones used for navigation on many music controllers).
In this case, only add a new ButtonsCommanderPush class instance beside the encoder.
This class can handles also a current position, using the encoder like a potentiometer...

Events thrown:

reason               |   id      |                 type          | data
----------------------------|-----------|-------------------------------|---------------------
clockwise movement          | button id | COMMANDERS_EVENT_MOVE         | COMMANDERS_MOVE_MORE
counter-clockwise movement  | button id | COMMANDERS_EVENT_MOVE         | COMMANDERS_MOVE_LESS
positional position changed | button id | COMMANDERS_EVENT_MOVEPOSITION | value
*/
class ButtonsCommanderEncoder : public ButtonsCommanderButton
{
 private:
	int currentValue;
	int startingCurrentValue;
	GPIO_pin_t pin1;
	GPIO_pin_t pin2;
	int mini, maxi;
	int lastEncoded;
	byte moveIncrement;	// number of encoder movements needed to change current position.
	char incrementPosition;

 public:
	/** Default constructor.*/
	ButtonsCommanderEncoder();

	/**Gets current value of the positional mode.*/
	inline int GetPosition() const { return this->currentValue; }
	/**Resets the current value of the positional mode to its starting value.*/
	inline void ResetStartingPosition() { this->currentValue = this->startingCurrentValue; }

	/** Initialize the instance. This encoder can be use like a potentiometer. To use this positional value,
	give a different value to minimum and maximum arguments.
	@param inId		id of the button.
	@param inPin1	Arduino pin connected to this encoder.
	@param inPin2	Arduino pin connected to this encoder.
	@param inMoveIncrement	minimum change value to throw a new event. Default is 3, but it can depends of the material used.
	@param inStartingCurrentValue	In positional use, starting position. Must be between minimum and maximum ! Default is 0.
	@param inMinimum	minimum value in positional use, default is 0.
	@param inMaximum	maximum value in positional use, default is 0.
	*/
	void begin(unsigned long inId, int inPin1, int inPin2, byte inMoveIncrement = 3, int inStartingCurrentValue = 0, int inMinimum = 0, int inMaximum = 0);
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
