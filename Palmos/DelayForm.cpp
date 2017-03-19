
#include "Main.h"
#include "CombatQOGC.h"

static Character* delayer = NULL;
static Int16 initMin = 0;
static Int16 initMax = 0;

void Character::popupDelayForm(void) {
	delayer = this;
	FrmPopupForm(DelayFormID);
}

/* 
 * Process input-event to delay form dialog.
 */
Boolean EventHandlerDelayForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form;
	Char* s;
    switch (event->eType) {
        case frmOpenEvent:
			form = FrmGetActiveForm();
			initMax = delayer->getDelayInitMax();

            FrmDrawForm(form);

			// Set up field
			FldSetI(form,MaxDelayFieldID,initMax);
			FrmSetFocus(form,FrmGetObjectIndex(form,DelayInitFieldID));

            handled = true;
            break;
        case ctlSelectEvent: // button pushed
            switch (event->data.ctlSelect.controlID) {
                case (DoneID): 
					form = FrmGetActiveForm();
					{
						FieldType* field = (FieldType*) GetObjectPtr(form,DelayInitFieldID);
    					Char* fieldS = FldGetTextPtr(field);
						// determine if field is valid number
						if (StrIsI(fieldS)==true) {
    						// Assume that DelayInitFieldID is numeric only.
    						Int32 initIn = StrAToI(fieldS);
    						// make sure that initiative is in range
    						if ( initIn>initMax ) {
    							FrmAlert(InitOutOfRangeErrorID);
    						} else {
    							FrmReturnToForm(0); // close window
    							delayer->delay(initIn);
								FrmUpdateForm(PlayFormID,frmRedrawUpdateCode);
    						}
						} else { // invalid entry in field
							FrmAlert(InitOutOfRangeErrorID);
						}
					}
                    handled = true;
                    break;
                case (NoneBID): // no initiative selected... use current
                    FrmReturnToForm(0);
					delayer->delay();
					FrmUpdateForm(PlayFormID,frmRedrawUpdateCode);
                    handled = true;
                    break;
                case (CancelID): 
                    // close form
                    FrmReturnToForm(0);
                    handled = true;
                    break;
            }
            break;
    }
    return handled;
}

