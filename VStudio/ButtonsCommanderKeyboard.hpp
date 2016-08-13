//-------------------------------------------------------------------
#ifndef __buttonsCommanderKeyboard_H__
#define __buttonsCommanderKeyboard_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER

//-------------------------------------------------------------------
// A keyboard button is a used by debugging in VisualStudio.
//-------------------------------------------------------------------

class ButtonsCommanderKeyboard : public ButtonsCommanderButton
{
 private:
	int key;	// character to scan...

 public:
	ButtonsCommanderKeyboard();

	void begin(unsigned long inId, int inKey) { this->Id = inId; this->key = inKey; ArduiEmulator::ArduinoForm::lastKeyPressed = 0; }
	unsigned long loop();
	void EndLoop();
};


//-------------------------------------------------------------------
#endif
#endif
