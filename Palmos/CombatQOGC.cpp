/********************************************************************************

  FILE: OGC.cpp
  AUTHOR: Peter Mikelsons, plm@snow.org
  VERSION: 1.3
  PURPOSE: Maintain list of items and initiative order in d20 combat.
  CHANGE HISTORY:
  v1.0 - March 24, 2003 - file created.
  v1.1 - June 6, 2003 - Corrected defect in EndEncounterForce() which was
  corrupting initiative list. The iteration over the init list was only
  deactivating the first performer because the pointer was never moving
  to the next performer.
  v1.2 - July 23, 2003 - Made compatible with Revised (v3.5) SRD.
  Updated OGL.
  Replaced Character::hasPartialAction with Character::hasStandardAction.
  Deleted Character::getDelayInitMin, as there is no longer a minimum initiative 
  count. Modified Character::delay to simply set Delaying flag and go on to
  next performer.
  Modified Character::addToInitList to use character's initiative modifiers
  to break ties before opening Tied dialog.
  v1.3 - December 2, 2003 - In EndEncounterForce(), (re)activate all player 
  characters when encounter ends.

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
#include "Main.h"
#include "CombatQOGC.h"

/* Combat round.
 * Less than zero - No combat is in progress.
 * Exactly zero - Surprise round of combat in progress.
 * More than zero - Normal combat in progress, value is the current round number
 */
Int32 round = -1;

/* The first performer in the initiative list */
Performer* first = NULL;
/* The current performer in the combat */
Performer* current = NULL;

/***************************************************************************
 * Utility Functions
 ***************************************************************************/

/*
 * Start a combat. Function uses a database of performers, which has been
 * sorted to put active characters before other performers.
 * The argument is the index of the last active character.
 */
void EncounterStart(Int32 iLastChar) {
	/* Check for surprise round */
	UInt16 nAware = 0;
	// iterate over all active characters
	for (int i=0; i < iLastChar; i++) {
		Character* c = (Character*) queryPerformerP(i);
		if ( (c!=NULL) && c->isAware() ) nAware++;
	}
	if ( (nAware==0) || (nAware==iLastChar) ) {
		// no combatants are aware, or all combatants are aware
		// no surprise round
		round = 1;
	} else {
		// surprise round
		round = 0;
	}
	// Insert all active performers into initiative list
	UInt16 num = DmNumRecordsInCategory(dbEncounter,dmAllCategories);
	for (UInt16 i=0; i < num; i++) {
		Performer* p = queryPerformerP(i);
		if ( (p!=NULL) && p->isActive() ) {
		 	p->addToInitList();
		}
	}
	// Establish current performer
	current = first;
	// Initialize encounter
	current->prepareToAct();
}    

/* End encounter. */
static void EncounterEndForce(void) {
	// save a vestige of initiative list
	Performer* p = first;
	// destroy init list - must do before getting to removeFromInitList()
	first = NULL;
	current = NULL;
	round = -1;
	// deactivate all performers remaining in list
	Performer* next = p;
	do {
		p = next;
		next = p->getNext();
		p->deactivate();
	} while (p != next);
	
	// Activate all player characters in database
	UInt16 num = DmNumRecordsInCategory(dbEncounter,dmAllCategories);
	for (UInt16 i=0; i < num; i++) {
		Performer* p = queryPerformerP(i);
		if ( (p!=NULL) && p->isPC() ) {
		 	p->activate();
		}
	}
	
	FrmGotoForm(OverviewFormID);
}

/* End encounter, but with a warning to user.
   Returns true if user accepted warning and combat ended. */
Boolean EncounterEnd(void) {
	// verify that combat is in progress
	if (round < 0) return true;
	// display alert to verify ending of encounter
	UInt16 result = FrmAlert(EncounterEndAlertID);
	if ( result != 0 ) {// first button ("End") not pressed
		return false;
	}
	EncounterEndForce();
	return true;
}

/***************************************************************************
 * Performer Methods
 ***************************************************************************/

/*
 * This method removes a performer from the initiative list
 * Returns true if the performer is no longer in list.
 */
Boolean Performer::removeFromInitList(void){
	Err e;
	Performer* leader = getPrev();
	Performer* follower = getNext();
	if ( (leader==this) && (follower==this) ) { // not in or last performer in list
		if ( (first==NULL) || (current==NULL) ) { // there is no list - no need to do anything
			return true;
		} else { // only performer in list - end combat
			return EncounterEnd();
		}			
	}
	ErrFatalDisplayIf( (leader==this) || (follower==this) 
		|| (leader==NULL) || (follower==NULL), "Corrupt initiative list.");
	/* Isolate this performer from the initiative list. */
	// set next to this
	setNext(this);
	// set previous to this 
	setPrev(this);
	/* Close the hole in the initiative list.*/
	// set leader's next to follower
	if (leader != NULL) leader->setNext(follower);
	// set follower's previous to leader
	if (follower != NULL) follower->setPrev(leader);
	// If removing the first performer in init list, make the next one first.
	if (this == first) {
		first = follower;
	}
	// If removing the current performer, make the next one current.
	if (this == current) {
		current = follower;
	}
	return true;
}

/* Prepare to add this performer to the initiative list. */
void Performer::activate(void){
	// set active flag
	setActive(true);

	if ( round >= 0 ) { // combat in progress
		addToInitList();
	}
}

/* Return false if performer is last on list and user declines to end encounter */
Boolean Performer::deactivate(void){
	if (isActive() == true) {// removing inactive performer from list might end encounter
    	if ( removeFromInitList() ) {
			// unset active flag
    		setActive(false);
    		return true;
    	} else { // failed to deactivate
			return false;
		}
	} else { // inactive performer automatically deactivated
		return true;
	}
}

/*
 * Remove this performer from the initiative list and pass control on to next performer.
 */
void Performer::die(void) {
	Performer* nextOld = getNext();
	if ( this == nextOld  || nextOld == NULL ) { // last in init list or corrupt list
		EncounterEnd();
	} else {
		if (nextOld == first) round++;
		deactivate();
		current = nextOld;
		current->prepareToAct();
		FrmGotoForm(PlayFormID);
	}
}
/***************************************************************************
 * Character methods.
 ***************************************************************************/

void Character::activate(void) {
	Performer::activate();
}

Boolean Character::deactivate(void) {
	return Performer::deactivate();
}

void Character::setInitiativeRoll(Int16 roll) {
	setInitiative(roll + getModifier() );
}

void Character::addToInitList(void){
	if (first==NULL) { // list is empty
		// make this the first performer
		first = this;
		return;
	}
	/* Start at begining of initiative list. Look for first performer
	   with lower initiative, and insert this character before it. */
	Performer* that = first;
	do {
		Int16 initThis = getInitiative();
        Int16 initThat = that->getInitiative();
        if (initThis == initThat) {
        /* Ignore ties with effects - characters always go after effects */
        	if (that->getType()==CHARACTER) {
				/* Compare initiative modifiers. */
				Int16 modThis = getModifier();
				Int16 modThat = ((Character*)that)->getModifier();
				if (modThis == modThat) {
    		        // Display Resolve Tie Confirmation
       				Char* nameThis;
       				Char* nameThat;
    	        	MemHandle hThis = getNameHandle(&nameThis);
        	    	MemHandle hThat = that->getNameHandle(&nameThat);
    			    UInt32 romVersion;
    			    FtrGet(sysFtrCreator,sysFtrNumROMVersion, &romVersion);
    				UInt16 result = (romVersion > sysMakeROMVersion(2, 0, 0, 0, 0))?
    					FrmCustomAlert(TieAlertID,nameThat,nameThis,"") :
    					FrmCustomAlert(TieAlertID,nameThat,nameThis," ");
            		MemHandleUnlock(hThis);
            		MemHandleUnlock(hThat);
            		if (result == 1) { // this character is faster than that
            			if (that==first) first = this;
    					// insert this character before the slower Performer
            			that->insertBefore(this); 
            			return;
       				}
				} else if (modThis > modThat) { // that performer is slower
					if (that==first) first = this;
		        	that->insertBefore(this);
	    		    return;
				} // end of modifier comparison condition
			} // end of CHARACTER conditional
        } else if (initThis > initThat) { // that performer is slower
			if (that == first) first = this;
        	that->insertBefore(this);
	        return;
        } // end of initiative comparison conditional
		that = that->getNext();
	} while (that != first);
	// Make this character last
	first->insertBefore(this);
}

Boolean Character::hasStandardAction(void) {
	// All actions are standard or move actions during surprise round
	if (round==0) return true;
	// If character is taking a readied action, it must be a standard or move action
	return isTakingReadied();
}

void Character::prepareToAct(void) {
    if ( (round==0) && (isAware() == false ) ) {
	    // character surprised on surprise round!
    	setAware(true); // make it aware
	    act(); // Skip the current character
    }
    // if character was delaying, it no longer is
    setDelaying(false);
    // can't hold readied action any longer either
    setReadied(false);
}

void Character::act(void) {
	setTakingReadied(false);
	current = getNext();
	if (current == first) { // Start of the list - new round
		round++;
	}
	current->prepareToAct();
}

/*
 * Take steps necessary to move character to new, later initiative.
 */
void Character::delay(Int16 initNew) {
	// Verify that this is a valid value
	if (initNew > getDelayInitMax())
		return;
	Performer* nextOld = getNext();
	Performer* firstOld = first;
    setInitiative(initNew);
	setDelaying(true);
	if (this != nextOld) {// not only performer in initiative list
		// take out of initiative list
		deactivate();
		// put character back into initiative list
		activate();
	}
	// go to old next character (if order changed)
	Performer* nextNew = getNext();
	if ( (nextOld==nextNew) && (firstOld==first) ) { // no change to init list
		current = this; // let this performer go again with new initiative
	} else { // order changed
		current = nextOld; // continue with next performer
	}
	current->prepareToAct();
}

/* Take steps to move character to unspecified, later initiative. */
void Character::delay() {
	setDelaying(true);
	// go on to next character
	act();
}

/*
 * Take delayed action after the given performer.
 */
void Character::delayTrigger(Performer* p) {
	// sanity check...
	ErrNonFatalDisplayIf(isDelaying()==false,
		"Triggering delayed action for non-delaying character!");
	/* Warn user about potentially abusive action: Suppose this character
	 * is taking a readied action to interrupt character x. If character
	 * p was delaying and decides to follow this character, character p will
	 * go before character x. Delaying should not allow characters to effectively
	 * interrupt other characters. */
	if (p->getType()==CHARACTER) {
		if (((Character*)current)->isTakingReadied()) {
			if (FrmAlert(ReadiedTriggeringAlertID)!=0) {
				return;
			}
		}
	}
	// prepare for moving to new list location
	/* Assume that this is not the only performer in initiative list,
	   for if it was it could not have been selected by trigger form. */
    removeFromInitList();
	// reset initiative value
    setInitiative(p->getInitiative());
	// move to new list location
    p->insertAfter(this);
	// clear delaying status
    setDelaying(false);
}

Int16 Character::getDelayInitMax(void) {
	// maximum delaying initiative is current initiative minus one
	return getInitiative() - 1;
}

/*
 * Have character take Ready action: go into ready state and continue with next character
 */ 
void Character::ready(void) {
	setReadied(true);
	act();
}

/*
 * Take readied action before the given performer.
 */
void Character::interrupt(Performer* p) {
	// sanity check
	ErrNonFatalDisplayIf(isReadied()==false,"Interrupting with non-readied character!");
	// prepare for moving to new list location
	/* Assume that this is not the only performer in initiative list,
	   for if it was it could not have been selected by trigger form. */
	removeFromInitList();
	// reset initiative value
	setInitiative(p->getInitiative());
	// move to new list location
	p->insertBefore(this);
	// clear readied status
	setReadied(false);
	// indicate that this is a readied action
	setTakingReadied(true);
	// move interrupter to start of list, if needed
	if (current == first) {
		first = this;
	}
	// make interrupter current
	current = this;
}

/*
 * Change character's initiative to 20+bonus. This action is not present
 * in the Revised (v3.5) SRD, but it retained for players who want to
 * use it as a house-rule.
 */
void Character::refocus(void) {
	if (isTakingReadied()) {
		// can't Refocus on Readied action... would allow a move+Refocus
		return;
	}
	Performer* oldNext = getNext();
	// Is this the last performer in the initiative list?
	Boolean wasLast = (oldNext==first);
	// Is this the current performer?
	Boolean wasCurrent = (this==current);
	// reset initiative
	setInitiative( 20 + getModifier() );
	if (this != oldNext) { // This is not the only performer in initiative list
		// remove from list
		deactivate();
		// return to list
		activate();
	}
	/* If for some reason we are refocusing non-current character, 
	   then no more changes needed. */
	if (wasCurrent==false) {
		return;
	} else if ( wasLast && (this==first) ) {
	/* If current performer was last, and is now first, 
	   then it's a new turn and it goes again. */
		round++;
		current = this;
	} else {
	/* Otherwise go on to old next performer. */
		current = oldNext;
	}
	current->prepareToAct();
}

/***************************************************************************
* Effect Methods
 ***************************************************************************/

void Effect::activate(void) {
	// if adding to initiative list before combat, reset the rounds remaining
	if (round < 0) setRemaining( getNominal() );
	Performer::activate();
}

Boolean Effect::deactivate(void) {
	if ( Performer::deactivate() ) {
	
    	// deactivate all summoned performers
    	Performer* p;
		// this forces deactivation, regardless if it would end combat
    	while ( (p=removeSummoned()) != NULL ) {
    		p->deactivate();
    	}
		
		return true;

	} else { // This effect was not deactivated
		return false;
	}
}

/* Reduce the number of rounds remaining by one, to minimum of zero. */
void Effect::decrement(void) {
	UInt8 i = getRemaining();
	if (i > 0) {
		i--;
		setRemaining(i);
	}
}

/* "Effects that last a certain number of rounds end just before the same 
    initiative count that they began on." (SRD) */
void Effect::addToInitList(){
	if (first==NULL) { // list is empty
		// make this the first performer
		first = this;
		return;
	}
	Int16 initThis = getInitiative();
	Performer* p = first;
	do { 
		if (initThis >= p->getInitiative()) { // current performer is slower
			if (p->getType() == CHARACTER) { 
				// effects always go before characters with same initiative
				// this means effects with same init will go in order added to list
				p->insertBefore(this);
				// update start of initiative list, if necessary
				if (p==first) first = this;
				// we're done
				return;
			}
		} // end of initiative comparison conditional
		p = p->getNext();
	} while (p != first);
	// No characters were slower, make this effect last.
	first->insertBefore(this);
}

void Effect::prepareToAct(void) {
	decrement();
}

/* This effect acts, then makes its next performer the current performer. */
void Effect::act(void) {
	Performer* next = getNext();
    if ( getRemaining() < 1) {
        // Effect is done - get rid of it
		if (next == this) { // only performer in initiative list!
			EncounterEndForce();
			return;
		}
        deactivate();
	}
	current = next;
    if (first == current) round++; // last performer in init list 
	current->prepareToAct();
}

/* Associate something with an effect and place that something in the initiative list,
   as appropriate. 

   If followOwner is true, the something will go right after the effect's owner.
   Otherwise, the something will go right after the effect.

   Normally, followOwner will be true, and summoned creatures will act immediately 
   after the character who summoned them. A use case where followOwner is false would
   be when a character creates an effect, the character changes initiative, and then the
   effect summons another creature. The creature would then logically act on the effect's
   initiative, not the character's.*/
void Effect::summon(Performer* p, Boolean followOwner) {
	Character* owner;

	// Add the performer to the effect's collection of summoned things
	addSummoned(p);
	
	if (followOwner && ( (owner=getOwner()) != NULL ) && (owner->isActive()) ) {
		// Set the summoned thing to follow the owner in the initiative list (if any)
		p->setInitiative( owner->getInitiative() );
		
		if (isActive()) { 
			// only put summoned thing in combat if owner and effect are both in combat
			p->setActive( true );
			
			if (round >= 0) // only put summoned thing in combat if there is a combat underway
				owner->insertAfter(p);
		}
			
	} else { // followOwner is false, or there is no owner, or owner is not in combat

		// Set the summoned thing to follow the effect in the initiative list (if any)
		p->setInitiative( getInitiative() );

		if (isActive()) { // only put summoned thing in combat if effect is in combat
			p->setActive(true);

			if (round >= 0) // only put summoned thing in combat if there is a combat underway
				insertAfter(p);
		}
	}
}

