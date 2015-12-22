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
		ButtonsCommanderButton *pLastSelectedButton;

		// Start of the linked list of all buttons. Each button have the address of its follower or NULL !
		ButtonsCommanderButton *pFirstButton;

	public:
		ButtonsCommander();

		void Setup();
		ButtonsCommanderButton *Add(ButtonsCommanderButton *inpButton);
		inline ButtonsCommanderButton *GetLastSelectedButton() const { return this->pLastSelectedButton; }

	public:
		unsigned long Loop();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
