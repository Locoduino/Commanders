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
	static int lastLoopKey;

 public:
	ButtonsCommanderKeyboard(unsigned long inId);

	void begin(int inKey) { this->key = inKey; lastLoopKey = 0; }
	unsigned long loop();
	void EndLoop();
};


//-------------------------------------------------------------------
#endif
#endif
