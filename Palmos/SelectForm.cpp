// Copyright 2017 Peter L Mikelsons

#include "Main.h"
#include "CombatQOGC.h"

// string resource ID to display on screen
static UInt16 TextStringID;
// record handles of items in list
static MemHandle* itemsHandle = NULL;
// names of items in list
static Char** itemsText = NULL; 
// object references for items in the list
static Performer** itemsP = NULL; 
// number of items in the list
static Int16 numItems = 0; 

// pointer to function to perform when item is selected
static void (*SelectionAction)(Int16 sel);

/*
 * Clean up memory assigned to globals used to pass data to event handling
 * function. Must be called after FrmPopup*Form functions are called.
 */
static void FreeSelectHandles(void) {
	// clean up memory
	for (UInt16 i=0; i<numItems; i++) {
		Err e = MemHandleUnlock(itemsHandle[i]);
		ErrFatalDisplayIf(e!=0,"Could not unlock record handle.");
	}
	MemPtrFree(itemsHandle);
	MemPtrFree(itemsText);
	MemPtrFree(itemsP);
}

/*
 * Generic selection form display.
 * First argument is resource ID of alert displayed when there is nothing
 * to choose from.
 * Second argument is pointer to boolean function that tests whether a
 * performer should be on the list.
 */
static void FrmPopupSelectForm(UInt16 emptyListAlertID, 
						Boolean (*test)(Performer* p) ) {
	Err e = errNone;
	// look through entire initiative list for delaying characters
	Performer* p = first;
	numItems = 0;
	// count number of characters in list
	do {
		if ( ((*test)(p)) == true ) {
			numItems++;
		}
		p = p->getNext();
	} while ( p != first );

	if (numItems == 0) {
		FrmAlert(emptyListAlertID);
		return;
	}
		
	/* allocate memory for global arrays */
	itemsHandle = (MemHandle*) MemPtrNew(numItems * sizeof(MemHandle) );
	itemsP = (Performer**) MemPtrNew(numItems * sizeof(Performer*) );
	itemsText = (Char**)MemPtrNew(numItems * sizeof(Char*) );
	numItems = 0;
	do {
		if ( ((*test)(p)) == true ) {
            // assign this object to array
            itemsP[numItems] = p;
            // assign this handle and name to arrays
            itemsHandle[numItems] = p->getNameHandle( &(itemsText[numItems]) );
            numItems++;
		}
		p = p->getNext();
	} while ( p != first );

	FrmPopupForm(SelectFormID);
	// statements after the last one get executed before form is shown
}

/*
 * Clean up various things when closing this form.
 */
static void CloseSelectForm(void) {
    // free the database string's handle from the field
	FormType* form = FrmGetActiveForm(); 
    FldSetTextHandle((FieldType*)GetObjectPtr(form,SelectTextFieldID),NULL);

	FreeSelectHandles();	
}

/*
 * Test function for FrmPopupSelectForm() - determines whether performer
 * should be displayed.
 */
static Boolean isDelayingCharacter(Performer* p) {
    if ( p->getType()==CHARACTER ) {
    	Character* c = (Character*) p;
    	if (c->isDelaying()==true) {
    		return true;
    	}
    }
    return false;
}
/*
 * What to do when something is chosen from the delaying character set.
 */
static void TriggerAction(Int16 sel) {
	( (Character*) itemsP[sel] )->delayTrigger(current);
    CloseSelectForm();
    FrmReturnToForm(0);
}
/*
 * Open a form with a list for choosing one delaying character to act.
 */
void FrmPopupTriggerForm() {
	// assign globals
	TextStringID = SelectTriggerStringID;
	SelectionAction = TriggerAction;
	// call the generic function
	FrmPopupSelectForm(NoDelayingCharactersAlertID, isDelayingCharacter);
}

/*
 * Test function for FrmPopupSelectForm() - determines whether performer
 * should be displayed.
 */
static Boolean isReadiedCharacter(Performer* p) {
    if ( p->getType()==CHARACTER ) {
    	Character* c = (Character*) p;
    	if (c->isReadied()==true) {
    		return true;
    	}
    }
    return false;
}
/*
 * What to do when something is chosen from the readied character set.
 */
static void InterruptAction(Int16 sel) {
	( (Character*) itemsP[sel] )->interrupt(current);
    CloseSelectForm();
    FrmReturnToForm(0);
	// go to the play form for the interrupter
	FrmGotoForm(PlayFormID);
}
/*
 * Open a form with a list for choosing one delaying character to act.
 */
void FrmPopupInterruptForm() {
	// assign globals
	TextStringID = SelectInterruptStringID;
	SelectionAction = InterruptAction;
	// call the generic function
	FrmPopupSelectForm(NoReadiedCharactersAlertID, isReadiedCharacter);
}

/*
 * Test function for FrmPopupSelectForm() - determines whether performer
 * should be displayed.
 */
static Boolean isKillable(Performer* p) {
	// any other performer can be killed
    return (current != p);
}
/*
 * What to do when something is chosen for killing.
 */
static void KillAction(Int16 sel) {
	itemsP[sel]->deactivate();
   	// always do this before leaving form to clean up globals
   	CloseSelectForm();
	// no need to redraw
	FrmReturnToForm(0);
}
/*
 * Open a form with a list for choosing one delaying character to act.
 */
void FrmPopupKillForm() {
	// assign globals
	TextStringID = SelectKillStringID;
	SelectionAction = KillAction;
	// call the generic function
	FrmPopupSelectForm(NoOtherPerformersAlertID, isKillable);
}

/* 
 * Process input-event to select form dialog.
 */
Boolean EventHandlerSelectForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form;
	ListType* list;
    switch (event->eType) {
        case frmOpenEvent:
			form = FrmGetActiveForm();
			list = (ListType*) GetObjectPtr(form,SelectListID);
			LstSetListChoices(list,itemsText,numItems);
            FrmDrawForm(form); // must do this before FldDrawField, below
			// set up field
			{
				// get handle for string resource
                MemHandle h = DmGetResource('tSTR',TextStringID);
                ErrFatalDisplayIf(h==NULL,"Failed to find text string resource.");
				// get pointer to field
				FieldType* field = (FieldType*)GetObjectPtr(form,SelectTextFieldID);
				// detach old handle, if any, from field
                FldSetTextHandle(field,NULL);
                // attach the resource to the field
                FldSetTextHandle(field,h);
                FldDrawField(field);
			}
            handled = true;
            break;
        case ctlSelectEvent: // button pushed
            switch (event->data.ctlSelect.controlID) {
                case (DoneID): 
					form = FrmGetActiveForm();
					{
						// get index of selected performer
						Int16 sel = LstGetSelection(
							(ListType*)GetObjectPtr(form,SelectListID)
						);
						// call the appropriate function for this screen
						if (sel != noListSelection) {
							(*SelectionAction)(sel);
						}
					}
					handled = true;
                    break;
                case (CancelID): 
                    // close form
					CloseSelectForm();
                    FrmReturnToForm(0);
                    handled = true;
                    break;
            }
            break;
        case frmCloseEvent:
			CloseSelectForm();
            break;
    }
    return handled;
}

