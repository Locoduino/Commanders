//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitch_H__
#define __buttonsCommanderSwitch_H__
//-------------------------------------------------------------------

#include "Commanders.h"

//-------------------------------------------------------------------

#ifdef DEBUG_MODE
#define CHECKPORT(type, val, text)	CheckPortNb(type, val, F(text))
//#define CHECKPIN(val, text)		Driver::CheckPinNb(val, F(text))
#else
#define CHECKPORT(type, val, text)
//#define CHECKPIN(val, text)
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
	
	void begin();
	void AddId(unsigned long inId, int inPin);
	unsigned long loop();
	inline unsigned long GetCurrentLoopId() const { return this->pId[this->IdState].Id; }
};

#define SWITCH_BUTTON(name, pin, ID) \
	ButtonsCommanderSwitch *name = new ButtonsCommanderSwitch(1); \
	name->AddId(ID); \
	name->begin(); \
	macro_buttons.Add(name);

#define SWITCH_BUTTON_2ID(name, pin, ID1, ID2) \
	ButtonsCommanderSwitch *name = new ButtonsCommanderSwitch(2); \
	name->AddId(ID1); \
	name->AddId(ID2); \
	name->begin(); \
	macro_buttons.Add(name);

#define SWITCH_BUTTON_IDS(name, pin, number) \
	ButtonsCommanderSwitch *name = new ButtonsCommanderSwitch(number); \
	name->begin(); \
	macro_buttons.Add(name);

#define SWITCH_BUTTON_ADDID(name, pin, ID) \
	name->AddId(ID, pin);

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
