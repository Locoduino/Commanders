//-------------------------------------------------------------------
#ifndef __buttonsCommanderKeyboard_H__
#define __buttonsCommanderKeyboard_H__
//-------------------------------------------------------------------

#include "../src/ButtonsCommanderButton.hpp"

#define KEYBOARD(list, nb)	((ButtonsCommanderKeyboard *) list[nb])

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

	void Setup(int inKey) { this->key = inKey; lastLoopKey = 0; }
	BasicsCommanderEvent Loop();
	void EndLoop();
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
