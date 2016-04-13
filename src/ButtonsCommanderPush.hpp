//-------------------------------------------------------------------
#ifndef __buttonsCommanderPush_H__
#define __buttonsCommanderPush_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifdef DEBUG_MODE
#define CHECKPORT(type, val, text)	CheckPortNb(type, val, F(text))
//#define CHECKPIN(val, text)		Driver::CheckPinNb(val, F(text))
#else
#define CHECKPORT(type, val, text)
//#define CHECKPIN(val, text)
#endif

//-------------------------------------------------------------------
// A push button is a hardware device giving a time limited impulsion.
// It will return one id at a time, passing through the whole list of Ids.
//-------------------------------------------------------------------

class ButtonsCommanderPush : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t buttonPin;	// the number of the pushbutton pin
	int buttonState;		// the current reading from the input pin
	int lastButtonState;	// the previous reading from the input pin

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	
	byte IdSize;
	byte IdAddCounter;
	byte IdLoopCounter;
	unsigned long *pId;

 public:
	ButtonsCommanderPush();
	ButtonsCommanderPush(unsigned long inId);

	void begin(int inButtonPin, byte inIdNumber = 1);
	void AddId(unsigned long inId);
	unsigned long loop();

	inline int GetPin() const { return GPIO_to_Arduino_pin(this->buttonPin); }
	inline GPIO_pin_t GetPin2() const { return this->buttonPin; }
	inline int GetState() const { return this->buttonState; }
	inline unsigned long GetCurrentLoopId() const { return this->pId[this->IdLoopCounter]; }
	
	static ButtonsCommanderPush Empty;
};

#define PUSH_BUTTON(name, pin, ID) \
	ButtonsCommanderPush *name = new ButtonsCommanderPush(ID); \
	name->begin(pin); \
	macro_buttons.Add(name);

#define PUSH_BUTTON_IDS(name, pin, number) \
	ButtonsCommanderPush *name = new ButtonsCommanderPush(); \
	name->begin(pin, number); \
	macro_buttons.Add(name);


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
