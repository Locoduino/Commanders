//-------------------------------------------------------------------
#ifndef __buttonsCommanderEncoder_H__
#define __buttonsCommanderEncoder_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERENCODER

class ButtonsCommanderPush;

//-------------------------------------------------------------------

class ButtonsCommanderEncoder : public ButtonsCommanderButton
{
 private:
	int currentValue;
	int startingCurrentValue;
	GPIO_pin_t pin1;
	GPIO_pin_t pin2;
	int mini, maxi;
	int lastEncoded;

 public:
	ButtonsCommanderEncoder();

	inline int GetPosition() const { return this->currentValue; }
	inline void ResetStartingPosition() { this->currentValue = this->startingCurrentValue; }

	void begin(int inPin1, int inPin2, unsigned long inId, int inStartingCurrentValue = 0, int inMinimum = 0, int inMaximum = 0);
	unsigned long loop();
};
//-------------------------------------------------------------------
#endif
#endif
#endif
