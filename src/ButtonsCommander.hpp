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
		ButtonsCommanderButton* GetFromId(unsigned long inId) const;
		void RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

	public:
		unsigned long Loop();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
