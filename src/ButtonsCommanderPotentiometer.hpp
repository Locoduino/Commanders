//-------------------------------------------------------------------
#ifndef __potentiometer_H__
#define __potentiometer_H__
//-------------------------------------------------------------------

#ifdef VISUALC
#include "VStudio/arduino2.hpp"
#else
#include "arduino2.hpp"
#endif
#include "ButtonsCommanderButton.hpp"

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

	void Setup(int inPin, int inMoveAccuracy = 1);
	unsigned long Loop();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
