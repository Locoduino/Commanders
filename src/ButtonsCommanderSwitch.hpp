//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitch_H__
#define __buttonsCommanderSwitch_H__
//-------------------------------------------------------------------

#ifdef VISUALC
#include "VStudio/arduino2.hpp"
#else
#include "arduino2.hpp"
#endif
#include "ButtonsCommanderButton.hpp"

//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifdef DEBUG_MODE
#define CHECKPORT(type, val, text)	CheckPortNb(type, val, F(text))
#define CHECKPIN(val, text)		Driver::CheckPinNb(val, F(text))
#else
#define CHECKPORT(type, val, text)
#define CHECKPIN(val, text)
#endif

struct IdPin
{
	unsigned long Id;
	GPIO_pin_t Pin;

	int buttonState;       // the current reading from the input pin
	int lastButtonState;   // the previous reading from the input pin
	unsigned long lastDebounceTime;  // the last time the output pin was toggled
};

class ButtonsCommanderSwitch : public ButtonsCommanderButton
{
 private:
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	
	byte IdSize;
	byte IdAddCounter;
	byte IdLoopCounter;
	IdPin *pId;
	int IdState;

public:
	ButtonsCommanderSwitch(byte inIdNumber);
	
	void Setup();
	void AddId(unsigned long inId, int inPin);
	unsigned long Loop();
	inline unsigned long GetCurrentLoopId() const { return this->pId[this->IdState].Id; }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
