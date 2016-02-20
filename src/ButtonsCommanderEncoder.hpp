//-------------------------------------------------------------------
#ifndef __buttonsCommanderEncoder_H__
#define __buttonsCommanderEncoder_H__
//-------------------------------------------------------------------

#ifdef VISUALC
#include "VStudio/arduino2.hpp"
#else
#include "arduino2.hpp"
#endif
#include "BasicsCommanders.h"

class ButtonsCommanderPush;

//-------------------------------------------------------------------

class ButtonsCommanderEncoder : public ButtonsCommanderButton
{
 private:
	int currentValue;
	GPIO_pin_t pin1;
	GPIO_pin_t pin2;
	int moveAccuracy;
	int mini, maxi;
	int lastEncoded;

public:
	ButtonsCommanderPush *pPush;
	
 public:
	ButtonsCommanderEncoder(unsigned long inId, int inStrtingCurrentValue, int inMinimum, int inMaximum, bool inAssociatedPushButton = false);

	inline bool IsAnalog() const { return false; }
	inline int GetPosition() const { return this->currentValue; }

	void Setup(int inPin1, int inPin2, int inMoveAccuracy = 1);
	BasicsCommanderEvent Loop();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
