//-------------------------------------------------------------------
#ifndef __buttonsCommanderPush_H__
#define __buttonsCommanderPush_H__
//-------------------------------------------------------------------

#define  GPIO2_PREFER_SPEED    1
#ifdef VISUALC
	#include "VStudio/arduino2.hpp"
#else
	#include "arduino2.hpp"
#endif

#include "ButtonsCommanderButton.hpp"

#define PUSH(list, nb)	((ButtonsCommanderPush *) list[nb])

#ifdef DEBUG_MODE
#define CHECKPORT(type, val, text)	CheckPortNb(type, val, F(text))
#define CHECKPIN(val, text)		Driver::CheckPinNb(val, F(text))
#else
#define CHECKPORT(type, val, text)
#define CHECKPIN(val, text)
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
	ButtonsCommanderPush(byte inIdNumber = 1);
	ButtonsCommanderPush(unsigned long inId);

	void Setup(int inButtonPin);
	void AddId(unsigned long inId);
	unsigned long Loop();
	void EndLoop();

	inline int GetPin() const { return GPIO_to_Arduino_pin(this->buttonPin); }
	inline GPIO_pin_t GetPin2() const { return this->buttonPin; }
	inline int GetState() const { return this->buttonState; }
	inline unsigned long GetCurrentLoopId() const { return this->pId[this->IdLoopCounter]; }
	
	static ButtonsCommanderPush Empty;
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
