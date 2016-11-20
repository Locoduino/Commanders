//-------------------------------------------------------------------
#ifndef __potentiometer_H__
#define __potentiometer_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER

//-------------------------------------------------------------------

class ButtonsCommanderPotentiometer : public ButtonsCommanderButton
{
 private:
	int currentValue;
	int pin;
	int moveAccuracy;
	int mini, maxi;
	
 public:
	ButtonsCommanderPotentiometer();

	//inline bool IsAnalog() const { return true; }
	inline int GetPosition() const { return this->currentValue; }

	void begin(unsigned long inId, int inPin, int inMinimum, int inMaximum, int inMoveAccuracy = 1);
	unsigned long loop();
#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
