//-------------------------------------------------------------------
#ifndef __potentiometer_H__
#define __potentiometer_H__
//-------------------------------------------------------------------

#include "Commanders.h"

//-------------------------------------------------------------------

class ButtonsCommanderPotentiometer : public ButtonsCommanderButton
{
 private:
	int currentValue;
	int pin;
	int moveAccuracy;
	int mini, maxi;
	
 public:
	ButtonsCommanderPotentiometer(unsigned long inId, int inMinimum, int inMaximum);

	inline bool IsAnalog() const { return true; }
	inline int GetPosition() const { return this->currentValue; }

	void begin(int inPin, int inMoveAccuracy = 1);
	unsigned long loop();
};

#define POTENTIOMETER(name, pin, ID, min, max) \
	ButtonsCommanderPotentiometer *name = new ButtonsCommanderPotentiometer(ID, min, max); \
	name->begin(pin); \
	macro_buttons.Add(name);

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
