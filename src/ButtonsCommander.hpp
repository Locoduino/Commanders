//-------------------------------------------------------------------
#ifndef __buttonsCommander_H__
#define __buttonsCommander_H__
//-------------------------------------------------------------------

#ifndef NO_BUTTONSCOMMANDER
#include "Commander.hpp"
#include "ButtonsCommanderButton.hpp"

#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPUSH
#include "ButtonsCommanderPush.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES
#include "ButtonsCommanderAnalogPushes.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERSWITCH
#include "ButtonsCommanderSwitch.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERENCODER
#include "ButtonsCommanderEncoder.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#include "ButtonsCommanderPotentiometer.hpp"
#endif
#endif

//-------------------------------------------------------------------

class ButtonsCommander : Commander
{
	private:
		ButtonsCommanderButton *pLastSelectedButton;

		// Start of the linked list of all buttons. Each button have the address of its follower or NULL !
		ButtonsCommanderButton *pFirstButton;

	public:
		ButtonsCommander();

		void begin();
		ButtonsCommanderButton *Add(ButtonsCommanderButton *inpButton);
		inline ButtonsCommanderButton *GetLastSelectedButton() const { return this->pLastSelectedButton; }
		ButtonsCommanderButton* GetFromId(unsigned long inId) const;
		void RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

	public:
		unsigned long loop();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
