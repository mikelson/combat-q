/*
 * Source code for the Edit Character Form.
 */

#include "Main.h"
#include "CombatQOGC.h"
#include "Performer.h"

// Initiative modifier to display for new character
#define DEFAULT_MODIFIER 0
// Spell counter to display for new character
#define DEFAULT_COUNTER 1
// Default spell duration to display for new character
#define DEFAULT_DURATION 3

Character* character = NULL;
Char* creatorName = NULL;

/* Pop up form for editing existing character. */ 
void Character::popupEditForm(void) {
	character = this;
	creatorName = NULL;
	FrmPopupForm(EditCharFormID);
}

/* Pop up form for creating new character. 
   Argument will be used for generating name. */
void FrmPopupEditCharForm(Char* creatorName_) {
	character = NULL;
	creatorName = creatorName_;
	FrmPopupForm(EditCharFormID);
}

/* 
 * Process input-event to form.
 */
Boolean EventHandlerEditCharForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form;
	FieldType* field;
    switch (event->eType) {
        case frmOpenEvent: // draw the form on the screen
            form = FrmGetActiveForm();
            FrmDrawForm(form);

			if (character == NULL) { // creating a new character - put default values in fields

				/* Name */
				/* default name is: <creator><creatorSuffix><base><number> */

				// create creator and creatorSuffix strings, if any
				Char* creator;
				Char* creatorSuffix;
				if (creatorName == NULL) {
					creator = "";
					creatorSuffix = "";
				} else {
					creator = creatorName;
					MemHandle h = DmGetResource('tSTR',DefaultCharNameStringID);
					creatorSuffix = (Char*) MemHandleLock(h);
				}

        		// create base
        		Char* base = "NPC ";

				// create number
				Char numS[maxStrIToALen];
        		StrIToA(numS,counterCharacterName);

				// make result big enough for everything
        		Char name[StrLen(creator) + StrLen(creatorSuffix) 
						+ StrLen(base) + StrSize(numS)];
				
        		// concatenate everything
				StrCopy(name, creator);
				StrCat(name, creatorSuffix);
        		StrCat(name, base);
        		StrCat(name, numS);
				FldSet(form,NameFieldID,name);
					
				// release memory locked for generating name
				if (creatorName != NULL) MemPtrUnlock(creatorSuffix);
			
				/* Modifier */
				StrIToA(numS, DEFAULT_MODIFIER);
				FldSet(form,ModifierFieldID,numS);
				
				/* Effect Name Counter */
				StrIToA(numS, DEFAULT_COUNTER);
				FldSet(form,CountFieldID,numS);
				
				/* Default Effect Duration */
				StrIToA(numS, DEFAULT_DURATION);
				FldSet(form,DurationFieldID,numS);
				
				/* Check Boxes */
				// TODO - F2 - replace hardwired defaults with variables for default conditions
				CtlSetValue((ControlType*)GetObjectPtr(form,PlayerCharacterCBID), 0);
				CtlSetValue((ControlType*)GetObjectPtr(form,DelayingCBID), 0);
				CtlSetValue((ControlType*)GetObjectPtr(form,ReadiedCBID), 0);
				CtlSetValue((ControlType*)GetObjectPtr(form,TakingReadiedCBID), 0);
				CtlSetValue((ControlType*)GetObjectPtr(form,AwareCBID), 1);
				
			} else { // editing an existing character - put current values in fields
				Char numS[maxStrIToALen];

				/* Character's Name Field */
				Char* s;
				MemHandle h = character->getNameHandle(&s);
				FldSet(form,NameFieldID,s);
				MemHandleUnlock(h);

				/* Modifier Field */
				StrIToA(numS, character->getModifier());
				FldSet(form,ModifierFieldID,numS);
				
				// TODO - F2 - Add check boxes for editing Delaying, Readied, Aware statii?
				/* Delaying Check Box */
				/* CtlSetValue(
					(ControlType*)GetObjectPtr(form,DelayingCBID),
					character->isDelaying() 
				); */
				
				/* Effect Name Counter Field */
				StrIToA(numS, character->getCounter());
				FldSet(form,CountFieldID,numS);
				
				/* Character's Duration Field */
				StrIToA(numS, character->getDuration());
				FldSet(form,DurationFieldID,numS);

				/* Check Boxes */
				CtlSetValue((ControlType*)GetObjectPtr(form,PlayerCharacterCBID),
							 character->isPC());
				CtlSetValue((ControlType*)GetObjectPtr(form,DelayingCBID),
							 character->isDelaying());
				CtlSetValue((ControlType*)GetObjectPtr(form,ReadiedCBID),
							 character->isReadied());
				CtlSetValue((ControlType*)GetObjectPtr(form,TakingReadiedCBID), 
							 character->isTakingReadied());
				CtlSetValue((ControlType*)GetObjectPtr(form,AwareCBID),
							 character->isAware());
			}
			/* Finish setting up name field */
			field = (FieldType*) GetObjectPtr(form,NameFieldID);
			// select whole thing
            FldSetSelection(field,0,StrLen(FldGetTextPtr(field)));
			// put focus there
			FrmSetFocus(form, FrmGetObjectIndex(form,NameFieldID) );
			// ...and redraw
			FldDrawField(field);			
            // set up Graffiti shift state to upper shift on
            GrfSetState(false,false,true);
			handled = true;
            break;
        case ctlSelectEvent: // button pushed
            form = FrmGetActiveForm();
            switch (event->data.ctlSelect.controlID) {
				case DoneID:
					form = FrmGetActiveForm();
					{
						/* Validate fields as needed. 
						   Save strings found for assignment, if warranted. */
						Boolean valid = true;
						Char* modifierS = NULL;
						
						// TODO - F2 - Validate new name

						/* Initiative Modifier */
						field = (FieldType*) GetObjectPtr(form,ModifierFieldID);
						if (FldDirty(field)) {
							modifierS = FldGetTextPtr(field);
							if ( StrIsI(modifierS) == false ) {
								FrmAlert(InvalidModifierID);
								valid = false;
							} 
						}

						/* Spell Counter */
						Int32 count = -1;
						field = (FieldType*) GetObjectPtr(form,CountFieldID);
						if (FldDirty(field)) {
							// field is numeric, so count will be non-negative number
							count = StrAToI(FldGetTextPtr(field));
							if ( (count<0) || (count > 255) ) {
								FrmAlert(SpellNumberAlertID);
								valid = false;
							} 
						}

						/* Default duration */
						Int32 duration = -1;
						field = (FieldType*) GetObjectPtr(form,DurationFieldID);
						if (FldDirty(field)) {
							duration = StrAToI(FldGetTextPtr(field));
							if ( (duration<0) || (duration > 255) ) {
								FrmAlert(DefaultDurationAlertID);
								valid = false;
							} 
						}
						
						if (valid == true) {
							// Get value of checkbox(es) for later
							Boolean isPC = 
								CtlGetValue( 
									(ControlType*) GetObjectPtr(form,PlayerCharacterCBID) 
								) != 0;
								
							if (character == NULL) { // create new character
        						field = (FieldType*) GetObjectPtr(form,NameFieldID);
        						if (FldDirty(field)==false) // user did not touch default name 
									counterCharacterName++;
								// Get name
								Char* name = FldGetTextPtr(field);

								Int16 mod = (modifierS==NULL)? 
									DEFAULT_MODIFIER // modifier field was clean 
									: StrAToI(modifierS); // convert dirty field entry

								if (count < 0) // count field was clean
									count = DEFAULT_COUNTER;
									
								if (duration < 0) // duration field was clean
									duration = DEFAULT_DURATION;

								character = new Character(name,mod,count,duration);
								
								// Assign value of checkbox(es) to character's flags
    							character->setPC(isPC);

								// Make New Player Character Active
								if (isPC == true) character->activate();
							
							} else { // edit old character
        						/* Name */
        						field = (FieldType*) GetObjectPtr(form,NameFieldID);
        						if (FldDirty(field)) {
        							character->setName( FldGetTextPtr(field) );
        						}
        						
        						/* Initiative Modifier */
        						if (modifierS != NULL) {
        							character->setModifier( StrAToI(modifierS) );
        						}
        						
        						/* Effect Name Counter */
        						if (count >= 0) {
        							character->setCounter( count );
        						}
        						
        						/* Duration */
        						if (duration >= 0) {
        							character->setDuration( duration );
        						}

								/* Flag(s) */
    							character->setPC(isPC);

							} // end of new/old character conditional
							
							// Close this form
							FrmReturnToForm(0);
							// If returning to Overview Form, tell it to display this character
							OverviewFormUpdate( character );
						} // end of valid conditional
    				}
					handled = true;
					break;
				case DeleteID:
					// have user verify
					if (FrmAlert(VerifyDeleteAlertID)==0) {
						if (character == NULL) { // no character to delete, just exit
							FrmReturnToForm(0);
						} else {
							if (character->deactivate()==true) { // character was deactivated
								// exit form
								FrmReturnToForm(0);
								// free object from heap
								delete character;
								character = NULL;
								// if returning to overview form, update it
								OverviewFormUpdate(NULL);
							}
						}
					}
					handled = true;
					break;
				case CancelID:
					FrmReturnToForm(0);
					handled = true;
					break;
            }
            break; // end of control selected events
		case frmCloseEvent: // form closed
			break;
        default:
            break;
    }
    return handled;
} // end of function EventHandlerEditCharForm
