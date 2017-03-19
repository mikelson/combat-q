/*
 * Source code for the Edit Effect Form.
 * Copyright 2017 Peter L Mikelsons
 */

#include "Main.h"
#include "CombatQOGC.h"
#include "Performer.h"

Character* owner = NULL;
MemHandle ownerNameHandle = 0;
Effect* effect = NULL;

/* Pop up form for editing existing effect. */
void Effect::popupEditForm(void) {
	effect = this;
	owner = getOwner();
	FrmPopupForm(EditEffectFormID);
}

/* Pop up form for editing new effect. Argument is effect's owner. */
void FrmPopupEditEffectForm(Character* c) {
	owner = c;
    effect = NULL;
	FrmPopupForm(EditEffectFormID);
}

/* Node in a Singly Linked List of Performer* data */
class PerfNode {
public:
	PerfNode(Performer* value) {
		perf = value;
		next = NULL;
	}
	~PerfNode(void) {
		if (next != NULL) {
			delete next;
			next = NULL;
		}
	}
	/* Add a node to list. */
	void append(PerfNode* newTail) {
		if (next == NULL)
			next = newTail;
		else
			next->append(newTail);
	}
	PerfNode*  getNext(void) { return next; }
	Performer* getPerf(void) { return perf; }
private:
	Performer* perf;
	PerfNode* next;
};
/* The first node in a Linked List used to buffer Performers before adding
   them to an effect's summoned creatures set. */
static PerfNode* firstSummoned = NULL;

/* Clean up stuff needed to display and edit effects */
static void FreeMemory(void) {
	if (owner == NULL) { // free "No Owner" string from owner field
		FormType* form = FrmGetActiveForm();
		FieldType* field = (FieldType*) GetObjectPtr(form,OwnerFieldID);
		FldSetTextHandle(field,NULL);
	} else { // free owner's name handle, which was locked by getNameHandle()
		MemHandleUnlock(ownerNameHandle);
	}
	// free the summoned creatures linked list
	if (firstSummoned != NULL) delete firstSummoned;
	firstSummoned = NULL;
}

/* 
 * Process input-event to form.
 */
Boolean EventHandlerEditEffectForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form;
	FieldType* field;
    switch (event->eType) {
        case frmOpenEvent: // draw the form on the screen
            form = FrmGetActiveForm();
            FrmDrawForm(form);
			/* Put owner's name, if any, into field */
			field = (FieldType*) GetObjectPtr(form,OwnerFieldID);
			if (owner == NULL) {
				ownerNameHandle = DmGetResource('tSTR',NoOwnerStringID);
				FldSetTextHandle(field,NULL);
				FldSetTextHandle(field,ownerNameHandle);				
			} else {
				Char* name;
				ownerNameHandle = owner->getNameHandle(&name);
				FldSetTextPtr(field,name);
			}
			FldDrawField(field);

			FrmSetFocus(form,FrmGetObjectIndex(form,NameFieldID));
			if (effect==NULL) { // creating new effect - put defaults in fields
				Char counterS[maxStrIToALen];
				/* Name field */
				// default name is "<owner's name><constant string><owner's counter>"
				// <owner's name> will be truncated if default name is too long

				// get <owner's name>
				Char* ownerS = FldGetTextPtr(field);

				// get <constant string>
				MemHandle h = DmGetResource('tSTR',DefaultEffectNameStringID);
				Char* constantS = (Char*) MemHandleLock(h);

				// get and increment <owner's counter>
				StrIToA(counterS,owner->getCounter());
				// increment counter later if new effect is accepted

				// concatenate above strings

				// Copy first part of owner's name to effectS
				Char effectS[MAX_NAME_BYTES];
                WChar ch;
				UInt16 ownerSLength = StrLen(ownerS);
                UInt16 suffixSize = StrLen(constantS) + StrSize(counterS);
                UInt16 iNext = 0;
				UInt16 iLast = 0;
		        while ( (iNext < MAX_NAME_BYTES - suffixSize) && (iNext < ownerSLength) ) {
                    iNext += TxtGlueGetNextChar(ownerS, iLast, &ch);
                    TxtGlueSetNextChar(effectS, iLast, ch);
                    iLast = iNext;
                }
				// Null-terminate effectS
				TxtGlueSetNextChar(effectS, iLast, chrNull);

				// Append constant and counter to effectS
				StrCat(effectS,constantS);
				StrCat(effectS,counterS);
				
				MemHandleUnlock(h); // done with constant string's resource
				// preset field
				FldSet(form,NameFieldID,effectS);

				/* Duration Field*/
				// preset to owner's duration
				// resuse counterS, from above
				StrIToA(counterS, owner->getDuration());
				FldSet(form,DurationFieldID, counterS);
				
				/* Rounds Remaining Field */
				// preset to exactly the same as duration
				FldSet(form,RemainingFieldID,counterS);
			} else { // editing an existing effect - put current values in fields
				Char counterS[maxStrIToALen];
				/* Effect's Name Field */
				Char* s;
				MemHandle h = effect->getNameHandle(&s);
				FldSet(form,NameFieldID,s);
				MemHandleUnlock(h);
				/* Effect's Duration Field */
				StrIToA(counterS, effect->getNominal());
				FldSet(form,DurationFieldID,counterS);
				/* Rounds Remaining Field */
				StrIToA(counterS, effect->getRemaining());
				FldSet(form,RemainingFieldID,counterS);
			}
            handled = true;
            break;
		case frmUpdateEvent:
			form = FrmGetActiveForm();
			if (event->data.frmUpdate.updateCode != frmRedrawUpdateCode) {
				/* Add indicated character to summoned creature's list */

				UInt16 iEnc = event->data.frmUpdate.updateCode;
               	// remap extreme values of update-code/database-index
				iEnc = swapUpdateCodeForMaxRecordIndex( iEnc );
				Performer* p = queryPerformerP(iEnc);

				if (effect == NULL) { // creating new effect
					// store this summoned creature in the buffer list
					PerfNode* node = new PerfNode(p);
					if (node == NULL) { // sanity check - allocation failed!
						delete p;
						p = NULL;
						ErrNonFatalDisplayIf(node == NULL, 
							"Memory allocation failed - could not tie creature to effect!");
					} else // memory allocation successfull
						if (firstSummoned == NULL) // list is currently empty
							firstSummoned = node;
						else
							firstSummoned->append(node);
				}
				else { // editing an existing effect
					// store this summoned creature in the effect
					effect->summon(p, false);
				}
			}
			break;
        case ctlSelectEvent: // button pushed
            form = FrmGetActiveForm();
            switch (event->data.ctlSelect.controlID) {
				case EffectSummonBID: // Summon Creature Button pushed
					{
						// get current name of effect
						field = (FieldType*) GetObjectPtr(form,NameFieldID);
    					Char* name = FldGetTextPtr(field);
						
						// pop up form for editing new character using current name
						FrmPopupEditCharForm(name);
					}
					handled = true;
					break;
				case DoneID:
					form = FrmGetActiveForm();
					{
						/* Validate fields as needed. 
						   Save strings found for assignment, if warranted. */
						Boolean valid = true;

						/* Nominal Duration */
						Int32 duration = -1;
						field = (FieldType*) GetObjectPtr(form,DurationFieldID);
						if (FldDirty(field)) {
							duration = StrAToI(FldGetTextPtr(field));
							if ( (duration<0) || (duration > 255) ) {
								FrmAlert(NominalDurationAlertID);
								valid = false;
							} 
						}
						/* Rounds Remaining */
						Int32 remaining = -1;
						field = (FieldType*) GetObjectPtr(form,RemainingFieldID);
						if (FldDirty(field)) {
							remaining = StrAToI(FldGetTextPtr(field));
							if ( (remaining<0) || (remaining > 255) ) {
								FrmAlert(RemainingDurationAlertID);
								valid = false;
							} 
						}

						if (valid == true) {
        					if (effect==NULL) { // creating new effect
        						field = (FieldType*) GetObjectPtr(form,NameFieldID);
        						Char* name = FldGetTextPtr(field);

        						field = (FieldType*) GetObjectPtr(form,DurationFieldID);

        						// assume DurationFieldID is numeric only
        						duration = StrAToI(FldGetTextPtr(field));

        						field = (FieldType*) GetObjectPtr(form,RemainingFieldID);
        						// assume RemainingFieldID is numeric only
        						remaining = StrAToI(FldGetTextPtr(field));

        						effect = new Effect(name,duration,remaining,owner);

        						// increment owner's effect-name counter
        						owner->incrementCounter();
								
								// copy summoned creatures list from buffer to effect
								PerfNode* temp = firstSummoned;
								while (temp != NULL) {
									Performer* p = temp->getPerf();
									effect->summon( p, true );
									temp = temp->getNext();
								}
        					} else { // editing existing effect
        						field = (FieldType*) GetObjectPtr(form,NameFieldID);
        						if (FldDirty(field)) {
        							effect->setName( FldGetTextPtr(field) );
        						}
        						if (duration >= 0) {
        							effect->setNominal( duration );
        						}
        						if (remaining >= 0) {
        							effect->setRemaining( remaining );
        						}
        					}
							FreeMemory();
							FrmReturnToForm(0);
							OverviewFormUpdate(effect);
						}
					}
					handled = true;
					break;
				case DeleteID:
					if (effect == NULL) { // no effect to delete, just exit
						FreeMemory();
						FrmReturnToForm(0);
					} else {
						if (effect->deactivate()==true) { // effect was deactivated
							// exit form
							FreeMemory();
							FrmReturnToForm(0);
							// free object from heap
							delete effect;
							effect = NULL;
							// if returning to overview form, update it
							OverviewFormUpdate(NULL);
						}
					}
					handled = true;
					break;
				case CancelID:
					FreeMemory();
					FrmReturnToForm(0);
					handled = true;
					break;
            }
            break; // end of control selected events
		case frmCloseEvent: // form closed
			FreeMemory();
			break;
        default:
            break;
    }
    return handled;
} // end of function EventHandlerEditEffectForm
