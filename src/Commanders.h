//-------------------------------------------------------------------
#ifndef __commanders_H__
#define __commanders_H__
//-------------------------------------------------------------------

/**	 @mainpage
A full explanation in French can be read at http://www.locoduino.org/spip.php?article165 .

The goal of this library is to virtualize input through a lot of ways (hardware buttons, bus, radio and so on)
in order to be able to drive the rest of the sketch without taking care of the way the information was got.

Passing this information from the commanders through the accessories is done by events of type COMMANDERS_TYPE_EVENT.
Each event have an Id, a type (MOVE, TOGGLE, MOVEPOSITION, LEFT, RIGHT, TOP, BOTTOM, etc... An additional integer
is added with some events to complete the event information : the position value for MOVEPOSITION for instance.

Each kind of Commanders can be excluded from compilation, for memory problems purpose or for naming conflicts.
Inside some Commanders, sub items can be excluded too, like the different buttons in the ButtonsCommander. 
You can exclude some parts of library here, to avoid losing program and data memory on parts you don't use.
For example, if you don't want DCC railroad modelling command protocol, 
just uncomment the line #define NO_DCCCOMMANDER by removing // at the beginning.

CANCommander is excluded by default, because this is a rare usage of the library, and the associated CAN library mcp_can
consume memory for nothing if this commander is not used.

The Arduino IDE compiles all the files of the library 'src' directory even if its content is not used.
The result is, if an unused source file contains static declarations (SERIAL, DCC or CAN do !), these statics 
will be allocated and this memory will be lost. The only solution I have found is to rename the source files to
something IDE don't know...
So if you want to lose less memory, you can rename the linked files from .cpp to .ori, and from .hpp to .hppori.
See below in the exclusion area the file names related to each exclusion.
This software is Copyright (C) 2015-2018 thierry Paris / Locoduino. Use is subject to license
conditions. The main licensing options available are GPL V2 or Commercial:

\par Open Source Licensing GPL V2
This is the appropriate option if you want to share the source code of your
application with everyone you distribute it to, and you also want to give them
the right to share who uses it. If you wish to use this software under Open
Source Licensing, you must contribute all your source code to the open source
community in accordance with the GPL Version 2 when your application is
distributed. See http://www.gnu.org/copyleft/gpl.html

\par Commercial Licensing
This is the appropriate option if you are creating proprietary applications
and you are not prepared to distribute and share the source code of your
application. Contact info@open.com.au for details.

\page Revision History
\par 15/07/2018 V1.61.1
- Correction du format de Keywords.txt
- Cr�ation de LIBRARY_VERSION
- Mise en conformit� de la partie doc HTML avec DOXYGEN_SPECIFIC
- Ajout du fichier LICENSE
_______________
- Fix Keywords.txt format.
- LIBRARY_VERSION created
- Update of DOXYGEN_SPECIFIC part for HTML documentation.
- Add of LICENSE file

\par 12/07/2018 1.61.0
- Correction de ButtonsCommanderSwitch
- Am�lioration de la doc dans Commanders.h pour �viter les #define NO_* ...
_______________
- Fix of ButtonsCommanderSwitch.
- Improvement of the documentation to avoid influence of #define NO_* inside Commanders.h

\par 21/05/2018 1.60.2
- Correction de ButtonsCommanderAnalogPushes
_______________
- Fix of ButtonsCommanderAnalogPushes.

\par 20/05/2018 1.60.1
- Correction des exemples SimpleButtons et SimpleButtonsByIds
_______________
- Fix of samples SimpleButtons and SimpleButtonsByIds.

\par 14/04/2018 1.60.0
- Ajout de EventsSequencer
- Ajout d'un exemple pour EventsSequencer
- Correction de l'exemple FullDeclaration
_______________
- Added EventsSequencer
- Added a sample for EventsSequencer
- Fix of FullDeclaration sample.

\par 16/02/2018 1.51.2
- Correction de la syntaxe de la documentation anglaise.
_______________
- Fix of English documentation syntax !

\par 27/10/2017 1.51.1
- Correction des exemples pour tester la bonne configuration de Commanders.h
_______________
- Fix all samples to have a better check of Commanders.h configuration.

\par 18/10/2017 1.51.0
- Mise en forme du num�ro de version en 'semver' (Semantic Versioning 2.0.0).
- Ajout de ArduiEmulator dans extras.
_______________
- Version number converted to 'semver' format (Semantic Versioning 2.0.0).
- ArduiEmulator added in extras folder.

\par 10/08/2017 1.50
- Nouvelle doc en anglais dans extras/Doc .
- Retrait de DccCommanderClass.CheckIndex()
- Ajout de Commander.BeforeFirstLoop() et Commander.printCommanders()
- Renommage des fonctions de EventStack.
- Passage de statique classe � statique fichier de LastEventId dans I2CCommander
_______________
- New complete English documentation in extras/Doc
- Removing of DccCommanderClass.CheckIndex()
- Add Commander.BeforeFirstLoop() and Commander.printCommanders()
- Renaming of EventStack functions.
- Pass from static of class to file static of LastEventId in I2CCommander.

\par 14/07/2017 1.40
- Nouvelles classes ButtonscommanderSwitchOnePin et ButtonscommanderSwitchTwoPins pour g�rer les cas les plus fr�quents sur des switchs...
- Nouvelle fonction virtuelle BeforeFirstLoop() dans ButtonsCommander et certains boutons.
- Ajout d'un param�tre MoveIncrement sur les encodeurs.
- Ajouts de messages de compilation 'No *** Commander' quand un NO_***COMMANDER a �t� d�clar�.
_______________
- New classes ButtonscommanderSwitchOnePin and ButtonscommanderSwitchTwoPins to handle the more common cases for switch usage.
- New function BeforeFirstLoop() for ButtonsCommander and some buttons.
- New parameter MoveIncrement for encoders.
- New compilation messages 'No *** Commander' when a NO_***COMMANDER has been defined.

\par 21/04/2017 1.31
- ButtonsCommanderSwitch.AddEvent() retourne l'�v�nement cr�� au cas o�...
- Correction de ButtonscommanderSwitch.begin() pour s�lectionner le bon id au lancement.
- Correction de ButtonsCommanderEncoder.loop() qui retournait le mauvais �v�nement.
_______________
- ButtonsCommanderSwitch.AddEvent() returns the newly created event.
- Fix ButtonscommanderSwitch.begin() to select the right Id at launch.
- Fix ButtonsCommanderEncoder.loop() for bad event type returned.

\par 09/04/2017 1.30
- Ajout des macros PRINT_COMMANDERS, COMMANDERS_PRINT_EVENT et PRINT_EVENT_DCC.
- Ajout d'une EventStack pour pouvoir �mettre plusieurs events pendant un seul loop.
- Ajout d'un �v�nement COMMANDER_MOVE_OFF au basculement d'un switch.
- Initialisation d'un switch par la lecture de son �tat pendant le setup.
- Test sur la pr�sence d'au moins un �tat pour un switch.
- Retrait d'un pinMode inutile pour les switchs.
- Retrait d'une g�n�ration d'�v�nement inutile pour un encodeur.
- Ajout d'une fonction SetMiniMaxi pour les potentiom�tres.
- Test de la pr�sence de la biblioth�que mcp_can si le CAN commander est activ�.
_______________
- Macros PRINT_COMMANDERS, COMMANDERS_PRINT_EVENT and PRINT_EVENT_DCC added.
- EventStack sources added to be able to push more than one event during one loop() call.
- COMMANDER_MOVE_OFF event generated when a switch toggle.
- Initialisation of switch current value at startup, druing setup.
- Check state number for a switch.
- A overnumbered pinMode has been deleted for switch.
- A wrong event has benn removed for encoders.
- SetMiniMaxi function added for potentiometers.
- Check for mcp_can library installed if CAN commander is activated.

\par 24/02/2017 1.25
- Correction de l'include "Dio2.h" en <Dio2.h>
_______________
- Fix "Dio2.h" include into <Dio2.h>

\par 31/01/2017 1.24
- Ajout de DCC_Monitor_Accessories.zip dans extra.
- Ajout d'infpormartions sur extra dans readme.adoc .
- Renommage de CHAIN_ENUMERATE en CMDRSCHAIN_ENUMERATE
_______________
- DCC_Monitor_Accessories.zip added to extra.
- Information added about extra directory in readme.adoc file.
- CHAIN_ENUMERATE renamed in CMDRSCHAIN_ENUMERATE.

\par 31/12/2016 1.23
- Mise � jour de DIO2
_______________
- Included library DIO2 updated

\par 21/11/2016 1.22
- Correction de compilation avec printCommanders si pas de COMMANDERS_PRINT_COMMANDERS
_______________
- Fix compilation if no COMMANDERS_PRINT_COMMANDERS defined.

\par 19/11/2016 1.21
- Ajout de printCommanders() pour avoir le d�tail de ce qui est d�clar�. Activable avec #define COMMANDERS_PRINT_COMMANDERS
_______________
- printCommanders() added to show on console all the declared commanders. Must be activated with #define COMMANDERS_PRINT_COMMANDERS

\par 04/11/2016 1.20
- Correction de DccCommander qui ne marchait plus...
_______________
- Fix of DccCommander...

\par 31/10/2016 1.11
- Changement de nom pour �viter des collisions de nom avec d'autres biblios : DEBUG_VERBOSE->COMMANDERS_DEBUG_VERBOSE.
- Ajout des biblioth�ques DIO2 et mcp-can dans 'extra'.
- event.h ne sera plus inclus dans un croquis utilisant Commanders.
_______________
- Name changed from DEBUG_VERBOSE to COMMANDERS_DEBUG_VERBOSE, to avoid confusion with other libraries.
- DIO2 and mcp_can libraries included in 'extra'.
- event.h will not be included anymore when Commanders will be used in a sketch.

\par 28/09/2016 1.1
- D�claration de NO_CANCOMMANDER par d�faut.
_______________
- NO_CANCOMMANDER is the default

\par 09/09/2016 1.0
- Inversion de la broche et de l'identifiant dans AddEvent du switch pour �tre homog�ne avec le poussoir.
Idem pour l'encodeur et le potentiometre.
- Cr�ation du source 'Events.h' pour ne pas avoir � recopier les enums dans les exemples.
_______________
- Inverted pin and id in potentiometer, switch and encoder constructors
- Created 'Events.h' to avoid copying enums in samples.

\par 04/09/2016 0.99
- Plus d'initialisation de Serial, pour le DUE, m�me dans SerialCommander .
Serial.begin() doit �tre fait dans le .ino .
- Ajout de l'�v�nement COMMANDERS_EVENT_MOVEPOSITIONID.
- un begin() remplace SetEventHandler et SetStatusLedPin pour Commanders.
- sur une (fausse) intuition, retrait de tous les 'byte' remplac�s par des uint8_t .
_______________
- No more internal begin() of Serial class, to avoid problems with the DUE, even in SerialCommander.
Serial.begin must be done, if necessary, in .ino file .
- Added event COMMANDERS_EVENT_MOVEPOSITIONID.
- A begin() function replace the old SetEventHandler and SetStatusLedPin for Commanders.
- Based on a false deduction, replacement of all 'byte' by 'uint8_t'. It is not wrong, but it
could have been avoided...
_______________
\par 21/08/2016 0.98
- Ajout de l'�v�nement COMMANDERS_EVENT_MOVEPOSITIONINDEX.
- Ajout de la syntaxe id:acc pour le SerialCommander
_______________
- Added event COMMANDERS_EVENT_MOVEPOSITIONINDEX
- Added new syntax for Dcc Id in SerialCommander

\par 16/08/2016 0.97
- Correction de Chain.hpp avec des pointeurs
- Adaptation de ButtonsCommanderKeyboard par rapport � ButtonsCommanderPush
_______________
- Fix Chain.hpp with pointers
- Upgrade ButtonsCommanderKeyboard to ButtonsCommanderPush

\par 12/06/2016 V0.96
- Retrait de StartSetup et EndSetup
- Correction des exemples.
_______________
- StartSetup and EndSetup removed
- Samples fixed.

\par 05/06/2016 V0.95
- D�placement de RaiseEvent de Commander � Commanders.
- Correction de Chain.hpp
_______________
- RaiseEvent() moved from Commander to Commanders.
- chain.hpp fixed

\par 25/05/2016 V0.94
- Ajout de STRAIGHT et DIVERGE dans les �v�nements
_______________
- Added events STRAIGHT and DIVERGE.

\par 22/05/2016 V0.93
- Modification des �v�nements pour un MOVE multiple.
- Optimisation m�moire et code.
- #define NO_BUTTONSCOMMANDER qui marche...
_______________
- Events modified to add a MOVE with arguments.
- Code and central memory optimized.
- #define NO_BUTTONSCOMMANDER works now.

\par 19/05/2016 V0.92
- Retrait des macros
- Simplification de l'�criture pour les utilisateurs par l'instanciation automatique des commanders et la suppression des
arguments des constructeurs renvoy�s vers les begin(). Plus de pointeurs � manipuler !
- Ajout de test.bat pour compiler automatiquement les exemples et obtenir un rapport.
_______________
- Macros removed.
- Simplification of code writing for library users by automatic instanciation of commanders,
and removing of all constructor arguments moved to the begin() functions. No more pointers in the .ino file !
- Added test.bat to check syntax of all examples and reading a final text report.

\par 17/05/2016 V0.91
- Correction de DccCommander qui ne marchait pas du tout !
- Exemple DccMacro adapt�.
_______________
- Fix of DccCommander.

\par 15/05/2016 V0.90
- Ajout de l'�v�nement CONFIG avec un octet pour l'adresse, et un autre pour la valeur.
_______________
- Added CONFIG event, with a byte for the address, and another one for the value.

\par 03/05/2016 V0.82
- Correction de l'exemple CANCommander.
- Mise � jour de DIO2 dans extra.
- Renommage de AddId en AddEvent dans les switchs.
- Ajout des masques et des filtres dans CANCommander.
_______________
- Fixes of the sample CANCommander.
- Update of DIO2 library in extra.
- Rename of AddId by AddEvent in switches
- Added masks and filters in CANCommander.

\par 03/05/2016 V0.81
- I2C fonctionne !
- Deux exemples simplissimes de r�cepteurs CAN et I2C ajout�s.
_______________
- I2C is working !
- Two new samples with basics CAN and I2C receptors.

\par 02/05/2016 V0.80
- Utilisation de listes chain�es (avec le template Chain.hpp) pour �viter les dimensionnements inutiles.
- Ajout du type d'�v�nement et du data associ� dans les push et les switches.
- Ajout des �v�nements MOVELEFT, MOVERIGHT et MOVESTOP . SELECTED devient TOGGLE.
- Utilisation de la biblioth�que externe DIO2 au lieu de la version interne Arduino2.
- Pour �viter des conflits, DEBUG_MODE est renomm� en COMMANDERS_DEBUG_MODE.
- NULL est utilis� aussi souvent que possible.
- CANCommander fonctionne ! Mais seulement en r�ception. Un exemple d'�meteur a �t� ajout�.
_______________
- Use of chained lists (via the template Chain.hpp) instead of classic lists.
- Event types and associated data added for event lists in pushes and switches.
- New events MOVELEFT, MOVERIGHT and MOVESTOP added. SELECTED becomes TOGGLE.
- Using external DIO2 library instead of internal Arduino2.
- To avoid conflicts, DEBUG_MODE has been renamed in COMMANDERS_DEBUG_MOSE.
- NULL is now used as frequently as possible.
- CANCommander works, but in reception only. A sample of sender is added in examples folder.

\par 27/04/2016 V0.72
- Renommage de Setup en begin dans les sources Dcc.
_______________
- Rename of Setup to begin in Dcc sources.

\par 25/04/2016 V0.71
- Ajout de I2CCommander, en receveur uniquement pour des questions de timing.
_______________
- I2CCommander added. Only a receiver to avoid timing problems.

\par 20/04/2016 V0.70
- Ajout de TextInterpreter.
- Utilisation de TextInterpreter dans SerialCommander
- Exclusion de CANCommander quand NO_CANCOMMANDER d�fini.
_______________
- TextInterpreter added.
- Use of TextInterpreter inside SerialCommander.
- Exclusion of CANCommander when asked with define NO_CANCOMMANDER.

\par 05/04/2016 V0.64
- Ajout d'une Led de statut commune � tous les Commanders.
- Renommage de tous les 'Setup' en 'begin' et 'Loop' en 'loop'.
_______________
- Added a global status Led for all the commanders.
- All 'Setup' have been renamed in 'begin' and 'Loop' in 'loop'.

\par 02/04/2016 V0.63
- Correction du constructeur de ButtonsCommanderPush avec le nombre d'ids pass�s maintenant en argument du setup.
- Ajout d'une macro PUSH_BUTTON_IDS pour les ID multiples d'un poussoir.
- Ajout d'une macro ANALOGPUSH_BUTTON_TOL pour les boutons poussoir en Charlieplexing avec une tol�rance.
- Correction d'un probl�me de compilation de ButtonsCommanderButton
- Correction de SerialCommander dans l'interpr�tation du texte...
- ButtonsCommanderEncoder peut maintenant envoyer un �v�nement absolu avec une valeur entre mini et maxi,
ou un �v�nement relatif avec +1 ou -1.
_______________
- Fix ButtonsCommanderPush constructor, number of multiple ids now passed to the setup function.
- Added PUSH_BUTTON_IDS macro for push button with multiple ids.
- Added ANALOGPUSH_BUTTON_TOL for charlieplexing pushes with tolerancy.
- Fix commpilation problem with	ButtonsCommanderButton
- Fix SerialCommander in text parsing.
- ButtonsCommanderEncoder can now raise an Absolute event, with an associated value between mini and maxi,
or a Relative event, with +1 or -1 as value.

\par 29/03/2016 V0.62
- Changement de syntaxe des macros g�n�rales avec COMMANDERS au lieu de COMMANDER
- Suppression de point-virgules � la fin des macros.
- DCCACCESSORY devient DCCACTIVATION
- Correction de la macro ANALOGPUSH_BUTTON .
_______________
- Name change of global macros from COMMANDER to COMMANDERS
- Semi-colon removed at the end of macros.
- DCCACCESSORY changed to DCCACTIVATION
- Fix ANALOGPUSH_BUTTON macro.

\par 25/03/2016 V0.61
- Correction de probl�mes de consommation m�moire.
_______________
- Fix memory Consumption in events.

\par 19/03/2016 V0.60
- Correction et am�liorations du SerialCommander. Par macros, et compatible avec tous les types de serial class...
- Compatibilit� ajout�e avec l'Arduino Due, mais sans b�n�ficier de l'acc�l�ration promise par l'acc�s direct aux ports.
_______________
- Fix of SerialCommander, to be able to use any kind of serial class, from the hardware serials of Arduinos, to
software emulations by AltSoftSerial or SoftwareSerial .
- Added Arduino Due compatibility, but by using the classic IDE functions to access the pins, not the fast direct
addressing of processor ports.

\par 18/03/2016 V0.50
- Nouveau nom 'Commanders' pour la biblioth�que. Les classes et les defines ont suivi le mouvement.
- Un moyen simple de programmer � l'aide de macros C a �t� ajout�.
_______________
- New name 'Commanders' for the library. All classes and defines have been adapted when necessary.
- An easy programming way have been added based on C macros.

\par 20/02/2016 V0.40
- Correction des Ids multiples de PushButton.
- Ajout de la classe ButtonsCommanderAnalogPushes
- ButtonsCommander::Loop renvoie une structure avec l'id du bouton s�lectionn� ou UNDEFINED_ID. LastSelectedButton garde en m�moire le dernier,
m�me pour un bouton composite comme ButtonsCommanderAnalogPushes. Dans ce cas c'est l'item qui est conserv�.
- Un exemple SimpleButtonsByIds a �t� ajout� pour tester cette fa�on de fonctionner.
_______________
- Fix multiple ids for push button.
- New class ButtonsCommanderAnalogPushes added.
- Now the ButtonsCommander::Loop function returns the selected id or UNDEFINED_ID. LastSelectedButton is the ...
Last selected button ! Works also with composite buttons like ButtonsCommanderAnalogPushes. In this case, this is the item
which is kept.
- A sample SimpleButtonsByIds has been added to test this mode.

\par 22/12/2015 V0.30
- EventHandler impl�ment� et fonctionnel
- Simplification de la construction des commanders avec des listes chain�es
- Ajout du traitement clavier pour les tests PC-Windows.
_______________
- EventHandler is implemented and operational.
- Commander building in .ino file is more simpler with transparent linked lists.
- Keyboard handling added for PC-Windows tests.

\par 20/12/2015 V0.20
- Gros gain de m�moire dynamique entre DccCommander avec sa liste de CVs trop longue, et surtout SerialCommander qui
utilisait les Serial* du noyau, chacun allouant 150 octets de buffer interne en dur !
_______________
- Big fix on dynamic memory size, because of CV list in DccCommander, but mostly because of SerialCommander which
reference the Serial* default classes from the kernel, each one allocating 150 bytes of internal buffer !

\par 01/09/2015 V0.10
- Cr�ation de la librairie, extraite de ma librarie UniversalAccessoryDecoder.
_______________
- Creation, extracted from my UniversalAccessoryDecoder library v4.
*/

/** @file Commanders.h
Main include file of the library.*/

#define LIBRARY_VERSION		"Commanders V1.61.1"

///////////////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to be in debug mode.
#define COMMANDERS_DEBUG_MODE

//////////////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to be in verbose debug mode.
//#define COMMANDERS_DEBUG_VERBOSE_MODE

//////////////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to activate the print function.
#define COMMANDERS_PRINT_COMMANDERS

/** This is used internally by DIO2.h */
#define  GPIO2_PREFER_SPEED    1

#include <DIO2.h>

//////////////////////////////////////////
//  Exclusion area
//
//NO_CANCOMMANDER
//	CANCommander.cpp
//	CANCommander.hpp
//
//NO_DCCCOMMANDER
//	DccCommander.cpp
//	DccCommander.hpp
//	DCC_Decoder.cpp
//	DCC_Decoder.hpp
//
//NO_I2CCOMMANDER
//	I2CCommander.cpp
//	I2CCommander.hpp
//
//NO_SERIALCOMMANDER
//	SerialCommander.hpp
//	TextInterpreter.cpp
//	TextInterpreter.hpp
//
//NO_BUTTONSCOMMANDER
//	ButtonsCommander.cpp
//	ButtonsCommander.hpp
//	ButtonsCommanderButtons.cpp
//	ButtonsCommanderButtons.hpp
//	ButtonsCommanderEncoder.cpp
//	ButtonsCommanderEncoder.hpp
//	ButtonsCommanderPush.cpp
//	ButtonsCommanderPush.hpp
//	ButtonsCommanderSwitch.cpp
//	ButtonsCommanderSwitch.hpp
//	ButtonsCommanderPotentiometer.cpp
//	ButtonsCommanderPotentiometer.hpp
//
//NO_BUTTONSCOMMANDERENCODER
//	ButtonsCommanderEncoder.cpp
//	ButtonsCommanderEncoder.hpp
//
//NO_BUTTONSCOMMANDERPUSH
//	ButtonsCommanderPush.cpp
//	ButtonsCommanderPush.hpp
//
//NO_BUTTONSCOMMANDERANALOGPUSHES
//	ButtonsCommanderAnalogPush.cpp
//	ButtonsCommanderAnalogPush.hpp
//
//NO_BUTTONSCOMMANDERSWITCH
//	ButtonsCommanderSwitch.cpp
//	ButtonsCommanderSwitch.hpp
//
//NO_BUTTONSCOMMANDERPOTENTIOMETER
//	ButtonsCommanderPotentiometer.cpp
//	ButtonsCommanderPotentiometer.hpp
//
//NO_EVENTsSEQUENCER
//	EventsSequencer.cpp
//	EventsSequencer.hpp

//#define NO_BUTTONSCOMMANDER
//#define NO_BUTTONSCOMMANDERENCODER
//#define NO_BUTTONSCOMMANDERPUSH
//#define NO_BUTTONSCOMMANDERANALOGPUSHES
//#define NO_BUTTONSCOMMANDERSWITCH
//#define NO_BUTTONSCOMMANDERPOTENTIOMETER
#define NO_CANCOMMANDER
#define NO_DCCCOMMANDER
#define NO_I2CCOMMANDER
//#define NO_SERIALCOMMANDER
//#define NO_EVENTSSEQUENCER

#ifdef DOXYGEN_SPECIFIC
			// DO NOT CHANGE THESE LINES IN THIS BLOCK 'DOXYGEN_SPECIFIC' : Only here for library documentation !

			/** If this is defined, all the buttons part of the library is removed from the compilation. 
			It can result in a smaller memory footprint for the final program.*/
			#define NO_BUTTONSCOMMANDER
			/** If this is defined, the encoders buttons part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_BUTTONSCOMMANDERENCODER
			/** If this is defined, the push buttons part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_BUTTONSCOMMANDERPUSH
			/** If this is defined, the analog push buttons part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_BUTTONSCOMMANDERANALOGPUSHES
			/** If this is defined, the switches part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_BUTTONSCOMMANDERSWITCH
			/** If this is defined, the potentiometer part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_BUTTONSCOMMANDERPOTENTIOMETER

			/** If this is defined, the full bus CAN part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_CANCOMMANDER
			/** If this is defined, the DCC railroad modelling protocol command part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_DCCCOMMANDER
			/** If this is defined, the I2C bus part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_I2CCOMMANDER
			/** If this is defined, the serial communication part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_SERIALCOMMANDER
			/** If this is defined, the sequencer of events of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_EVENTSSEQUENCER

			#undef NO_BUTTONSCOMMANDER
			#undef NO_BUTTONSCOMMANDERENCODER
			#undef NO_BUTTONSCOMMANDERPUSH
			#undef NO_BUTTONSCOMMANDERANALOGPUSHES
			#undef NO_BUTTONSCOMMANDERSWITCH
			#undef NO_BUTTONSCOMMANDERPOTENTIOMETER
			#undef NO_CANCOMMANDER
			#undef NO_DCCCOMMANDER
			#undef NO_I2CCOMMANDER
			#undef NO_SERIALCOMMANDER
			#undef NO_EVENTSSEQUENCER

			/** If this is defined, the library will do many checks during setup and execution, and print errors, warnings and
			information messages on console. These messages can take a lot of memory, so be careful about the free memory of
			your program if you activate debug mode.*/
			#define COMMANDERS_DEBUG_MODE
			/** If this is defined, the Verbose mode lets you see all actions done by the  library, but with a real flood of
			text to the console... It has no effect if COMMANDERS_DEBUG_MODE is not activated.*/
			#define COMMANDERS_DEBUG_VERBOSE_MODE
			/** If this is defined, the function Commanders::printCommanders() will become available. This is useful to try
			to understand why a commander, or a commander item is not correctly defined.
			This function uses a lot of memory, so activate it only if necessary, and be careful about your program's memory.
			You can use the define PRINT_COMMANDERS() in your sketch instead of a call to Commanders::printCommanders().
			If COMMANDERS_PRINT_COMMANDERS is not defined, PRINT_COMMANDERS is defined as empty, so you will not have a compilation error.*/
			#define COMMANDERS_PRINT_COMMANDERS
#endif

/////////////////////////////////////

#include "Commander.hpp"
#include "Commanders.hpp"

#ifndef NO_CANCOMMANDER
#include "CANCommander.hpp"
#endif

#ifndef NO_DCCCOMMANDER
#include "DccCommander.hpp"
#endif

#ifndef NO_I2CCOMMANDER
#include "I2CCommander.hpp"
#endif

#ifndef NO_BUTTONSCOMMANDER
#include "ButtonsCommander.hpp"
#endif

#ifndef NO_SERIALCOMMANDER
#include "SerialCommander.hpp"
#include "TextInterpreter.hpp"
#endif

#ifndef NO_EVENTSSEQUENCER
#include "EventsSequencer.hpp"
#endif

#endif