/*
 * Source code for the Overview Form, aka "the Dungeon Master Screen".
 * This form displays all of the performers in an encounter and allows
 * arbitrary editing of them, as well as switching to new encounters.
 */

#include "Main.h"
#include "CombatQOGC.h"
#include "Performer.h"
#include <WinGlue.h> // for WinGlueDrawChar

// The table column that indicates the current performer, if any
#define CURRENT_COLUMN 0
// The table column with "active" checkbox
#define ACTIVE_COLUMN 1
// The table column with initiative values
#define INIT_COLUMN 2
// Memory for initiative fields
static MemHandle initHandle[NUM_ROWS];
// The table column with performer names
#define NAME_COLUMN 3

// Encounter Database Index of performer in top row
static Int32 iEncounterTop = 0;

// Row that has been selected by entering a Field. Negative if none.
static Int16 rowSelected = -1;

/*
 * This is run when the application starts, just before the form is drawn.
 * Allocates handles for initiative column fields.
 */
void InitOverviewForm() {
	/* Allocate handles for initiative display column */
	for(UInt16 row=0; row<NUM_ROWS; row++) {
		// can't allocate size 0 handle, so make it smallest useful size
		initHandle[row] = MemHandleNew(TxtGlueCharSize(chrNull));
		ErrFatalDisplayIf(initHandle[row]==0,
			"could not allocate handle for initiative display");
	}
}

/*
 * This runs when the application stops, after all forms are clsoed.
 * This cannot be done when Overview form is closed, because the fields
 * are loaded again after a frmCloseEvent is received.
 */ 
void StopOverviewForm() {
	/* Free handles allocated for display.*/
	for(UInt16 row=0; row<NUM_ROWS; row++) {
		Err e = MemHandleFree(initHandle[row]);
		ErrFatalDisplayIf(e!=errNone,"could not free handle for initiative display");
	}
}

/* It just so happens that the form-update code is UInt16, just like database index. */
UInt16 swapUpdateCodeForMaxRecordIndex(UInt16 in) {
    if (in == frmRedrawUpdateCode) {
    	// index happens to coincide with the redraw code
    	return dmMaxRecordIndex;
    } else if (in == dmMaxRecordIndex) {
    	// index is out of range
    	return frmRedrawUpdateCode;
    } else {
		// normal index - no change
		return in;
	}
}

/* If returning to Overview Form, tell it to display this performer. */
void OverviewFormUpdate(Performer* p) {
	UInt16 formID = FrmGetActiveFormID();
    if ( (formID == OverviewFormID) | (formID == EditEffectFormID) ) {
        UInt16 iEnc = (p==NULL)? dmMaxRecordIndex : p->getIEncounter();
        // remap extreme values of update-code/database-index
		iEnc = swapUpdateCodeForMaxRecordIndex(iEnc);
        FrmUpdateForm(formID,iEnc);
    }
}

/*
 * Update form features which change from page to page: scroll arrows, table row
 * integers, and which table rows are displayed.
 * Call after scrolling or adding or deleting a row.
 * Set draw=true to call TblDrawTable. Always OK except when redundant with FrmDrawForm?
 */
static void UpdateTablePage(FormType* form, Boolean draw) {
	Performer* p;
	// Verify that iEncounterTop is not too big
	UInt16 nEncounter = PERF_COUNT;
	while ( (iEncounterTop>=nEncounter) && (iEncounterTop>=NUM_ROWS) ) {
		iEncounterTop -= NUM_ROWS;
	}
	
	TableType* table = (TableType*) GetObjectPtr(form, TableID );

	// Unselect table cell. This can cause redraw, which uses iEncounterTop
	TblUnhighlightSelection(table);
	// Unfocus field, if any. This can also cause redraws.
	// setting no focus can also cause calls to table-field save callbacks
	if (draw) FrmSetFocus(form,noFocus);

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
		iEncounterTop + NUM_ROWS < nEncounter
	);

	// Display rows with performers
	for (Int32 row=0; row < NUM_ROWS; row++) {
		if (iEncounterTop + row < nEncounter) {
			// initialize "active" checkbox
			p = queryPerformerP(iEncounterTop + row);
			if (p!=NULL) /*{*/
				TblSetItemInt(table, row, ACTIVE_COLUMN,p->isActive());
    			// show row
    			TblSetRowUsable(table, row, true);
    			TblSetRowSelectable(table, row, true);
			/*} else {
				// hide this row if performer not found during set up
    			TblSetRowUsable(table, row, false);
    			TblSetRowSelectable(table, row, false);
			}*/
		} else {
			// hide row
			TblSetRowUsable(table, row, false);
			TblSetRowSelectable(table, row, false);
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
 * Custom Drawing Procedure for the column that indicates which Performer 
 * is acting.
 *
 * Application definition of TableDrawItemFuncType.
 */
static void DrawCurrentCallback (void *tableP, Int16 row, Int16 column, 
								  RectangleType *bounds) {
	WChar c;
	FontID curFont;

	Performer* p = queryPerformerP(iEncounterTop + row);
	if (p==NULL) return;
	Boolean isChar = p->getType() == CHARACTER;

	if ( current == p ) {
		// draw an arrow
		curFont = FntSetFont(symbolFont);
		c = symbolRightArrow;
	} else if ( isChar && ((Character*)p)->isReadied() ) {
		// draw an "R"
		curFont = FntSetFont(boldFont);
		c = chrCapital_R;
	} else if ( isChar && ((Character*)p)->isDelaying() ) {
		// draw a "D"
		curFont = FntSetFont(boldFont);
		c = chrCapital_D;
	} else {
		// draw a blank
		curFont = FntSetFont(stdFont);
		c = chrSpace;
	}
	WinGlueDrawChar(c, bounds->topLeft.x + 1, bounds->topLeft.y);
	FntSetFont(curFont);
}

/*
 * Application definition of TableLoadDataFuncType.
 * Called when an initiative field is drawn.
 */
static Err LoadInitCallback (void* tableP, Int16 row, Int16 column, Boolean editable,
							  MemHandle* dataH, Int16* dataOffset, Int16* dataSize, 
							  FieldPtr fld) {

	Err e;
	Char initS[maxStrIToALen];
	// get initiative and convert to a string
	Performer* perfP = queryPerformerP(iEncounterTop + row);
	if (perfP==NULL) {
		// display blank
		TxtGlueSetNextChar(initS,0,chrNull);
//	} else if ( (round>=0) && perfP->isActive() ) { // Active performer
	} else {
		Int16 init = perfP->getInitiative();
		StrIToA(initS, (Int32) init);
	}
	/*	
	} else { // display a "-" when not active or no combat
		UInt16 j = TxtGlueSetNextChar(initS,0,chrHyphenMinus);
		TxtGlueSetNextChar(initS,j,chrNull);
	} */
	// resize the (pre-allocated) display memory for the new string
	e = MemHandleResize(initHandle[row],StrSize(initS));
	if (e!=errNone) {
		ErrDisplay("Could not resize init display handle.");
		return 1;
	}
	// copy the new string to the display
	Char* dispS = (Char*) MemHandleLock(initHandle[row]);
	StrCopy(dispS,initS);
	MemHandleUnlock(initHandle[row]);

	*dataH = initHandle[row];
	*dataOffset = 0;
	*dataSize = MemHandleSize(*dataH);

	// adjust properties of the field
	FieldAttrType attr;
	FldGetAttributes(fld, &attr);
	attr.autoShift = 1;
	attr.singleLine = 1; // prevent new-lines in the middle of initiatives
	FldSetAttributes(fld, &attr);
	FldSetMaxChars(fld, MAX_NUMERIC_BYTES);

	return errNone;
}

/*
 * Application definition of TableSaveDataFuncType.
 * Called when user is done editing an initiative field.
 * Return true if the table should be redrawn.
 */
#define saveInitExit \
TblMarkRowInvalid(table,row); \
FldSetTextHandle(field,NULL); \
return true;

static Boolean SaveInitCallback (void *tableP, Int16 row, Int16 column) {
	TableType* table = (TableType*) tableP;
	FieldType* field = TblGetCurrentField(table);

	// see if anything changed...
	if ( FldDirty(field) ) {
		Char* s = FldGetTextPtr(field);
		if ( StrIsI(s) == false ) { // non-numeric data entered
			saveInitExit
		}
		Performer* p = queryPerformerP(iEncounterTop + row);
		if (p==NULL) {
			saveInitExit
		}
		/* Change initiative */
		Int16 initNew = StrAToI(s);
		if (initNew == p->getInitiative()) { // no change, do nothing
			return true;
		}
		if ( p == p->getNext() ) { // only performer in init list, or no combat
			// just change its initiative
			p->setInitiative( initNew );
			if ( p == current ) {
	            // update initiative count display
				FldSetI(FrmGetActiveForm(),InitiativeFieldID,(Int32)initNew);
			}
		} else {
			/* Assume a change in order can occur */
			// remember who is currently going
			Performer* oldCurrent = current;
			// remove perf from init list
    		p->deactivate();
    		// assign new init
    		p->setInitiative( initNew );
    		// restore perf to init list
    		p->activate();
    		if (oldCurrent == p) {
    			// restore perf as current, if it was before
				current = p;
				// update initiative count display
				FldSetI(FrmGetActiveForm(),InitiativeFieldID,(Int32)initNew);
			}
		}
	}

	// make the field let go of the memory, so only the global array has it
	FldSetTextHandle(field,NULL);

	TblMarkRowInvalid(table,row);// this cleans up stray pixels from highlighting
	return true;
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
	attr.autoShift = 1;
	attr.singleLine = 1; // prevent new-lines in the middle of names!
	FldSetAttributes(fld, &attr);
	FldSetMaxChars(fld, MAX_NAME_BYTES);
	return e;
}

/*
 * Application definition of TableSaveDataFuncType.
 * Called when user is done editing a name field.
 * Return true if the table should be redrawn.
 */
static Boolean SaveNameCallback (void *tableP, Int16 row, Int16 column) {
	TableType* table = (TableType*) tableP;
	FieldType* field = TblGetCurrentField(table);

	// see if anything changed...
	if ( FldDirty(field) ) {
		// get rid of unused memory, in case field shrank
		FldCompactText(field);
		// resize the field
		Char* textP = FldGetTextPtr(field);
		UInt16 newSize = StrSize(textP);
		// get the performer for this field
		Performer* perfP = queryPerformerP(iEncounterTop + row);
		if (perfP != NULL) {
			perfP->setNameSize(newSize);
		}
	}

	// make the field let go of the memory, so only the database has it
	FldSetTextHandle(field,NULL);

	TblMarkRowInvalid(table,row); // this cleans up stray pixels from highlighting
	return true;
}

/* Verify that at least one performer is active before going to Roll Initiative Form. */
static void VerifyGotoRollInit(void) {
    UInt16 nEncounter = PERF_COUNT;
    Boolean quorum = false;
    for(UInt16 i=0; i<nEncounter; i++) {
    	Performer* p = queryPerformerP(i);
    	if ( (p!=NULL) && p->isActive()) {
    		quorum = true;
    		break;
    	}
    }
    if (quorum == false) {
    	FrmAlert(NoCombatantsErrorID);
    } else {
    	FrmGotoForm(RollInitFormID);
    }
}
/* 
 * Process input-event to form.
 */
Boolean EventHandlerOverviewForm(EventPtr event)
{
    Boolean handled = false;
    FormType *form;
    switch (event->eType) {
        case frmOpenEvent: // draw the form on the screen
            form = FrmGetActiveForm();
			/* Sort the database */
			{
				Err e = DmQuickSort(dbEncounter, ComparePerformers, 0);
				ErrFatalDisplayIf(e!=errNone,"Failure quick sorting database.");
			}
			/* Initialize buttons and labels */
			if (round < 0) {
				// Start/Continue button
				FrmHideObject(form,FrmGetObjectIndex(form,DoneID));
				FrmShowObject(form,FrmGetObjectIndex(form,StartCombatBID));
			} else {
				// Start/Continue button
				FrmHideObject(form,FrmGetObjectIndex(form,StartCombatBID));
				FrmShowObject(form,FrmGetObjectIndex(form,DoneID));
			}
			{ /* Initialize the Table */
				TableType* table = (TableType*) GetObjectPtr(form,TableID);
				Int16 numRows = TblGetNumberOfRows(table); // rows visible in resource
				UInt16 nEncounter = PERF_COUNT;
				for (Int16 row=0; row<numRows; row++) {
					// Current Performer indicator
					TblSetItemStyle(table, row, CURRENT_COLUMN, customTableItem);
					// Active Flag
					TblSetItemStyle(table, row, ACTIVE_COLUMN, checkboxTableItem);
					// Initiative Indicator
					TblSetItemStyle(table, row, INIT_COLUMN, textTableItem);
					// Name Indicator
					TblSetItemStyle(table, row, NAME_COLUMN, textTableItem);
					// prevent multiple-line rows
					TblSetRowStaticHeight(table, row, true);
				}
				// show all columns
				for (Int16 col=0; col<NUM_COLUMNS; col++) {
					TblSetColumnUsable(table, col, true);
				}
				// initialize call back functions
				TblSetCustomDrawProcedure(table,CURRENT_COLUMN,DrawCurrentCallback);
				TblSetLoadDataProcedure(table,INIT_COLUMN,LoadInitCallback);
				TblSetSaveDataProcedure(table,INIT_COLUMN,SaveInitCallback);
				TblSetLoadDataProcedure(table,NAME_COLUMN,LoadNameCallback);
				TblSetSaveDataProcedure(table,NAME_COLUMN,SaveNameCallback);

				// sort encounter database
				Err e = DmQuickSort(dbEncounter, ComparePerformers, 0);
				ErrFatalDisplayIf(e!=errNone,"Failure quick sorting database.");

				UpdateTablePage(form, false);
			}
            FrmDrawForm(form);
			/* Initialize fields - must be done after drawing form */
			if (round < 0) {
				// Round # display
				FldSet(form,RoundFieldID,"-");
				// Initiative counter display
				FldSet(form,InitiativeFieldID,"-");
			} else {
				// Round # display
				FldSetI(form,RoundFieldID,(Int32)round);
				// Initiative counter display
				FldSetI(form,InitiativeFieldID,(Int32)current->getInitiative() );
			}
			// clear selected row variable
			rowSelected = -1;
            handled = true;
            break;
			/* End of case frmOpenEvent ************************************************/
		case frmUpdateEvent:
			form = FrmGetActiveForm();
			if (event->data.frmUpdate.updateCode != frmRedrawUpdateCode) {
				// page to indicated performer
				UInt16 iEnc = event->data.frmUpdate.updateCode;
               	// remap extreme values of update-code/database-index
				iEnc = swapUpdateCodeForMaxRecordIndex( iEnc );
				// remember this performer's object
				Performer* p = queryPerformerP(iEnc);

				/* Reorder database/table */
				// sort encounter database
				Err e = DmQuickSort(dbEncounter, ComparePerformers, 0);
				ErrFatalDisplayIf(e!=errNone,"Failure quick sorting database.");

				// refresh index, because sorting may have changed it
    			iEnc = (p==NULL)? 0 : p->getIEncounter();
    			// display the table page with this character
    			iEncounterTop = 0;
    			while (iEnc >= iEncounterTop + NUM_ROWS) {
    				iEncounterTop += NUM_ROWS;
    			}
				
				// select indicated performer
				rowSelected = iEnc - iEncounterTop;
			} else {
				// clear selected row variable
				rowSelected = -1;
			}

			// redisplay the table
			UpdateTablePage(form,true);
			break;
			/* End of case frmUpdateEvent **********************************************/
        case menuEvent:
            switch (event->data.menu.itemID) {
				case NewCharID: // Create New Character
            		/* If user was editing field, then release focus on it,
            		   so that field may be saved and validated. */
            		FrmSetFocus(FrmGetActiveForm(),noFocus);
					FrmPopupEditCharForm(NULL);
					handled = true;
					break;
				case DeleteID: // Remove selected character from encounter database
					form = FrmGetActiveForm();
					FrmSetFocus(form,noFocus); // to save field, in case delete fails
					{
						TableType* table = (TableType*) GetObjectPtr(form,TableID);
						Int16 row, col;
						// True if pen entered CURRENT_COLUMN
						Boolean selected = TblGetSelection(table,&row,&col);
						if ( (selected==false) && (rowSelected>=0) ) {
							// in case pen selected other column instead
							selected = true;
							row = rowSelected;
						}
						if (selected) {
							Performer* p = queryPerformerP(iEncounterTop + row);
							if ( (p!=NULL) && p->deactivate() ) { // Fails if last and user declines to end combat
								// Unselect this row now, so unselecting when its gone won't cause error
								TblUnhighlightSelection(table);
                            	// Unfocus field, if any, to release it.
                            	FrmSetFocus(form,noFocus);
								// clear selected row variable
								rowSelected = -1;
								delete p;
								p = NULL;
    							// update display
								//OverviewFormUpdate(NULL);
    							UpdateTablePage(form,true);
							} // end of deactivate()==true conditional
						} else { // Nothing in the table was selected
							FrmAlert(NoneSelectedID);
						}
					}
					handled = true;
					break;
				case SortTableID:
					{
						form = FrmGetActiveForm();
						FrmSetFocus(form,noFocus);
						// order of rows may change, so clear selected row
						rowSelected = -1;
						// sort encounter database
						Err e = DmQuickSort(dbEncounter, ComparePerformers, 0);
						ErrFatalDisplayIf(e!=errNone,"Failure quick sorting database.");
						UpdateTablePage(form,true);
					}
					handled = true;
					break;
				case CopyItemID:
					{
						form = FrmGetActiveForm();
						FrmSetFocus(form,noFocus);
						/* Get Selected Row, if any. */
						TableType* table = (TableType*) GetObjectPtr(form,TableID);
						Int16 row, col;
						// True if pen entered CURRENT_COLUMN
						Boolean selected = TblGetSelection(table,&row,&col);
						if ( (selected==false) && (rowSelected>=0) ) {
							// in case pen selected other column instead
							selected = true;
							row = rowSelected;
						}
						if (selected) {
							/* Display form for choosing a number */
							Int16 num = PickNumber(NCopyStringID);

							if (num >= 0) {
								Performer* p = queryPerformerP(iEncounterTop + row);
								if (p!=NULL) {
									for (Int16 i=1; i<=num; i++) {
										p->copy(i);
									}
									UpdateTablePage(form,true);
								}
							}
						} else { // Nothing in the table was selected
							FrmAlert(NoneSelectedID);
						}
					}
					handled = true;
					break;
// TODO - F2 - Add "make selected performer current" option
				case StartCombatID:
					if (round < 0) { // no combat in progress
						VerifyGotoRollInit();
					} else { // combat in progress - resuming
						FrmGotoForm(PlayFormID);
					}
					handled = true;
					break;
				case EndCombatID:
					EncounterEnd();
					handled = true;
					break;
// TODO - F2 - Add Beam/Save/Load/New Encounter options
                case HowToID:
                    FrmHelp(OverviewHelpString);
                    handled = true;
                    break;
                case AboutID:
                    FrmPopupForm(LogoFormID);
                    handled = true;
                    break;
                case OGLID:
					FrmPopupTextForm(StringOGLTitleID,StringOGLID);
                    handled = true;
                    break;
            }
            break;
			/* End of case menuEvent **********************************************/
        case ctlSelectEvent: // button pushed
            form = FrmGetActiveForm();
            switch (event->data.ctlSelect.controlID) {
				case NewCharBID:
					FrmPopupEditCharForm(NULL);
					handled = true;
					break;
				case DetailBID:
					/* Edit details of selected performer. */
					form = FrmGetActiveForm();
					{
						TableType* table = (TableType*) GetObjectPtr(form,TableID);
						Int16 row, col;
						// True if pen entered CURRENT_COLUMN
						Boolean selected = TblGetSelection(table,&row,&col);
						if ( (selected==false) && (rowSelected>=0) ) {
							// in case pen selected other column instead
							selected = true;
							row = rowSelected;
						}
						if (selected) {
							Performer* p = queryPerformerP(iEncounterTop + row);
							if (p!=NULL) p->popupEditForm();
						} else { // Nothing in the table was selected
							FrmAlert(NoneSelectedID);
						}
					}
					handled = true;
					break;
				case StartCombatBID:
					VerifyGotoRollInit();
					handled = true;
					break;
				case DoneID:
					FrmGotoForm(PlayFormID);
					handled = true;
					break;
            }
            break;
			/* End of case ctlSelectEvent **********************************************/
        case ctlRepeatEvent: // repeat (scrolling) button pushed
            form = FrmGetActiveForm();
            switch (event->data.ctlRepeat.controlID) {
                case (ScrollUpID):
					iEncounterTop -= NUM_ROWS;
					if (iEncounterTop < 0) iEncounterTop = 0;
					UpdateTablePage(form,true);
					rowSelected = -1;
                    break;
                case (ScrollDownID):
					iEncounterTop += NUM_ROWS;
					UpdateTablePage(form,true);
					rowSelected = -1;
					break;
            }
            break;
			/* End of case ctlRepeatEvent **********************************************/
        case keyDownEvent: // hardware key, silkscreen button, or Graffiti entered
            {
				form = FrmGetActiveForm();
                UInt32 keyMask = KeyCurrentState();
                if (keyMask & keyBitPageUp) { 
					// verify that screen can be scrolled up
					if (iEncounterTop > 0) { // Same as ScrollUpID, above
						FrmSetFocus(form,noFocus);  // allows saving of any edited field
						iEncounterTop -= NUM_ROWS;
						if (iEncounterTop < 0) iEncounterTop = 0;
						UpdateTablePage(form,true);
						rowSelected = -1;
					}
					handled = true;
                }
                else if (keyMask & keyBitPageDown) {
					// verify that screen can be scrolled down...
					if (iEncounterTop + NUM_ROWS < PERF_COUNT) { // Same as ScrollDownID, above
						FrmSetFocus(form,noFocus);  // allows saving of any edited field
						iEncounterTop += NUM_ROWS;
						UpdateTablePage(form,true);
						rowSelected = -1;
					}
					handled = true;
                }
            }

            break;
		case tblEnterEvent: // pen set down in table
			switch (event->data.tblEnter.column) {
				case INIT_COLUMN:
				case NAME_COLUMN:
					rowSelected = event->data.tblEnter.row;
					break;
				default:
					rowSelected = -1;
					break;
			}
			break;
		case tblSelectEvent: // pen lifted in same field it touched down in
			switch (event->data.tblSelect.column) {
				case ACTIVE_COLUMN:
					{
						Int16 value = TblGetItemInt( 
							event->data.tblSelect.pTable,
							event->data.tblSelect.row, 
							event->data.tblSelect.column);
						Performer* p = queryPerformerP(
							iEncounterTop + event->data.tblSelect.row);
						if (p!=NULL) {
    						if (value == 0) {
    							if ( p->deactivate() == false ) { //deactivate failed - reset control
    								TblSetItemInt(
            							event->data.tblSelect.pTable,
            							event->data.tblSelect.row, 
            							event->data.tblSelect.column, 1);
    							}
    						} else {
    							p->activate();
    						}
    						// update row
            				TblMarkRowInvalid(
                							event->data.tblSelect.pTable,
                							event->data.tblSelect.row);
            				TblRedrawTable(event->data.tblSelect.pTable);
						}
					}
					handled = true;
					break;
				default:
					break;
			}
			break;
			/* End of case tblSelectEvent **********************************************/
        default:
            break;
    }
    return handled;
} // end of function EventHandlerOverviewForm
