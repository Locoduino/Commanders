//-------------------------------------------------------------------
#ifndef __potentiometer_H__
#define __potentiometer_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER

/** This class describes a potentiometer.
This is an analog item, with a minimal, a maximal and a current analog value.
An analog pin of the Arduino will return a value between 0 and 1023. The current value of this item
will be mapped from 0/1023 to minimum/maximum current value.
An event will be thrown when a change of the value will be greater than moveAccuracy.

Events thrown:

reason        |   id      |                 type          | data
--------------|-----------|-------------------------------|------
value changed | button id | COMMANDERS_EVENT_MOVEPOSITION | value
*/
class ButtonsCommanderPotentiometer : public ButtonsCommanderButton
{
 private:
	int currentValue;
	int pin;
	int moveAccuracy;
	int mini, maxi;
	
 public:
	 /** Default constructor.*/
	 ButtonsCommanderPotentiometer();

	 /** Gets the current value.*/
	 inline int GetPosition() const { return this->currentValue; }

	/** Initialize the instance.
	 @param inId		id of the button.
	 @param inPin Arduino analog pin connected to this potentiometer.
	 @param inMinimum	minimum value, when the Arduino pin returns 0.
	 @param inMaximum	maximum value, when the Arduino pin returns value of 1023.
	 @param inMoveAccuracy minimum change value to throw a new event.
	*/
	void begin(unsigned long inId, int inPin, int inMinimum, int inMaximum, int inMoveAccuracy = 1);
	/** Change the mini/maxi values.
	@param inMini	minimum value, when the Arduino pin returns 0.
	@param inMaxi	maximum value, when the Arduino pin returns value of 1023.
	@param inMoveAccuracy minimum change value to throw a new event.
	*/
	inline void SetMiniMaxi(int inMini, int inMaxi, int inMoveAccuracy = 1) { this->mini = inMini; this->maxi = inMaxi; this->moveAccuracy = inMoveAccuracy; }
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
