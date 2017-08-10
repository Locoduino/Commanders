//-------------------------------------------------------------------
#ifndef __buttonsCommanderAnalogPushesItem_H__
#define __buttonsCommanderAnalogPushesItem_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES

/**This class describes a push item part of a ButtonsCommanderAnalogPushes class.
*/
class ButtonsCommanderAnalogPushesItem : public ButtonsCommanderButton
{
 private:
	int analogValue;			// analog value associated with this button
	int readingTolerancy;

 public:
	/**Default constructor.
	@param inId id of this button.
	*/
	ButtonsCommanderAnalogPushesItem(unsigned long inId = 0) : ButtonsCommanderButton(inId) { this->analogValue = 0; }

	/** Initialize the instance.
	@param inId				id of this button.
	@param inAnalogValue	Analog value associated with this button.
	@param inTolerancy	tolerancy of the read analog value to identify this push button.
	*/
	void begin(unsigned long inId, int inAnalogValue, int inTolerancy) { this->Id = inId;  this->analogValue = inAnalogValue; this->readingTolerancy = inTolerancy; }
	/**Checks if the given value is in the interval [value-tolerancy , value+tolerancy].
	@param inCurrentValue	value to check.
	@return true in the given value is in the interval.
	*/
	bool IsPushed(int inCurrentValue);
	/**Gets the reference analog value for this button.*/
	inline int GetAnalogValue() { return this->analogValue;	}

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