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
		unsigned char size;
		unsigned char addCounter;
		ButtonsCommanderButton* *pButtons;
		ButtonsCommanderButton *pLastSelectedButton;

	public:
		ButtonsCommander();

		void Setup(int inSize);
		void Setup(int inSize, ButtonsCommanderButton *inpFirstState, ...);
		void Set(unsigned char inIndex, ButtonsCommanderButton *inpState);
		unsigned char Add(ButtonsCommanderButton *inpButton);
		inline ButtonsCommanderButton *operator[](unsigned char idx) { return this->pButtons[idx]; }
		int IndexOf(ButtonsCommanderButton *inpbutton);
		inline ButtonsCommanderButton *GetLastSelectedButton() const { return this->pLastSelectedButton; }

	public:
		unsigned long Loop();

public:
#ifdef DEBUG_MODE
	void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
