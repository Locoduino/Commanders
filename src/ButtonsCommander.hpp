//-------------------------------------------------------------------
#ifndef __buttonsCommander_HPP__
#define __buttonsCommander_HPP__
//-------------------------------------------------------------------

#include "Commander.hpp"

#ifndef NO_BUTTONSCOMMANDER
#include "ButtonsCommanderButton.hpp"

#ifndef NO_BUTTONSCOMMANDERPUSH
#include "ButtonsCommanderPush.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES
#include "ButtonsCommanderAnalogPushes.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERSWITCH
#include "ButtonsCommanderSwitch.hpp"
#include "ButtonsCommanderSwitchOnePin.hpp"
#include "ButtonsCommanderSwitchTwoPins.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERENCODER
#include "ButtonsCommanderEncoder.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#include "ButtonsCommanderPotentiometer.hpp"
#endif

#ifdef VISUALSTUDIO
#include "ButtonsCommanderKeyboard.hpp"
#endif

#define ButtonsCommander ButtonsCommanderClass::GetCurrent()

/** This Commanders receive orders from all kind of physical buttons.

This commander is mainly a list of buttons. This is not maintained by CMDRSCHAINEDLIST, but it is the same
way of work.
*/
class ButtonsCommanderClass : Commander
{
	private:
		// Start of the linked list of all buttons. Each button have the address of its follower or NULL !
		ButtonsCommanderButton *pFirstButton;
		static ButtonsCommanderClass *pButtonsCommander;

	public:
		/** Default constructor.*/
		ButtonsCommanderClass();

		/** Initialize the instance.*/
		void begin();
		/** This function will be called after sketch Setup(), but before the first call to loop().
		It will be called only one time.*/
		void beforeFirstLoop();
		/** Add a new button to the internal linked list.
		@param inpButton	Button to add.
		@return address of the inserted button.
		*/
		ButtonsCommanderButton *Add(ButtonsCommanderButton *inpButton);
		/** Gets a button address from its id.
		@param inId	id to find.
		@return Address of the found button or NULL if nothing found.
		@remark This is an internal function.
		*/
		ButtonsCommanderButton* GetFromId(unsigned long inId) const;

		/** Main loop function. */
		unsigned long loop();
#ifdef COMMANDERS_PRINT_COMMANDERS
		/** Print this Commander on the console.
		@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
		*/
		void printCommander();
#endif

	public:
		/** Add a new button to the internal linked list of the static ButtonsCommander.
		@param inpButton	Button to add.
		@return address of the inserted button.
		@remark This is an internal function.
		*/
		static ButtonsCommanderButton *AddButton(ButtonsCommanderButton *inpButton);
		/** Get the current instance of DccCommanderClass
		@remark This is an internal function.
		*/
		static inline ButtonsCommanderClass &GetCurrent() {	return *(ButtonsCommanderClass::pButtonsCommander);}
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
