//-------------------------------------------------------------------
#ifndef __buttonsCommanderAnalogPushesItem_H__
#define __buttonsCommanderAnalogPushesItem_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES

class ButtonsCommanderAnalogPushesItem : public ButtonsCommanderButton
{
 private:
	int analogValue;			// analog value associated with this button
	int readingTolerancy;

 public:
	ButtonsCommanderAnalogPushesItem(unsigned long inId = 0) : ButtonsCommanderButton(inId) { this->analogValue = 0; }

	void begin(unsigned long inId, int inAnalogValue, int inTolerancy) { this->Id = inId;  this->analogValue = inAnalogValue; this->readingTolerancy = inTolerancy; }
	bool IsPushed(int inCurrentValue);
	inline int GetAnalogValue() { return this->analogValue;	}
};

//-------------------------------------------------------------------
#endif
#endif
#endif