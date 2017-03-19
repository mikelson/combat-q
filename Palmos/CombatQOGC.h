/********************************************************************************

  FILE: CombatQOGC.h
  AUTHOR: Peter Mikelsons, plm@snow.org
  VERSION: 1.3
  PURPOSE: Maintain list of items and initiative order in d20 combat. Header file 
  used by all modules in application that use Open Game Content members.
  CHANGE HISTORY:
  v1.0 - March 24, 2003 - file created.
  v1.1 - May 25, 2003 - Change Performer::removeFromInitList() from protected to
  public.
  v1.2 - July 23, 2003 - Made compatible with Revised (v3.5) SRD.
  Updated OGL.
  Replaced Character::hasPartialAction with Character::hasStandardAction.
  Deleted Character::getDelayInitMin, as there is no longer a minimum initiative 
  count.
  v1.3 - December 2, 2003 - Add mutator and accessor functions for Player 
  Character flag.

  A d20 System Licensed Product

  Requires the use of the Dungeons & Dragons(R) Player's Handbook, 
  Third Edition, published by Wizards of the Coast, Inc.

  The contents of the help windows and the files "CombatQOGC.cpp", 
  CombatQOGC.h", and "CombatQHelp.txt" (distributed with this 
  application) are Open Game Content under the Open Game License (see 
  "Help" menu), and may only be used under and in terms of that License. 
  All other parts of the Combat-Q application are considered Product 
  Identity under the Open Game License, and are copyright (c) 2003, 
  Peter Mikelsons; all rights reserved.

  'd20 System' and the 'd20 System' logo are trademarks of Wizards of 
  the Coast, Inc., a subsidiary of Hasbro, Inc., and are used 
  according to the terms of the d20 System License version 6.0.  A 
  copy of this License can be found at www.wizards.com/d20.

  Dungeons & Dragons(R) and Wizards of the Coast(R) are registered 
  trademarks of Wizards of the Coast, Inc., a subsidiary of Hasbro, 
  Inc., and are used with permission.

  OPEN GAME LICENSE Version 1.0a

The following text is the property of Wizards of the Coast, Inc. and
is Copyright 2000 Wizards of the Coast, Inc ("Wizards"). All Rights
Reserved.

1. Definitions: (a)"Contributors" means the copyright and/or trademark
   owners who have contributed Open Game Content; (b)"Derivative
   Material" means copyrighted material including derivative works and
   translations (including into other computer languages), potation,
   modification, correction, addition, extension, upgrade,
   improvement, compilation, abridgment or other form in which an
   existing work may be recast, transformed or adapted; (c)
   "Distribute" means to reproduce, license, rent, lease, sell,
   broadcast, publicly display, transmit or otherwise distribute;
   (d)"Open Game Content" means the game mechanic and includes the
   methods, procedures, processes and routines to the extent such
   content does not embody the Product Identity and is an enhancement
   over the prior art and any additional content clearly identified as
   Open Game Content by the Contributor, and means any work covered by
   this License, including translations and derivative works under
   copyright law, but specifically excludes Product Identity. (e)
   "Product Identity" means product and product line names, logos and
   identifying marks including trade dress; artifacts; creatures
   characters; stories, storylines, plots, thematic elements,
   dialogue, incidents, language, artwork, symbols, designs,
   depictions, likenesses, formats, poses, concepts, themes and
   graphic, photographic and other visual or audio representations;
   names and descriptions of characters, spells, enchantments,
   personalities, teams, personas, likenesses and special abilities;
   places, locations, environments, creatures, equipment, magical or
   supernatural abilities or effects, logos, symbols, or graphic
   designs; and any other trademark or registered trademark clearly
   identified as Product identity by the owner of the Product
   Identity, and which specifically excludes the Open Game Content;
   (f) "Trademark" means the logos, names, mark, sign, motto, designs
   that are used by a Contributor to identify itself or its products
   or the associated products contributed to the Open Game License by
   the Contributor (g) "Use", "Used" or "Using" means to use,
   Distribute, copy, edit, format, modify, translate and otherwise
   create Derivative Material of Open Game Content. (h) "You" or
   "Your" means the licensee in terms of this agreement.

2. The License: This License applies to any Open Game Content that
   contains a notice indicating that the Open Game Content may only be
   Used under and in terms of this License. You must affix such a
   notice to any Open Game Content that you Use. No terms may be added
   to or subtracted from this License except as described by the
   License itself. No other terms or conditions may be applied to any
   Open Game Content distributed using this License.

3. Offer and Acceptance: By Using the Open Game Content You indicate
   Your acceptance of the terms of this License.

4. Grant and Consideration: In consideration for agreeing to use this
   License, the Contributors grant You a perpetual, worldwide,
   royalty-free, non-exclusive license with the exact terms of this
   License to Use, the Open Game Content.

5. Representation of Authority to Contribute: If You are contributing
   original material as Open Game Content, You represent that Your
   Contributions are Your original creation and/or You have sufficient
   rights to grant the rights conveyed by this License.

6. Notice of License Copyright: You must update the COPYRIGHT NOTICE
   portion of this License to include the exact text of the COPYRIGHT
   NOTICE of any Open Game Content You are copying, modifying or
   distributing, and You must add the title, the copyright date, and
   the copyright holder's name to the COPYRIGHT NOTICE of any original
   Open Game Content you Distribute.

7. Use of Product Identity: You agree not to Use any Product Identity,
   including as an indication as to compatibility, except as expressly
   licensed in another, independent Agreement with the owner of each
   element of that Product Identity. You agree not to indicate
   compatibility or co-adaptability with any Trademark or Registered
   Trademark in conjunction with a work containing Open Game Content
   except as expressly licensed in another, independent Agreement with
   the owner of such Trademark or Registered Trademark. The use of any
   Product Identity in Open Game Content does not constitute a
   challenge to the ownership of that Product Identity. The owner of
   any Product Identity used in Open Game Content shall retain all
   rights, title and interest in and to that Product Identity.

8. Identification: If you distribute Open Game Content You must
   clearly indicate which portions of the work that you are
   distributing are Open Game Content.

9. Updating the License: Wizards or its designated Agents may publish
   updated versions of this License. You may use any authorized
   version of this License to copy, modify and distribute any Open
   Game Content originally distributed under any version of this
   License.

10. Copy of this License: You MUST include a copy of this License with
    every copy of the Open Game Content You Distribute.

11. Use of Contributor Credits: You may not market or advertise the
    Open Game Content using the name of any Contributor unless You
    have written permission from the Contributor to do so.

12. Inability to Comply: If it is impossible for You to comply with
    any of the terms of this License with respect to some or all of
    the Open Game Content due to statute, judicial order, or
    governmental regulation then You may not Use any Open Game
    Material so affected.

13. Termination: This License will terminate automatically if You fail
    to comply with all terms herein and fail to cure such breach
    within 30 days of becoming aware of the breach. All sublicenses
    shall survive the termination of this License.

14. Reformation: If any provision of this License is held to be
    unenforceable, such provision shall be reformed only to the extent
    necessary to make it enforceable.

15. COPYRIGHT NOTICE
Open Game License v 1.0a Copyright 2000, Wizards of the Coast, Inc.

System Reference Document Copyright 2000-2003, Wizards of the Coast, Inc.; Authors Jonathan Tweet, Monte Cook, Skip Williams, Rich baker, Andy Collins, David noonan, Rich Redman, Bruce R. Cordell, based on original material by E. Gary Gygax and Dave Arneson. 

Combat-Q v1.3 Copyright 2004, Peter Mikelsons.

END OF OPEN GAME LICENSE

*********************************************************************************/
#ifndef __OGC_H__
#define __OGC_H__

#include <PalmOS.h>

// The different types of performers.
typedef enum PerformerTypeEnum {
	CHARACTER,
	EFFECT
} PerformerType;

/* Base class for the three classes in the initiative list. */
class Performer {
public:
	/* Object life-cycle functions */
	PERF_SECTION Performer(void) {};
	// Initializer for derived class constructors - creates database record
	Performer(UInt16 dbVersion, UInt16 index);// PERF_SECTION;
	// Deletes database record
	virtual ~Performer(void) PERF_SECTION;
	// Return pointer to new performer with same values as this one, with argument in name
	PERF_SECTION virtual Performer* copy(Int16 labelI) = 0;
	
	/* Object identification functions */
	// Return unique, execution-independent identification code for performer
	UInt32 getID(void) PERF_SECTION;
	// Return current database index
	UInt16 getIEncounter(void) PERF_SECTION;
	// Return type code of performer
	PerformerType getType(void) PERF_SECTION;

	/* Object name-handling functions */
	// Assign current size (in bytes) of performer name string. Used after saving table field.
	void setNameSize(UInt16 size) PERF_SECTION;
	// Get locked handle to performer name string - used in PlayForm.cpp
	MemHandle getNameHandle(Int16* dataOffset, Int16* dataSize) PERF_SECTION;
	// Get locked handle and pointer to performer name string
	MemHandle getNameHandle(Char** string) PERF_SECTION;
	// Copy the performer name string to dest. Resizes dest.
	void nameCopy(Char** dest) PERF_SECTION;
	// Sets the performer name string to have same value as argument
	void setName(Char* newName);// PERF_SECTION;

	/* Other, member-data functions */
	// Return current initiative
	Int16 getInitiative(void) PERF_SECTION;
	// Assign current initiative without making other changes
	void setInitiative(Int16 initiative) PERF_SECTION;
	// Return true if a Player Character
	PERF_SECTION virtual Boolean isPC(void) = 0;

	/* Initiative List handling functions */
	// Insert performer into initiative list at appropriate location
	PERF_SECTION virtual void addToInitList(void) = 0;
	// Remove performer from initiative list
	Boolean removeFromInitList(void) PERF_SECTION;
	// Return previous performer in initiative list
	Performer* getPrev(void) PERF_SECTION;
	// Return next performer in initiative list
	Performer* getNext(void) PERF_SECTION;
	// Add newbie to initiative list before this performer	
	void insertBefore(Performer* newbie) PERF_SECTION;
	// Add newbie to initiative list after this performer
	void insertAfter(Performer* newbie) PERF_SECTION;
	// Return whether performer is in initiative list
	Boolean isActive(void) PERF_SECTION;
	// Set flag indicating performer is in initiative list, without making other changes
	void setActive(Boolean active) PERF_SECTION;
	// Add performer to initiative list
	virtual void activate(void) PERF_SECTION;
	// Remove performer from initiative list, or return false.
	virtual Boolean deactivate(void) PERF_SECTION;
	// Prepare performer to take a turn
	PERF_SECTION virtual void prepareToAct(void) = 0;
	// Have performer pass the current pointer to the next performer
	PERF_SECTION virtual void act(void) = 0;
	// Remove performer from initiative list
	void die(void) PERF_SECTION;
	
	/* UI functions */
	// Open a form for editing performer
	PERF_SECTION virtual void popupEditForm(void) = 0;
	// Set up the playing form for a character
	PERF_SECTION virtual void setUpPlayForm(FormType* form) = 0;
	// Possibly, open a form for delaying performer
	PERF_SECTION virtual void popupDelayForm(void) = 0;

protected:
    // pointer- and database-independent, unique identification code
	UInt32 id;

	// Create a new database record for performer
	Err OpenRecord(Char* name, MemHandle* h, MemPtr* p, UInt16* iDb) PERF_SECTION;
	// Write data to this performer's database record
	void set(UInt32 offset, const void* srcP, UInt32 bytes) PERF_SECTION;
	// Set/reset a bit in this performer's database record
	void set(UInt32 offset, UInt8 bit, Boolean value) PERF_SECTION;
	// Get pointer to performer's record
	MemPtr get(void) PERF_SECTION;
	// Get data from performer's database record
	void get(Int32 offset, void* dstP, Int32 numBytes) PERF_SECTION;

	// Construct a new string using performer name string and argument
	Char* getNamePlusNumber(Int16 labelI) PERF_SECTION;
	// Return bytes used by performer name string (including null terminator)
	UInt16 Performer::getNameSize(void) PERF_SECTION;

private:
	// Return offset (in bytes) of name string in performer database record
	PERF_SECTION virtual UInt16 getNameOffset(void) = 0;
	// Assign next performer in initiative list
	void setNext(Performer* next) PERF_SECTION;
	// Assign previous performer in initiative list
	void setPrev(Performer* prev) PERF_SECTION;
};

/* 
 * Class that represents items in initiative list that can take actions,
 * delay, ready, etc.
 */
class Character : public Performer {
public:
	// Member functions derived from base class
	Character(UInt16 dbVersion, UInt16 iDb);// PERF_SECTION;
	void addToInitList() PERF_SECTION;
	Performer* copy(Int16 labelI) PERF_SECTION;
	void popupEditForm(void) PERF_SECTION;
	void prepareToAct(void) PERF_SECTION;
	void act(void) PERF_SECTION;
	void setUpPlayForm(FormType* form) PERF_SECTION;
	void popupDelayForm(void) PERF_SECTION;
	// Remove character from database - including effect's owner fields
	~Character(void) PERF_SECTION;

	// Create new character with given arguments
	Character(Char* name, Int16 mod, UInt8 counter, UInt8 duration) PERF_SECTION;

	// Return initiative modifier
	Int16 getModifier(void) PERF_SECTION;
	// Set initiative modifier
	void setModifier(Int16 bonus) PERF_SECTION;
	// Set initiative value to roll plus modifier
	void setInitiativeRoll(Int16 roll) PERF_SECTION;

	// Add character to initiative list
	void activate(void) PERF_SECTION;
	// Remove character from initiative list
	Boolean deactivate(void) PERF_SECTION;
	// Return true if the character only gets a Standard or Move action this round
	Boolean hasStandardAction(void) PERF_SECTION;
	// Return true if delaying
	Boolean isDelaying(void) PERF_SECTION;
	// Set delaying flag
	void setDelaying(Boolean value) PERF_SECTION;
	// Return the highest initiative the character may delay to
	Int16 getDelayInitMax(void) PERF_SECTION;
	// Return true if readied
	Boolean isReadied(void) PERF_SECTION;
	// Set readied flag
	void setReadied(Boolean readied) PERF_SECTION;
	// Return true if taking readied action
	Boolean isTakingReadied(void) PERF_SECTION;
	// Set taking-readied-action flag
	void setTakingReadied(Boolean value) PERF_SECTION;
	// Return true if aware of opponents
	Boolean isAware(void) PERF_SECTION;
	// Set aware-of-opponents flag
	void setAware(Boolean aware) PERF_SECTION;
	// Return true if a Player Character
	Boolean isPC(void) PERF_SECTION;
	// Set player-character flag
	void setPC(Boolean pc) PERF_SECTION;
	// Set label applied to default spell name
	void setCounter(UInt8 counter) PERF_SECTION;
	// Return default spell name label
	UInt8 getCounter(void) PERF_SECTION;
	// Increase default spell name label by one
	UInt8 incrementCounter(void) PERF_SECTION;
	// Return default spell duration
	UInt8 getDuration(void) PERF_SECTION;
	// Set default spell duration
	void setDuration(UInt8 duration) PERF_SECTION;

	// Put character into delaying state until specified count
	void delay(Int16 initNew) PERF_SECTION;
	// Put character into delaying state with unspecified count
	void delay(void) PERF_SECTION;
	// Make character take delayed action after given performer
	void delayTrigger(Performer* p) PERF_SECTION;
	// Put character into readying state
	void ready(void) PERF_SECTION;
	// Make character take readied action before given performer	
	void interrupt(Performer* p) PERF_SECTION;
	// Make character take refocus action - this was an action in old, pre-3.5 SRD
	void refocus(void) PERF_SECTION;
		
protected:
private:
	// Member functions derived from base class
	UInt16 getNameOffset(void) PERF_SECTION;
};

/* 
 * Class that represents items in initiative list that do not take actions
 * and that last for a limited duration.
 */
class Effect : public Performer {
public:
	// Member functions derived from base class
	Effect(UInt16 dbVersion, UInt16 iDb);// PERF_SECTION;
	// Remove character from database - including summoned creature's owner fields
	~Effect(void) PERF_SECTION;
	void addToInitList() PERF_SECTION;
	Performer* copy(Int16 labelI) PERF_SECTION;
	void popupEditForm(void) PERF_SECTION;
	void prepareToAct(void) PERF_SECTION;
	void act(void) PERF_SECTION;
	void setUpPlayForm(FormType* form) PERF_SECTION;
	PERF_SECTION void popupDelayForm(void) {};

	// Construct new effect with given arguments
	Effect(Char* name, UInt8 nominal, UInt8 remaining, Character* owner) PERF_SECTION;
	
	// Add effect to initiative list
	void activate(void) PERF_SECTION;
	// Remove effect from initiative list
	Boolean deactivate(void) PERF_SECTION;
	// Return usual duration, in rounds
	UInt8 getNominal(void) PERF_SECTION;
	// Assign usual duration
	void setNominal(UInt8 value) PERF_SECTION;
	// Return current number of rounds remaining
	UInt8 getRemaining(void) PERF_SECTION;
	// Assign number of rounds remaining
	void setRemaining(UInt8 value) PERF_SECTION;
	// Reduce number of rounds remaining by one
	void decrement(void) PERF_SECTION;
	// Return pointer to character that created the effect
	Character* getOwner(void) PERF_SECTION;
	// Assign pointer to character that created the effect
	void setOwner(Character* owner) PERF_SECTION;
	// Return true if a Player Character - always false for Effects
	Boolean isPC(void) PERF_SECTION;
	// Associate something with an effect and place that something in the initiative list
	void summon(Performer* p, Boolean followOwner) PERF_SECTION;
	// Remove something previously associated with an effect and return it
	Performer* removeSummoned(void) PERF_SECTION;
	
	/*
	UInt8 getSummonedCount(void);
	Character* getSummoned(UInt8 index);
	Character* removeSummoned(UInt8 index);
	Character* removeSummoned();
	*/
protected:
private:
	// Add a performer to effect's collection of summoned performers
	void addSummoned(Performer* summoned) PERF_SECTION;
	// Member functions derived from base class
	UInt16 getNameOffset(void) PERF_SECTION;
};

/* Variables used to define the Initiative List (a Double Ring Buffer data structure) */
// Pointer to performer that goes first in initiative list
extern Performer* first;
// Pointer to performer whose turn it is currently
extern Performer* current;
/* The current round number. */
extern Int32 round;

// Start a new combat
extern void EncounterStart(Int32 iLastChar) PERF_SECTION;
// End the current combat
extern Boolean EncounterEnd(void) PERF_SECTION;
// Get pointer to performer from database index
extern Performer* queryPerformerP(UInt16 iEncounter) PERF_SECTION;

#endif
