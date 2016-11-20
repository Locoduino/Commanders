//-------------------------------------------------------------------
#ifndef __buttonsCommander_H__
#define __buttonsCommander_H__
//-------------------------------------------------------------------

#include "Commander.hpp"

#ifndef NO_BUTTONSCOMMANDER
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

#define ButtonsCommander ButtonsCommanderClass::GetCurrent()

//-------------------------------------------------------------------

class ButtonsCommanderClass : Commander
{
	private:
		ButtonsCommanderButton *pLastSelectedButton;

		// Start of the linked list of all buttons. Each button have the address of its follower or NULL !
		ButtonsCommanderButton *pFirstButton;

	public:
		ButtonsCommanderClass();

		void begin();
		ButtonsCommanderButton *Add(ButtonsCommanderButton *inpButton);
		inline ButtonsCommanderButton *GetLastSelectedButton() const { return this->pLastSelectedButton; }
		ButtonsCommanderButton* GetFromId(unsigned long inId) const;

		unsigned long loop();

	public:
		static ButtonsCommanderClass *pButtonsCommander;

	public:
		static ButtonsCommanderButton *AddButton(ButtonsCommanderButton *inpButton);
		static inline ButtonsCommanderClass &GetCurrent() {	return *(ButtonsCommanderClass::pButtonsCommander);}
#ifdef COMMANDERS_PRINT_COMMANDERS
		void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
