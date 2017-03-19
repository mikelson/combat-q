// Copyright 2017 Peter L Mikelsons

#include "Main.h"

UInt16 textFormTitle;
UInt16 textFormStringID;

void FrmGotoTextForm(UInt16 title, UInt16 stringID) {
	textFormTitle = title;
	textFormStringID = stringID;
	FrmGotoForm(TextFormID);
}

void FrmPopupTextForm(UInt16 title, UInt16 stringID) {
	textFormTitle = title;
	textFormStringID = stringID;
    FrmPopupForm(TextFormID);
}

/*
 * Scroll the given field on the given form by one page in the given direction (winUp or winDown).
 * Enables/disables the given forward and backward buttons, as needed. 
 */
static void FldPageScroll(FormType* form, UInt16 fieldID, WinDirectionType dir) {
    FieldType* field = GetObjectPtr(form,fieldID);
    FldScrollField(field, FldGetVisibleLines(field), dir);
    
	// Update scroll arrows
	FrmUpdateScrollers(form,
		FrmGetObjectIndex(form,ScrollUpID), 
		FrmGetObjectIndex(form,ScrollDownID),
		FldScrollable(field,winUp), FldScrollable(field,winDown)
	);
}

/* 
 * Process input-event to generic scrolling text form.
 */
Boolean EventHandlerTextForm(EventPtr event)
{
    Boolean handled = false;
    FormType* form = FrmGetActiveForm();
	MemHandle handle;
    switch (event->eType) {
        case frmOpenEvent:
            FrmDrawForm(form);
			{ // initialize the title
				MemHandle handle = DmGetResource('tSTR',textFormTitle);
				Char* s = MemHandleLock(handle);
				FrmCopyTitle(form,s);
				MemHandleUnlock(handle);
			}
            { // initialize the field
                FieldType* field = GetObjectPtr(form,TextFormFieldID);
                // look for the OGL string resource
                MemHandle handle = DmGetResource('tSTR',textFormStringID);
                ErrFatalDisplayIf(handle==NULL,"Failed to find string for text form.");
                // detach old handle, if any, from field
                FldSetTextHandle(field,NULL);
                // attach the resource to the field
                FldSetTextHandle(field,handle);
                FldDrawField(field);
            }
			// initialize scroll buttons
			FldPageScroll(form,TextFormFieldID,winUp);
            handled = true;
            break;
        case ctlSelectEvent: // button pushed
            switch (event->data.ctlSelect.controlID) {
                case (DoneID): 
                    // free the database string's handle from the field
                    FldSetTextHandle(GetObjectPtr(form,TextFormFieldID),NULL);
   	                FrmReturnToForm(0);
                    handled = true;
                    break;
            }
            break;
        case ctlRepeatEvent: // repeat (scrolling) button pushed
            switch (event->data.ctlRepeat.controlID) {
                case (ScrollUpID):
                    FldPageScroll(form,TextFormFieldID,winUp);
                    break;
                case (ScrollDownID):
                    FldPageScroll(form,TextFormFieldID,winDown);
                    break;
            }
            break;
        case keyDownEvent: // hardware key, silkscreen button, or Graffiti entered
            {
                UInt32 keyMask = KeyCurrentState();
                if (keyMask & keyBitPageUp) {
                    FldPageScroll(form,TextFormFieldID,winUp);
                }
                else if (keyMask & keyBitPageDown) {
                    FldPageScroll(form,TextFormFieldID,winDown);
                }
            }
            handled = true;
            break;
        case frmCloseEvent:
            // free the database string's handle from the field
            FldSetTextHandle(GetObjectPtr(form,TextFormFieldID),NULL);
            break;
    }
    return handled;
}

