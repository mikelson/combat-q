/*
 * Source code for the Roll Initiative Form.
 * This form displays all of the performers in a combat and allows
 * selected editing of them.
 *
 * v1.1 - Remove all active performers from init list when opening form,
 * at same time as setting character's initiative rolls to 10.
 */

#include "Main.h"
#include "CombatQOGC.h"
#include "Performer.h"

// The table column with performer names
#define NAME_COLUMN 0
// The table column with initiative rolls
#define ROLL_COLUMN 1
// The table column with "aware of opponents" status
#define AWARE_COLUMN 2

// Encounter Database Index of performer in top row
static Int32 iEncounterTop = 0;

// The first performer in the Encounter Database to *not* display in this form
// For Roll Init Form, the encounter database should be sorted to have active
// characters first, and then this variable is the index of the first inactive character
static Int32 iEncounterLast;

/*
 * Update form features which change from page to page: scroll arrows, table row
 * integers, and which table rows are displayed.
 * Call after scrolling or adding or deleting a row.
 */
static void UpdateTablePage(FormType* form, Boolean draw) {
	UInt16 nEncounter = iEncounterLast;

	// Update scroll arrows
	UInt16 iUp, iDown;
	iUp = FrmGetObjectIndex(form,ScrollUpID);
	ErrFatalDisplayIf(iUp==frmInvalidObjectId,"Can't find up object id.");
	iDown = FrmGetObjectIndex(form,ScrollDownID);
	ErrFatalDisplayIf(iDown==frmInvalidObjectId,"Can't find down object id.");
	FrmUpdateScrollers(form,
		iUp,
		iDown,
		iEncounterTop > 0,
		iEncounterTop + ROLL_ROWS < nEncounter
	);

	// Display rows with performers
	TableType* table = (TableType*) GetObjectPtr(form, TableID );
	for (Int32 row=0; row < ROLL_ROWS; row++) {
		if (iEncounterTop + row < nEncounter) {
			Character* c = (Character*) queryPerformerP(iEncounterTop + row);
			if (c==NULL) {
				// hide row
				TblSetRowUsable(table, row, false);
			} else {
    			// (re)allocate pointer for name string and fill, as necessary
    			//c->nameCopy( &nameP[row] );
    			//TblSetItemPtr(table, row, NAME_COLUMN, nameP[row]);
    
    			// initialize initiative result
    			TblSetItemInt(table, row, ROLL_COLUMN, c->getInitiative());
    
    			// initialize aware value
    			Boolean aware = c->isAware();
    			TblSetItemInt(table, row, AWARE_COLUMN, aware);
    
    			// show row
    			TblSetRowUsable(table, row, true);
			}
		} else {
			// hide row
			TblSetRowUsable(table, row, false);
		}
	}

	if (draw) {
    	// erase table and start from clean slate
    	RectangleType rP;
    	TblGetBounds (table, &rP);
    	WinEraseRectangle(&rP, 0);
    
		TblDrawTable( table );
	}
}

/*
 * Application definition of TableLoadDataFuncType.
 * Called when a name field is drawn.
 */
static Err LoadNameCallback (void* tableP, Int16 row, Int16 column, Boolean editable,
							  MemHandle* dataH, Int16* dataOffset, Int16* dataSize, 
							  FieldPtr fld) {
	Err e = errNone;
	Performer* perfP;
	perfP = queryPerformerP(iEncounterTop + row);
	if (perfP != NULL) {
		*dataH = perfP->getNameHandle(dataOffset, dataSize);
	} else {
		e = 1;
		*dataH = NULL;
		dataOffset = 0;
		dataSize = 0;
	}
	// adjust properties of the field
	FieldAttrType attr;
	FldGetAttributes(fld, &attr);
	attr.editable = 0;
	attr.underlined = 0;
	attr.justification = rightAlign;
	FldSetAttributes(fld, &attr);

	FldSetMaxChars(fld, MAX_NAME_BYTES);
	return e;
}

/* 
 * Process input-event to form.
 */
Boolean EventHandlerRollInitForm(EventPtr event)
{
    Boolean handled = false;
    FormType *form;
    switch (event->eType) {
        case frmOpenEvent: // draw the form on the screen
            form = FrmGetActiveForm();
			iEncounterTop = 0;
			/* Initialize buttons and labels */
			{ /* Initialize iEncounterLast - determine how many performers to display */
				// sort active performers to lowest slots in database
				Err e = DmQuickSort(dbEncounter, ComparePerformers, 1);
				ErrFatalDisplayIf(e!=errNone,"Failure quick sorting database.");
				// find first inactive or non-character performer
				iEncounterLast = 0;
				UInt16 max = PERF_COUNT;
				Performer* p;
				for (iEncounterLast=0; iEncounterLast < max; iEncounterLast++) {
					p = queryPerformerP(iEncounterLast);
					if ( p == NULL ) continue;
					if ( p->isActive() == false) break;

					// If init list is corrupt, reset list links.
					if ( p != p->getNext() ) {
						ErrDisplay("Performer had follower before combat.");
						p->removeFromInitList();
					} else if (p != p->getPrev() ) {
						ErrDisplay("Performer had leader before combat.");
						p->removeFromInitList();
					}
				}
			}
			{ /* Initialize the Table */
				TableType* table = (TableType*) GetObjectPtr(form,TableID);
				for (Int16 row=0; row<ROLL_ROWS; row++) {
					// TODO - F2 - make this a customTableItem, a read only text display
					TblSetItemStyle(table, row, NAME_COLUMN, textTableItem);
					TblSetItemStyle(table, row, ROLL_COLUMN, numericTableItem);
					TblSetItemStyle(table, row, AWARE_COLUMN, checkboxTableItem);
					// prevent multiple-line rows
					TblSetRowStaticHeight(table, row, true);
				}
				// show all columns
				for (Int16 col=0; col<ROLL_COLS; col++) {
					TblSetColumnUsable(table, col, true);
				}
				// initialize call back functions
				TblSetLoadDataProcedure(table,NAME_COLUMN,LoadNameCallback);

				UpdateTablePage(form, false);
			}
            FrmDrawForm(form);
            handled = true;
            break;
        case ctlSelectEvent: // button pushed
            form = FrmGetActiveForm();
            switch (event->data.ctlSelect.controlID) {
				case DoneID:
					EncounterStart(iEncounterLast);
					FrmGotoForm(PlayFormID);						
					handled = true;
					break;
				case CancelID:
					FrmGotoForm(OverviewFormID);
					handled = true;
					break;
            }
            break; // end of control selected events
        case ctlRepeatEvent: // repeat (scrolling) button pushed
            form = FrmGetActiveForm();
            switch (event->data.ctlRepeat.controlID) {
                case (ScrollUpID):
					iEncounterTop -= ROLL_ROWS;
					if (iEncounterTop < 0) iEncounterTop = 0;
					UpdateTablePage(form,true);
                    break;
                case (ScrollDownID):
					iEncounterTop += ROLL_ROWS;
					UpdateTablePage(form,true);
					break;
            }
            break;
        case keyDownEvent: // hardware key, silkscreen button, or Graffiti entered
            {
				form = FrmGetActiveForm();
                UInt32 keyMask = KeyCurrentState();
                if (keyMask & keyBitPageUp) { 
					// verify that screen can be scrolled up
					if (iEncounterTop > 0) { // Same as ScrollUpID, above
						iEncounterTop -= NUM_ROWS;
						if (iEncounterTop < 0) iEncounterTop = 0;
						UpdateTablePage(form,true);
					}
					handled = true;
                }
                else if (keyMask & keyBitPageDown) {
					// verify that screen can be scrolled down...
					if (iEncounterTop + NUM_ROWS < PERF_COUNT) { // Same as ScrollDownID, above
						iEncounterTop += NUM_ROWS;
						UpdateTablePage(form,true);
					}
					handled = true;
                }
            }
            break;
		case tblSelectEvent: // pen lifted in same field it touched down in
			switch (event->data.tblSelect.column) {
				case NAME_COLUMN:
				case ROLL_COLUMN:
					TblUnhighlightSelection(event->data.tblSelect.pTable);
					{ // Get a d20 roll for this character's initiative
 						Int16 result = PickNumber(D20ResultStringID);
    					if ( result >= 0 ){// user pressed a number button
							// get the character in this row
    						Character* c = (Character*) queryPerformerP(
    							iEncounterTop + event->data.tblSelect.row);
							if (c!=NULL) {
    							// calculate the die roll and set it
        						c->setInitiativeRoll( result );
    							// Just redraw this row.
    							TblSetItemInt(event->data.tblSelect.pTable, 
    								event->data.tblSelect.row, 
    								ROLL_COLUMN, 
    								c->getInitiative() );
    							TblMarkRowInvalid(event->data.tblSelect.pTable,
    								event->data.tblSelect.row);
    							TblRedrawTable(event->data.tblSelect.pTable);
							}
    					}
					}
					handled = true;
					break;
				case AWARE_COLUMN:
					{
						Int16 value = TblGetItemInt( 
							event->data.tblSelect.pTable,
							event->data.tblSelect.row, 
							event->data.tblSelect.column);
						Character* c = (Character*) queryPerformerP(
							iEncounterTop + event->data.tblSelect.row);
						if (c != NULL)
							c->setAware( value? true : false );
					}
					TblUnhighlightSelection(event->data.tblSelect.pTable);
					handled = true;
					break;
				default:
					break;
			}
			break;
        default:
            break;
    }
    return handled;
} // end of function EventHandlerInitRollForm
