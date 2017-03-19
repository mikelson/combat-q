#include "Main.h"
#include "CombatQOGC.h"
#include "Performer.h"

void Character::setUpPlayForm(FormType* form) {
    // Hide effect duration labels
    FrmHideObject(form,FrmGetObjectIndex(form,
    	EffectDurationLabelID));
    FrmHideObject(form,FrmGetObjectIndex(form,RemainingFieldID));
	// Hide Effect-only buttons
    FrmHideObject(form,FrmGetObjectIndex(form,EffectContinueBID));
	// Show Character-only buttons
    FrmShowObject(form,FrmGetObjectIndex(form,CharCreateEffectBID));
    FrmShowObject(form,FrmGetObjectIndex(form,CharDelayBID));
    FrmShowObject(form,FrmGetObjectIndex(form,CharReadyBID));
    FrmShowObject(form,FrmGetObjectIndex(form,InterruptBID));
    FrmShowObject(form,FrmGetObjectIndex(form,TriggerBID));
    if ( hasStandardAction() ) { // standard or move action
    	FrmHideObject(form,FrmGetObjectIndex(form,CharNormalBID));
    	FrmShowObject(form,FrmGetObjectIndex(form,CharStandardBID));
    } else { // normal action
    	FrmHideObject(form,FrmGetObjectIndex(form,CharStandardBID));
    	FrmShowObject(form,FrmGetObjectIndex(form,CharNormalBID));
    }
}

void Effect::setUpPlayForm(FormType* form) {
    // Rounds remaining label
    FrmShowObject(form,FrmGetObjectIndex(form,EffectDurationLabelID));
    // Rounds remaining counter
    FrmShowObject(form,FrmGetObjectIndex(form,RemainingFieldID));
    // Last round indicator
	UInt8 n = getRemaining();
    if (n > 0) {
		FldSetI(form,RemainingFieldID,n);
    } else {
		FldSetS(form,RemainingFieldID,EffectEndingString); 
    }
    // Hide character-only buttons
    FrmHideObject(form,FrmGetObjectIndex(form,CharStandardBID));
    FrmHideObject(form,FrmGetObjectIndex(form,CharNormalBID));
    FrmHideObject(form,FrmGetObjectIndex(form,CharCreateEffectBID));
    FrmHideObject(form,FrmGetObjectIndex(form,CharDelayBID));
    FrmHideObject(form,FrmGetObjectIndex(form,CharReadyBID));
    FrmHideObject(form,FrmGetObjectIndex(form,InterruptBID));
    FrmHideObject(form,FrmGetObjectIndex(form,TriggerBID));
    // Show effect-only buttons
    FrmShowObject(form,FrmGetObjectIndex(form,EffectContinueBID));
}

/* Display Screen for Current Performer */
static void UpdatePlayForm() {
    ErrFatalDisplayIf(current==NULL,"null performer!");

	Char* s;
    FormType* form = FrmGetActiveForm();

    FrmDrawForm(form);

    // Performer-Type-dependent Set Up
	current->setUpPlayForm(form);

    // Round number
    ErrNonFatalDisplayIf(round<0,"Negative round number for play!");
    if (round == 0) { // surprise round
		FldSetS(form,RoundFieldID,SurpriseRoundString);
    } else { // normal round
		FldSetI(form,RoundFieldID,(Int32)round);
    }
    // Initiative Count
    FldSetI(form,InitiativeFieldID,current->getInitiative()); 
    // Performer's Name
	/*
    Char* nameP;
    MemHandle h = current->getNameHandle(&nameP);
    FldSet(form,NameFieldID,nameP);
    MemHandleUnlock(h);
	*/
	Int16 offset, size;
	MemHandle h = current->getNameHandle(&offset,&size);
	FieldType* field = (FieldType*) GetObjectPtr(form,NameFieldID);
	FldSetText(field, h, offset, size);
	FldDrawField(field);
}

/* 	
 * Process input-event to Play form.
 */
Boolean EventHandlerPlayForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form;
    switch (event->eType) {
        case frmOpenEvent:
			UpdatePlayForm();
            handled = true;
            break;
		case frmUpdateEvent:
			UpdatePlayForm();
			handled = true;
			break;
		/* Button Pushed */
        case ctlSelectEvent:
            switch (event->data.ctlSelect.controlID) {
				case CharStandardBID: // Character taking standard or move action
                case CharNormalBID: // Character taking normal action
					current->act();
					FrmGotoForm(PlayFormID);
                    handled = true;
                    break;
				case EffectContinueBID: // Effect continuing normally
					current->act();
					// only send an update to form, because we might not be coming back here
					FrmUpdateForm(PlayFormID,frmRedrawUpdateCode);
                    handled = true;
                    break;
				case CharDelayBID: // Character using Delay action
					current->popupDelayForm();
					handled = true;
					break;
				case TriggerBID:
   					// move a delaying char to after current
   					FrmPopupTriggerForm();
					handled = true;
					break;
				case CharReadyBID:
   					((Character*)current)->ready();
   					FrmGotoForm(PlayFormID);
					handled = true;
					break;
				case InterruptBID:
					FrmPopupInterruptForm();
					handled = true;
					break;
				case KillBID:
					FrmPopupKillForm();
					handled = true;
					break;
				case CharCreateEffectBID:
					FrmPopupEditEffectForm((Character*)current);
					handled = true;
					break;
            }
            break;
        case keyDownEvent: // hardware key, silkscreen button, or Graffiti entered
            {
				form = FrmGetActiveForm();
                UInt32 keyMask = KeyCurrentState();
                if (keyMask & keyBitPageDown) {
					// same as CharActMID or EffectContinueMID
					current->act();
					FrmGotoForm(PlayFormID);
					handled = true;
                }
            }
            break;
		/* Menu Item Selected */
		case menuEvent:
			switch (event->data.menu.itemID) {
				case EndMID: // stop encounter?
					EncounterEnd();
					handled = true;
					break;
				case CharActMID: // same as CharNormalBID or CharStandardBID
					if (current->getType()==CHARACTER) {
						current->act();
						FrmGotoForm(PlayFormID);
					}
					break;
				case EffectContinueMID: // same as EffectContinueBID
					if (current->getType()==EFFECT) {
						current->act();
						FrmUpdateForm(PlayFormID,frmRedrawUpdateCode);
					}
                    handled = true;
					break;
				case CharCreateEffectMID: // same as CharCreateEffectBID
					if (current->getType() == CHARACTER) {
						FrmPopupEditEffectForm((Character*)current);
					}
					handled = true;
					break;
				case CharDelayMID: // same as CharDelayBID
					current->popupDelayForm();
					handled = true;
					break;
				case TriggerMID: // same as TriggerBID
   					FrmPopupTriggerForm();
					handled = true;
					break;
				case CharRefocusMID: // same as CharRefocusBID
					if (current->getType() == CHARACTER) {
						((Character*)current)->refocus();
						FrmGotoForm(PlayFormID);
					}
					handled = true;
					break;
				case CharReadyMID: // same as CharReadyBID
					if (current->getType() == CHARACTER) {
    					((Character*)current)->ready();
    					FrmGotoForm(PlayFormID);
					}
					handled = true;
					break;
				case InterruptMID: // same as InterruptBID
					FrmPopupInterruptForm();
					handled = true;
					break;
				case DieMID: // deactivate current performer
					current->die();
					handled = true;
					break;
				case KillMID: // same as KillBID
					FrmPopupKillForm();
					handled = true;
					break;
/*				case EditMID: // display edit screen
					handled = true;
					current->popupEditForm();
					// TODO - F2 - handle deletions and changes to name
					break;
				case UndoMID:
					// TODO - F2 - undo function
					break;
				case RedoMID:
					// TODO - F2 - undo function
					break;
*/		
				case OverviewMID: // display overview screen
					FrmGotoForm(OverviewFormID);
					handled = true;
					break;
                case HowToID:
                    FrmHelp(PlayHelpString);
                    handled = true;
                    break;
                case AboutID:
                    FrmPopupForm(LogoFormID);
                    handled = true;
                    break;
			}
			break;
		case frmCloseEvent:
			{
                /* Release name memory */
                FieldType* field = (FieldType*) GetObjectPtr(FrmGetActiveForm(),NameFieldID);
				FldCompactText(field);
				FldSetTextHandle(field,NULL);
			}
			break;
    }
    return handled;
}
