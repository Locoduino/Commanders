//-------------------------------------------------------------------
#ifndef __buttonsCommander_H__
#define __buttonsCommander_H__
//-------------------------------------------------------------------

#ifndef NO_BUTTONSCOMMANDER
#include "Commander.hpp"
#include "ButtonsCommanderButton.hpp"

//-------------------------------------------------------------------

class ButtonsCommander : Commander
{
	private:
		byte size;
		byte addCounter;
		ButtonsCommanderButton* *pButtons;
		ButtonsCommanderButton *pLastSelectedButton;

	public:
		ButtonsCommander();

		void Setup(byte inSize);
		void Setup(byte inSize, ButtonsCommanderButton *inpFirstState, ...);
		void Set(byte inIndex, ButtonsCommanderButton *inpState);
		byte Add(ButtonsCommanderButton *inpButton);
		inline ButtonsCommanderButton *operator[](byte idx) { return this->pButtons[idx]; }
		int IndexOf(ButtonsCommanderButton *inpbutton);
		inline ButtonsCommanderButton *GetLastSelectedButton() const { return this->pLastSelectedButton; }

	public:
		unsigned long Loop();

public:
#ifdef DEBUG_MODE
	void CheckIndex(byte inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
