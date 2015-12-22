//-------------------------------------------------------------------
#ifndef __buttonsCommanderKeyboard_H__
#define __buttonsCommanderKeyboard_H__
//-------------------------------------------------------------------

#include "../ButtonsCommanderButton.hpp"

#define KEYBOARD(list, nb)	((ButtonsCommanderKeyboard *) list[nb])

//-------------------------------------------------------------------
// A keyboard button is a used by debugging in VisualStudio.
//-------------------------------------------------------------------

class ButtonsCommanderKeyboard : public ButtonsCommanderButton
{
 private:
	char key;	// character to scan...

 public:
	static char lastLoopKey;

 public:
	ButtonsCommanderKeyboard(unsigned long inId);

	void Setup(char inKey) { this->key = inKey; lastLoopKey = 0; }
	unsigned long Loop();
	void EndLoop();
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
