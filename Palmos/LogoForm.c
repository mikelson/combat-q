#include "Main.h"

/* 
 * Process input-event to D20 Logo display form.
 */
Boolean EventHandlerLogoForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form;
	
    switch (event->eType) {
        case frmOpenEvent:
			form = FrmGetActiveForm();
			{
				// Display version number
				FieldType* field = (FieldType*) GetObjectPtr(form,LogoFormVersionID); 
				FldSetTextHandle(field,NULL);
				FldSetTextHandle(field, DmGetResource (verRsc, VersionID) );		
            	FrmDrawForm(form);
				FldDrawField(field);		
			}
            handled = true;
            break;
        case ctlSelectEvent: // button pushed
            switch (event->data.ctlSelect.controlID) {
                case (DoneID): 
					// Release version number resource from field
					FldSetTextHandle(
						(FieldType*) GetObjectPtr(
							FrmGetActiveForm(), 
							LogoFormVersionID
						),
						NULL
					);
					FrmGotoTextForm(StringAboutTitleID, StringAboutID);
                    handled = true;
                    break;
            }
            break;
		case frmCloseEvent: // form closed
        	// Release version number resource from field
        	FldSetTextHandle(
        		(FieldType*) GetObjectPtr(
        			FrmGetActiveForm(), 
        			LogoFormVersionID
        		),
        		NULL
        	);
			break;
    }
    return handled;
}
