/********************************************************************************

  FILE: CombatQOGC.js
  AUTHOR: Peter Mikelsons, peter@frombits.com
  VERSION: 1.3
  PURPOSE: Maintain list of items and initiative order in d20 combat.
  CHANGE HISTORY:
  v1.0 - April 11, 2017 - file copied from CombatQOGC.cpp.

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

Combat-Q v1.3 Copyright 2017, Peter Mikelsons.

END OF OPEN GAME LICENSE

*********************************************************************************/
import {
  FrmAlert, 
  ResolveTieDialog, 
  ErrFatalDisplayIf, 
  ErrNonFatalDisplayIf
} from "./Forms";
import {
  EncounterEndAlertID, 
  ReadiedTriggeringAlertID
} from "./Strings";
import {
  DmNumRecordsInCategory,
  queryPerformerP, 
  getPrev, 
  setPrev, 
  getNext, 
  setNext, 
  isActive,
  setActive,
  getInitiative,
  setInitiative,
  getNameHandle,
  insertBefore,
  insertAfter,
} from "./PerformerDb";
import {
  getModifier,
  isAware,
  setAware,
  isTakingReadied,
  setTakingReadied,
  isReadied,
  setReadied,
  isDelaying,
  setDelaying,
} from "./CharacterDb";
import {
  getRemaining,
  setRemaining,
  decrement,
  getNominal,
  addSummoned,
  removeSummoned,
  getOwner
} from "./EffectDb";
class CombatQ {

/* Combat round.
 * Less than zero - No combat is in progress.
 * Exactly zero - Surprise round of combat in progress.
 * More than zero - Normal combat in progress, value is the current round number
 */
round: -1;

/* The first performer in the initiative list */
first: undefined;
/* The current performe in the combat */
current: undefined;

/***************************************************************************
 * Utility Functions
 ***************************************************************************/

/*
 * Start a combat. Function uses a database of performers, which has been
 * sorted to put active characters before other performers.
 * The argument is the index of the last active character.
 */
EncounterStart(iLastChar) {
	/* Check for surprise round */
	let nAware = 0;
	// iterate over all active characters
	for (let i=0; i < iLastChar; i++) {
		let c = this.queryPerformerP(i);
		if ( c && c.isAware() ) nAware++;
	}
	if ( (nAware===0) || (nAware===iLastChar) ) {
		// no combatants are aware, or all combatants are aware
		// no surprise round
		this.round = 1;
	} else {
		// surprise round
		this.round = 0;
	}
	// Insert all active performers into initiative list
	let num = DmNumRecordsInCategory();
	for (let i=0; i < num; i++) {
		let p = queryPerformerP(i);
		if ( p && p.isActive() ) {
		 	p.addToInitList();
		}
	}
	// Establish current performer
	this.current = this.first;
	// Initialize encounter
	this.current.prepareToAct();
}    

/* End encounter. */
static EncounterEndForce() {
	// save a vestige of initiative list
	let p = this.first;
	// destroy init list - must do before getting to removeFromInitList()
	this.first = undefined;
	this.current = undefined;
	this.round = -1;
	// deactivate all performers remaining in list
	let next = p;
	do {
		p = next;
		next = p.getNext();
		p.deactivate();
	} while (p !== next);
	
	// Activate all player characters in database
	let num = DmNumRecordsInCategory();
	for (let i=0; i < num; i++) {
		let p = queryPerformerP(i);
		if ( p && p.isPC() ) {
		 	p.activate();
		}
	}	
	// Controller logic should do this: FrmGotoForm(OverviewFormID);
}

/* End encounter, but with a warning to user.
   Returns true if user accepted warning and combat ended. */
EncounterEnd() {
	// verify that combat is in progress
	if (this.round < 0) return true;
	// display alert to verify ending of encounter
	let result = FrmAlert(EncounterEndAlertID);
	if ( result ) {// first button ("End") not pressed
		return false;
	}
	this.EncounterEndForce();
	return true;
}
}

let PerformerTypeEnum = {
  CHARACTER: 1,
  EFFECT: 2
}

class Performer {
constructor(encounter) {
  this.encounter = encounter;
}

getNext() {
  return getNext(this);
}

isActive() {
  return isActive(this);
}

setActive(value) {
  setActive(this, value);
}

getInitiative() {
  return getInitiative(this);
}

setInitiative(value) {
  setInitiative(this, value);
}

getModifier() {
  return getModifier(this);
}

insertBefore(p) {
  insertBefore(this, p);
}

insertAfter(p) {
  insertAfter(this, p);
}
/*
 * This method removes a performer from the initiative list
 * Returns true if the performer is no longer in list.
 */
removeFromInitList(){
	let leader = getPrev();
	let follower = this.getNext();
	if ( (leader===this) && (follower===this) ) { // not in or last performer in list
		if ( !this.encounter.first || !this.encounter.current ) { // there is no list - no need to do anything
			return true;
		} else { // only performer in list - end combat
			return this.encounter.EncounterEnd();
		}			
	}
	ErrFatalDisplayIf( (leader===this) || (follower===this) 
		|| !leader || !follower, "Corrupt initiative list.");
	/* Isolate this performer from the initiative list. */
	// set next to this
	setNext(this, this);
	// set previous to this 
	setPrev(this, this);
	/* Close the hole in the initiative list.*/
	// set leader's next to follower
	if (leader) setNext(leader, follower);
	// set follower's previous to leader
	if (follower) setPrev(follower, leader);
	// If removing the first performer in init list, make the next one first.
	if (this === this.encounter.first) {
		this.encounter.first = follower;
	}
	// If removing the current performer, make the next one current.
	if (this === this.encounter.current) {
		this.encounter.current = follower;
	}
	return true;
}

/* Prepare to add this performer to the initiative list. */
activate(){
	// set active flag
	setActive(true);

	if ( this.encounter.round >= 0 ) { // combat in progress
		this.addToInitList();
	}
}

/* Return false if performer is last on list and user declines to end encounter */
deactivate(){
	if (this.isActive() === true) {// removing inactive performer from list might end encounter
    	if ( this.removeFromInitList() ) {
			// unset active flag
    		this.setActive(false);
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
die() {
	let nextOld = this.getNext();
	if ( this === nextOld  || !nextOld ) { // last in init list or corrupt list
		this.encounter.EncounterEnd();
	} else {
		if (nextOld === this.encounter.first) this.encounter.round++;
		this.deactivate();
		this.encounter.current = nextOld;
		this.encounter.current.prepareToAct();
		// Controller logic should do this: FrmGotoForm(PlayFormID);
	}
}
} // end of Performer class

class Character extends Performer {

// Set initiative value to roll plus modifier
setInitiativeRoll(roll) {
	setInitiative(roll + getModifier() );
}

// Insert performer into initiative list at appropriate location
addToInitList(){
	if (!this.encounter.first) { // list is empty
		// make this the first performer
		this.encounter.first = this;
		return;
	}
	/* Start at begining of initiative list. Look for first performer
	   with lower initiative, and insert this character before it. */
	let that = this.encounter.first;
	do {
		let initThis = this.getInitiative();
        let initThat = that.getInitiative();
        if (initThis === initThat) {
        /* Ignore ties with effects - characters always go after effects */
        	if (that.getType()===PerformerTypeEnum.CHARACTER) {
				/* Compare initiative modifiers. */
				let modThis = this.getModifier();
				let modThat = that.getModifier();
				if (modThis === modThat) {
    		        // Display Resolve Tie Confirmation
       				let nameThis = getNameHandle(this);
       				let nameThat = getNameHandle(that);
    				let result = ResolveTieDialog(nameThat, nameThis);
            		if (result === 1) { // this character is faster than that
            			if (that===this.encounter.first) this.encounter.first = this;
    					// insert this character before the slower Performer
            			that.insertBefore(this); 
            			return;
       				}
				} else if (modThis > modThat) { // that performer is slower
					if (that===this.encounter.first) this.encounter.first = this;
		        	that.insertBefore(this);
	    		    return;
				} // end of modifier comparison condition
			} // end of CHARACTER conditional
        } else if (initThis > initThat) { // that performer is slower
			if (that === this.encounter.first) this.encounter.first = this;
        	that.insertBefore(this);
	        return;
        } // end of initiative comparison conditional
		that = that.getNext();
	} while (that !== this.encounter.first);
	// Make this character last
	this.encounter.first.insertBefore(this);
}

// Return true if the character only gets a Standard or Move action this round
hasStandardAction() {
	// All actions are standard or move actions during surprise round
	if (this.encounter.round===0) return true;
	// If character is taking a readied action, it must be a standard or move action
	return isTakingReadied(this);
}

// Prepare performer to take a turn
prepareToAct() {
    if ( (this.encounter.round===0) && (isAware(this) === false ) ) {
	    // character surprised on surprise round!
    	setAware(this, true); // make it aware
	    this.act(); // Skip the current character
    }
    // if character was delaying, it no longer is
    this.setDelaying(false);
    // can't hold readied action any longer either
    this.setReadied(false);
}

// Have performer pass the current pointer to the next performer
act() {
	this.setTakingReadied(false);
	this.encounter.current = this.getNext();
	if (this.encounter.current === this.encounter.first) { // Start of the list - new round
		this.encounter.round++;
	}
	this.encounter.current.prepareToAct();
}

/*
 * Take steps necessary to move character to new, later initiative.
 * If initNew is not a number, just leave it unspecified and continue.
 */
delay(initNew) {
  if (typeof initNew !== "number") {
    setDelaying(this, true);
    // go on to next character
    this.act();
    return;
  }
	// Verify that this is a valid value
	if (initNew > this.getDelayInitMax())
		return;
	let nextOld = this.getNext();
	let firstOld = this.encounter.first;
    this.setInitiative(initNew);
	setDelaying(this, true);
	if (this !== nextOld) {// not only performer in initiative list
		// take out of initiative list
		this.deactivate();
		// put character back into initiative list
		this.activate();
	}
	// go to old next character (if order changed)
	let nextNew = this.getNext();
	if ( (nextOld===nextNew) && (firstOld===this.encounter.first) ) { // no change to init list
		this.encounter.current = this; // let this performer go again with new initiative
	} else { // order changed
		this.encounter.current = nextOld; // continue with next performer
	}
	this.encounter.current.prepareToAct();
}

/*
 * Take delayed action after the given performer.
 */
delayTrigger(p) {
	// sanity check...
	ErrNonFatalDisplayIf(isDelaying(this)===false,
		"Triggering delayed action for non-delaying character!");
	/* Warn user about potentially abusive action: Suppose this character
	 * is taking a readied action to interrupt character x. If character
	 * p was delaying and decides to follow this character, character p will
	 * go before character x. Delaying should not allow characters to effectively
	 * interrupt other characters. */
	if (p.getType()===PerformerTypeEnum.CHARACTER) {
		if (this.encounter.current.isTakingReadied()) {
			if (FrmAlert(ReadiedTriggeringAlertID)) {
				return;
			}
		}
	}
	// prepare for moving to new list location
	/* Assume that this is not the only performer in initiative list,
	   for if it was it could not have been selected by trigger form. */
    this.removeFromInitList();
	// reset initiative value
    this.setInitiative(p.getInitiative());
	// move to new list location
    p.insertAfter(this);
	// clear delaying status
    setDelaying(false);
}

// Return the highest initiative the character may delay to
getDelayInitMax() {
	// maximum delaying initiative is current initiative minus one
	return getInitiative() - 1;
}

/*
 * Have character take Ready action: go into ready state and continue with next character
 */
ready() {
	setReadied(this, true);
	this.act();
}

/*
 * Take readied action before the given performer.
 */
interrupt(p) {
	// sanity check
	ErrNonFatalDisplayIf(!isReadied(this),"Interrupting with non-readied character!");
	// prepare for moving to new list location
	/* Assume that this is not the only performer in initiative list,
	   for if it was it could not have been selected by trigger form. */
	this.removeFromInitList();
	// reset initiative value
	this.setInitiative(p.getInitiative());
	// move to new list location
	p.insertBefore(this);
	// clear readied status
	setReadied(this, false);
	// indicate that this is a readied action
	setTakingReadied(this, true);
	// move interrupter to start of list, if needed
	if (this.encounter.current === this.encounter.first) {
		this.encounter.first = this;
	}
	// make interrupter current
	this.encounter.current = this;
}

/*
 * Change character's initiative to 20+bonus. This action is not present
 * in the Revised (v3.5) SRD, but it retained for players who want to
 * use it as a house-rule.
 */
refocus() {
	if (isTakingReadied(this)) {
		// can't Refocus on Readied action... would allow a move+Refocus
		return;
	}
	let oldNext = this.getNext();
	// Is this the last performer in the initiative list?
	let wasLast = (oldNext===this.encounter.first);
	// Is this the current performer?
	let wasCurrent = (this===this.encounter.current);
	// reset initiative
	this.setInitiative( 20 + getModifier() );
	if (this !== oldNext) { // This is not the only performer in initiative list
		// remove from list
		this.deactivate();
		// return to list
		this.activate();
	}
	/* If for some reason we are refocusing non-current character, 
	   then no more changes needed. */
	if (wasCurrent===false) {
		return;
	} else if ( wasLast && (this===this.encounter.first) ) {
	/* If current performer was last, and is now first, 
	   then it's a new turn and it goes again. */
		this.encounter.round++;
		this.encounter.current = this;
	} else {
	/* Otherwise go on to old next performer. */
		this.encounter.current = oldNext;
	}
	this.encounter.current.prepareToAct();
}
} // end of class Character

class Effect extends Performer {

// Add effect to initiative list
activate() {
	// if adding to initiative list before combat, reset the rounds remaining
	if (this.encounter.round < 0) setRemaining( this, getNominal(this) );
	super.activate();
}

// Remove effect from initiative list
deactivate() {
	if ( super.deactivate() ) {
	
    	// deactivate all summoned performers
    	var p;
		// this forces deactivation, regardless if it would end combat
    	while ( p = removeSummoned(this) ) {
    		p.deactivate();
    	}
		
		return true;

	} else { // This effect was not deactivated
		return false;
	}
}

/* Reduce the number of rounds remaining by one, to minimum of zero. */
decrement() {
	var i = getRemaining(this);
	if (i > 0) {
		i--;
		setRemaining(this, i);
	}
}

/* "Effects that last a certain number of rounds end just before the same 
    initiative count that they began on." (SRD) */
addToInitList(){
	if (!this.encounter.first) { // list is empty
		// make this the first performer
		this.encounter.first = this;
		return;
	}
	let initThis = this.getInitiative();
	var p = this.encounter.first;
	do { 
		if (initThis >= p.getInitiative()) { // current performer is slower
			if (p.getType() === PerformerTypeEnum.CHARACTER) { 
				// effects always go before characters with same initiative
				// this means effects with same init will go in order added to list
				p.insertBefore(this);
				// update start of initiative list, if necessary
				if (p===this.encounter.first) this.encounter.first = this;
				// we're done
				return;
			}
		} // end of initiative comparison conditional
		p = p.getNext();
	} while (p !== this.encounter.first);
	// No characters were slower, make this effect last.
	this.encounter.first.insertBefore(this);
}

// Prepare performer to take a turn
prepareToAct() {
	decrement(this);
}

/* This effect acts, then makes its next performer the current performer. */
act() {
	let next = this.getNext();
    if ( getRemaining(this) < 1) {
        // Effect is done - get rid of it
		if (next === this) { // only performer in initiative list!
			this.encounter.EncounterEndForce();
			return;
		}
        this.deactivate();
	}
	this.encounter.current = next;
    if (this.encounter.first === this.encounter.current) this.encounter.round++; // last performer in init list 
	this.encounter.current.prepareToAct();
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
summon(p, followOwner) {
	var owner;

	// Add the performer to the effect's collection of summoned things
	addSummoned(this, p);
	
	if (followOwner && ( (owner=getOwner(this)) && owner.isActive() )) {
		// Set the summoned thing to follow the owner in the initiative list (if any)
		p.setInitiative( owner.getInitiative() );
		
		if (this.isActive()) { 
			// only put summoned thing in combat if owner and effect are both in combat
			p.setActive( true );
			
			if (this.encounter.round >= 0) // only put summoned thing in combat if there is a combat underway
				owner.insertAfter(p);
		}
			
	} else { // followOwner is false, or there is no owner, or owner is not in combat

		// Set the summoned thing to follow the effect in the initiative list (if any)
		p.setInitiative( this.getInitiative() );

		if (this.isActive()) { // only put summoned thing in combat if effect is in combat
			p.setActive(true);

			if (this.encounter.round >= 0) // only put summoned thing in combat if there is a combat underway
				this.insertAfter(p);
		}
	}
}
}
