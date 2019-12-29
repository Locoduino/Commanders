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
Inside some Commanders, sub items can be excluded too, like the different kind of buttons in the ButtonsCommander. 
You can exclude some parts of library here, to avoid losing program and data memory on parts you don't use.
For example, if you don't want DCC railroad modeling command protocol, 
just uncomment the line #define NO_DCCCOMMANDER by removing // at the beginning.

CANCommander is excluded by default, because this is a rare usage of the library, and the associated CAN library mcp_can
consume memory for nothing if this commander is not used.

This software is Copyright (C) 2015-2019 thierry Paris / Locoduino. Use is subject to license
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
\par 19/06/2019 V2.01
- Correction de ButtonsCommanderSwitch et dérivés qui n'envoyaient pas les bons événements (encore).
_______________

- Fix of ButtonsCommanderSwitch and inherited, not sending the right events at the right time ! (one more time...)

\par 30/12/2018 V2.00
- Compatibilité avec ESP32 : Analog, Encoder, ...
- Intégration du décodage DCC par la bibliothèque NmraDcc	(https://github.com/mrrwa/NmraDcc) amélioré pour l'ESP32.
- Correction de l'exemple DCC, Serial.begin manquant, COMMANDERS_DEBUG_MODE obligatoire...
- Correction de #define PRINT_EVENT_DCC()
- Correction de ButtonsCommanderSwitch et dérivés qui n'envoyaient pas les bons événements.
_______________

- Add support of ESP32 chips for analog devices, encoder, etc...
- Integration of NmraDcc library for Dcc decoding, giving support for ESPs, STMs (STM32 is not tested)...
- Fix of DCC sample, Serial.begin missing, COMMANDERS_DEBUG_MODE needs to be declared for this sample.
- Fix of PRINT_EVENT_DCC().
- Fix of ButtonsCommanderSwitch and inherited, not sending the right events at the right time !

\par 22/11/2018 V1.62
- Correction de ButtonsCommanderAnalogPushes qui ne marchait pas.
- Ajout de RaiseEventWhen() à DccCommander pour gérer les centrales type Ecos.
- Correction de l'affichage de debug d'une pin dans ButtonsCommanderSwitch
_______________
- Fix of ButtonsCommanderAnalogPushes.
- RaiseEventWhen() added to DccCommander to handle centrals like Ecos.
- Fix debug print of pin number in ButtonsCommanderSwitch

\par 21/10/2018 V1.61.2
- Améliorations de l'affichage de debug	sur la console
_______________
- Improvements of debug prints on serial window.

\par 15/07/2018 V1.61.1
- Correction du format de Keywords.txt
- Création de LIBRARY_VERSION
- Mise en conformité de la partie doc HTML avec DOXYGEN_SPECIFIC
- Ajout du fichier LICENSE
_______________
- Fix Keywords.txt format.
- LIBRARY_VERSION created
- Update of DOXYGEN_SPECIFIC part for HTML documentation.
- Add of LICENSE file

\par 12/07/2018 1.61.0
- Correction de ButtonsCommanderSwitch
- Amélioration de la doc dans Commanders.h pour éviter les #define NO_* ...
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
- Mise en forme du numéro de version en 'semver' (Semantic Versioning 2.0.0).
- Ajout de ArduiEmulator dans extras.
_______________
- Version number converted to 'semver' format (Semantic Versioning 2.0.0).
- ArduiEmulator added in extras folder.

\par 10/08/2017 1.50
- Nouvelle doc en anglais dans extras/Doc .
- Retrait de DccCommanderClass.CheckIndex()
- Ajout de Commander.BeforeFirstLoop() et Commander.printCommanders()
- Renommage des fonctions de EventStack.
- Passage de statique classe à statique fichier de LastEventId dans I2CCommander
_______________
- New complete English documentation in extras/Doc
- Removing of DccCommanderClass.CheckIndex()
- Add Commander.BeforeFirstLoop() and Commander.printCommanders()
- Renaming of EventStack functions.
- Pass from static of class to file static of LastEventId in I2CCommander.

\par 14/07/2017 1.40
- Nouvelles classes ButtonscommanderSwitchOnePin et ButtonscommanderSwitchTwoPins pour gérer les cas les plus fréquents sur des switchs...
- Nouvelle fonction virtuelle BeforeFirstLoop() dans ButtonsCommander et certains boutons.
- Ajout d'un paramètre MoveIncrement sur les encodeurs.
- Ajouts de messages de compilation 'No *** Commander' quand un NO_***COMMANDER a été déclaré.
_______________
- New classes ButtonscommanderSwitchOnePin and ButtonscommanderSwitchTwoPins to handle the more common cases for switch usage.
- New function BeforeFirstLoop() for ButtonsCommander and some buttons.
- New parameter MoveIncrement for encoders.
- New compilation messages 'No *** Commander' when a NO_***COMMANDER has been defined.

\par 21/04/2017 1.31
- ButtonsCommanderSwitch.AddEvent() retourne l'événement créé au cas où...
- Correction de ButtonscommanderSwitch.begin() pour sélectionner le bon id au lancement.
- Correction de ButtonsCommanderEncoder.loop() qui retournait le mauvais événement.
_______________
- ButtonsCommanderSwitch.AddEvent() returns the newly created event.
- Fix ButtonscommanderSwitch.begin() to select the right Id at launch.
- Fix ButtonsCommanderEncoder.loop() for bad event type returned.

\par 09/04/2017 1.30
- Ajout des macros PRINT_COMMANDERS, COMMANDERS_PRINT_EVENT et PRINT_EVENT_DCC.
- Ajout d'une EventStack pour pouvoir émettre plusieurs events pendant un seul loop.
- Ajout d'un événement COMMANDER_MOVE_OFF au basculement d'un switch.
- Initialisation d'un switch par la lecture de son état pendant le setup.
- Test sur la présence d'au moins un état pour un switch.
- Retrait d'un pinMode inutile pour les switchs.
- Retrait d'une génération d'événement inutile pour un encodeur.
- Ajout d'une fonction SetMiniMaxi pour les potentiomètres.
- Test de la présence de la bibliothèque mcp_can si le CAN commander est activé.
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
- Mise à jour de DIO2
_______________
- Included library DIO2 updated

\par 21/11/2016 1.22
- Correction de compilation avec printCommanders si pas de COMMANDERS_PRINT_COMMANDERS
_______________
- Fix compilation if no COMMANDERS_PRINT_COMMANDERS defined.

\par 19/11/2016 1.21
- Ajout de printCommanders() pour avoir le détail de ce qui est déclaré. Activable avec #define COMMANDERS_PRINT_COMMANDERS
_______________
- printCommanders() added to show on console all the declared commanders. Must be activated with #define COMMANDERS_PRINT_COMMANDERS

\par 04/11/2016 1.20
- Correction de DccCommander qui ne marchait plus...
_______________
- Fix of DccCommander...

\par 31/10/2016 1.11
- Changement de nom pour éviter des collisions de nom avec d'autres biblios : DEBUG_VERBOSE->COMMANDERS_DEBUG_VERBOSE.
- Ajout des bibliothèques DIO2 et mcp-can dans 'extra'.
- event.h ne sera plus inclus dans un croquis utilisant Commanders.
_______________
- Name changed from DEBUG_VERBOSE to COMMANDERS_DEBUG_VERBOSE, to avoid confusion with other libraries.
- DIO2 and mcp_can libraries included in 'extra'.
- event.h will not be included anymore when Commanders will be used in a sketch.

\par 28/09/2016 1.1
- Déclaration de NO_CANCOMMANDER par défaut.
_______________
- NO_CANCOMMANDER is the default

\par 09/09/2016 1.0
- Inversion de la broche et de l'identifiant dans AddEvent du switch pour être homogène avec le poussoir.
Idem pour l'encodeur et le potentiometre.
- Création du source 'Events.h' pour ne pas avoir à recopier les enums dans les exemples.
_______________
- Inverted pin and id in potentiometer, switch and encoder constructors
- Created 'Events.h' to avoid copying enums in samples.

\par 04/09/2016 0.99
- Plus d'initialisation de Serial, pour le DUE, même dans SerialCommander .
Serial.begin() doit être fait dans le .ino .
- Ajout de l'événement COMMANDERS_EVENT_MOVEPOSITIONID.
- un begin() remplace SetEventHandler et SetStatusLedPin pour Commanders.
- sur une (fausse) intuition, retrait de tous les 'byte' remplacés par des uint8_t .
_______________
- No more internal begin() of Serial class, to avoid problems with the DUE, even in SerialCommander.
Serial.begin must be done, if necessary, in .ino file .
- Added event COMMANDERS_EVENT_MOVEPOSITIONID.
- A begin() function replace the old SetEventHandler and SetStatusLedPin for Commanders.
- Based on a false deduction, replacement of all 'byte' by 'uint8_t'. It is not wrong, but it
could have been avoided...
_______________
\par 21/08/2016 0.98
- Ajout de l'événement COMMANDERS_EVENT_MOVEPOSITIONINDEX.
- Ajout de la syntaxe id:acc pour le SerialCommander
_______________
- Added event COMMANDERS_EVENT_MOVEPOSITIONINDEX
- Added new syntax for Dcc Id in SerialCommander

\par 16/08/2016 0.97
- Correction de Chain.hpp avec des pointeurs
- Adaptation de ButtonsCommanderKeyboard par rapport à ButtonsCommanderPush
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
- Déplacement de RaiseEvent de Commander à Commanders.
- Correction de Chain.hpp
_______________
- RaiseEvent() moved from Commander to Commanders.
- chain.hpp fixed

\par 25/05/2016 V0.94
- Ajout de STRAIGHT et DIVERGE dans les événements
_______________
- Added events STRAIGHT and DIVERGE.

\par 22/05/2016 V0.93
- Modification des événements pour un MOVE multiple.
- Optimisation mémoire et code.
- #define NO_BUTTONSCOMMANDER qui marche...
_______________
- Events modified to add a MOVE with arguments.
- Code and central memory optimized.
- #define NO_BUTTONSCOMMANDER works now.

\par 19/05/2016 V0.92
- Retrait des macros
- Simplification de l'écriture pour les utilisateurs par l'instanciation automatique des commanders et la suppression des
arguments des constructeurs renvoyés vers les begin(). Plus de pointeurs à manipuler !
- Ajout de test.bat pour compiler automatiquement les exemples et obtenir un rapport.
_______________
- Macros removed.
- Simplification of code writing for library users by automatic instanciation of commanders,
and removing of all constructor arguments moved to the begin() functions. No more pointers in the .ino file !
- Added test.bat to check syntax of all examples and reading a final text report.

\par 17/05/2016 V0.91
- Correction de DccCommander qui ne marchait pas du tout !
- Exemple DccMacro adapté.
_______________
- Fix of DccCommander.

\par 15/05/2016 V0.90
- Ajout de l'événement CONFIG avec un octet pour l'adresse, et un autre pour la valeur.
_______________
- Added CONFIG event, with a byte for the address, and another one for the value.

\par 03/05/2016 V0.82
- Correction de l'exemple CANCommander.
- Mise à jour de DIO2 dans extra.
- Renommage de AddId en AddEvent dans les switchs.
- Ajout des masques et des filtres dans CANCommander.
_______________
- Fixes of the sample CANCommander.
- Update of DIO2 library in extra.
- Rename of AddId by AddEvent in switches
- Added masks and filters in CANCommander.

\par 03/05/2016 V0.81
- I2C fonctionne !
- Deux exemples simplissimes de récepteurs CAN et I2C ajoutés.
_______________
- I2C is working !
- Two new samples with basics CAN and I2C receptors.

\par 02/05/2016 V0.80
- Utilisation de listes chainées (avec le template Chain.hpp) pour éviter les dimensionnements inutiles.
- Ajout du type d'événement et du data associé dans les push et les switches.
- Ajout des événements MOVELEFT, MOVERIGHT et MOVESTOP . SELECTED devient TOGGLE.
- Utilisation de la bibliothèque externe DIO2 au lieu de la version interne Arduino2.
- Pour éviter des conflits, DEBUG_MODE est renommé en COMMANDERS_DEBUG_MODE.
- NULL est utilisé aussi souvent que possible.
- CANCommander fonctionne ! Mais seulement en réception. Un exemple d'émeteur a été ajouté.
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
- Exclusion de CANCommander quand NO_CANCOMMANDER défini.
_______________
- TextInterpreter added.
- Use of TextInterpreter inside SerialCommander.
- Exclusion of CANCommander when asked with define NO_CANCOMMANDER.

\par 05/04/2016 V0.64
- Ajout d'une Led de statut commune à tous les Commanders.
- Renommage de tous les 'Setup' en 'begin' et 'Loop' en 'loop'.
_______________
- Added a global status Led for all the commanders.
- All 'Setup' have been renamed in 'begin' and 'Loop' in 'loop'.

\par 02/04/2016 V0.63
- Correction du constructeur de ButtonsCommanderPush avec le nombre d'ids passés maintenant en argument du setup.
- Ajout d'une macro PUSH_BUTTON_IDS pour les ID multiples d'un poussoir.
- Ajout d'une macro ANALOGPUSH_BUTTON_TOL pour les boutons poussoir en Charlieplexing avec une tolérance.
- Correction d'un problème de compilation de ButtonsCommanderButton
- Correction de SerialCommander dans l'interprétation du texte...
- ButtonsCommanderEncoder peut maintenant envoyer un événement absolu avec une valeur entre mini et maxi,
ou un événement relatif avec +1 ou -1.
_______________
- Fix ButtonsCommanderPush constructor, number of multiple ids now passed to the setup function.
- Added PUSH_BUTTON_IDS macro for push button with multiple ids.
- Added ANALOGPUSH_BUTTON_TOL for charlieplexing pushes with tolerancy.
- Fix commpilation problem with	ButtonsCommanderButton
- Fix SerialCommander in text parsing.
- ButtonsCommanderEncoder can now raise an Absolute event, with an associated value between mini and maxi,
or a Relative event, with +1 or -1 as value.

\par 29/03/2016 V0.62
- Changement de syntaxe des macros générales avec COMMANDERS au lieu de COMMANDER
- Suppression de point-virgules à la fin des macros.
- DCCACCESSORY devient DCCACTIVATION
- Correction de la macro ANALOGPUSH_BUTTON .
_______________
- Name change of global macros from COMMANDER to COMMANDERS
- Semi-colon removed at the end of macros.
- DCCACCESSORY changed to DCCACTIVATION
- Fix ANALOGPUSH_BUTTON macro.

\par 25/03/2016 V0.61
- Correction de problèmes de consommation mémoire.
_______________
- Fix memory Consumption in events.

\par 19/03/2016 V0.60
- Correction et améliorations du SerialCommander. Par macros, et compatible avec tous les types de serial class...
- Compatibilité ajoutée avec l'Arduino Due, mais sans bénéficier de l'accélération promise par l'accès direct aux ports.
_______________
- Fix of SerialCommander, to be able to use any kind of serial class, from the hardware serials of Arduinos, to
software emulations by AltSoftSerial or SoftwareSerial .
- Added Arduino Due compatibility, but by using the classic IDE functions to access the pins, not the fast direct
addressing of processor ports.

\par 18/03/2016 V0.50
- Nouveau nom 'Commanders' pour la bibliothèque. Les classes et les defines ont suivi le mouvement.
- Un moyen simple de programmer à l'aide de macros C a été ajouté.
_______________
- New name 'Commanders' for the library. All classes and defines have been adapted when necessary.
- An easy programming way have been added based on C macros.

\par 20/02/2016 V0.40
- Correction des Ids multiples de PushButton.
- Ajout de la classe ButtonsCommanderAnalogPushes
- ButtonsCommander::Loop renvoie une structure avec l'id du bouton sélectionné ou UNDEFINED_ID. LastSelectedButton garde en mémoire le dernier,
même pour un bouton composite comme ButtonsCommanderAnalogPushes. Dans ce cas c'est l'item qui est conservé.
- Un exemple SimpleButtonsByIds a été ajouté pour tester cette façon de fonctionner.
_______________
- Fix multiple ids for push button.
- New class ButtonsCommanderAnalogPushes added.
- Now the ButtonsCommander::Loop function returns the selected id or UNDEFINED_ID. LastSelectedButton is the ...
Last selected button ! Works also with composite buttons like ButtonsCommanderAnalogPushes. In this case, this is the item
which is kept.
- A sample SimpleButtonsByIds has been added to test this mode.

\par 22/12/2015 V0.30
- EventHandler implémenté et fonctionnel
- Simplification de la construction des commanders avec des listes chainées
- Ajout du traitement clavier pour les tests PC-Windows.
_______________
- EventHandler is implemented and operational.
- Commander building in .ino file is more simpler with transparent linked lists.
- Keyboard handling added for PC-Windows tests.

\par 20/12/2015 V0.20
- Gros gain de mémoire dynamique entre DccCommander avec sa liste de CVs trop longue, et surtout SerialCommander qui
utilisait les Serial* du noyau, chacun allouant 150 octets de buffer interne en dur !
_______________
- Big fix on dynamic memory size, because of CV list in DccCommander, but mostly because of SerialCommander which
reference the Serial* default classes from the kernel, each one allocating 150 bytes of internal buffer !

\par 01/09/2015 V0.10
- Création de la librairie, extraite de ma librarie UniversalAccessoryDecoder.
_______________
- Creation, extracted from my UniversalAccessoryDecoder library v4.
*/

/** @file Commanders.h
Main include file of the library.*/

#define COMMANDERS_LIBRARY_VERSION		"Commanders V2.01"

///////////////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to be in debug mode.
//#define COMMANDERS_DEBUG_MODE

//////////////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to be in verbose debug mode.
//#define COMMANDERS_DEBUG_VERBOSE_MODE

//////////////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to activate the print function.
//#define COMMANDERS_PRINT_COMMANDERS

/** This is used internally by DIO2.h */
#define  GPIO2_PREFER_SPEED    1

#include <DIO2.h>

//////////////////////////////////////////
//#define NO_BUTTONSCOMMANDER
//#define NO_BUTTONSCOMMANDERENCODER
//#define NO_BUTTONSCOMMANDERPUSH
//#define NO_BUTTONSCOMMANDERANALOGPUSHES
//#define NO_BUTTONSCOMMANDERSWITCH
//#define NO_BUTTONSCOMMANDERPOTENTIOMETER
#define NO_CANCOMMANDER
#define NO_DCCCOMMANDER
#define NO_DCCCOMMANDERNMRA
#define NO_I2CCOMMANDER
#define NO_SERIALCOMMANDER
#define NO_EVENTSSEQUENCER

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
			/** If this is defined, the DCC railroad modeling protocol command DCC_Decoder part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_DCCCOMMANDER
			/** If this is defined, the DCC railroad modeling protocol command from NMRA part of the library is removed from the compilation.
			It can result in a smaller memory footprint for the final program.*/
			#define NO_DCCCOMMANDERNMRA
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
			#undef NO_DCCCOMMANDERNMRA
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
#ifndef ARDUINO_ARCH_AVR
#pragma message ("Commanders : DCC commander is ONLY compatible with AVR Arduino, use DCC NMRA for others!")
#endif
#include "DccCommander.hpp"
#endif

#ifndef NO_DCCCOMMANDERNMRA
#ifdef __DCC_DECODER_H__
#pragma message ("Commanders : two DCC commanders CANNOT be used in the same sketch, choose between DCC and DCC NMRA!")
#endif
#include "DccCommanderNMRA.hpp"
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