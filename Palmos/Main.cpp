/***********************************************************************
 *
 * PROJECT:  Combat-Q for Palm OS
 * FILE:     main.c
 * AUTHOR:   Peter Mikelsons
 *
 * DESCRIPTION:  Event loop code.
 *
 * Copyright 2017 Peter L. Mikelsons.  All rights reserved.
 *
 ***********************************************************************/
#include "Main.h" // all modules must include this first
#include "CombatQOGC.h" // C++ modules may include this
#include "Performer.h" // C++ modules may include this, if they have CombatQOGC.h

/* database variables */
#define DB_CARD 0 // card number is currently always zero
#define DB_TYPE 'DATA'
/* Making this 'appl' makes it appear to Launcher's beam utility. Should set dmHdrAttrHidden.*/
/* Note, from CDK: "...the Windows Backup conduit performs an additional check, not backing up databases also if they have an associated conduit and are of type 'data'." */
#define DB_ID 'PMcd'
/* Note: If DB_ID is different from CREATOR_ID,
then database will have separate entry in Launcher
utilities of Delete and Info, but not Beam (unless DB_TYPE is 'appl'). */
DmOpenRef dbEncounter = 0;

/* Name of default encounter database */
#define DEFAULT_DB_NAME "Combat-Q Database"

/* The version number of the saved preferences, should be Int16 */
#define PREFERENCES_VERSION 1
/* ID number of saved preferences resource for misc preferences, should be UInt16 */
#define PREFS_ID_MISC 0
/* ID number of saved preferences resource for database name, should be UInt16. */
#define PREFS_ID_DB_NAME 1

// Is this the first time this application has been launched?
Boolean firstExecution = false;

/* The number appended to generic character names. */
UInt16 counterCharacterName = 1;

// Struct for miscellaneous values saved when application exits
typedef struct {
} SavedPreferences;

/*
 * Let user pick from list of numbers on pop-up screen, and return result
 * Arg stringID -> ID number of string resource to display on screen
 * Returns < 0 if "Cancel" selected.
 */
Int16 PickNumber(UInt16 stringID) {
	// quick verification of button definitions
    ErrNonFatalDisplayIf(Roll20ButtonID - Roll1ButtonID != 19,
    	"Pick Number Button ID definitions may be out of order!");
	// set up the form
	FormType* form = FrmInitForm(PickNumberID);
    // get handle for string resource
    MemHandle h = DmGetResource('tSTR',stringID);
    ErrFatalDisplayIf(h==NULL,"Failed to find text string resource.");
    // get pointer to field
    FieldType* field = (FieldType*)GetObjectPtr(form,PickFieldID);
    // detach old handle, if any, from field
    FldSetTextHandle(field,NULL);
    // attach the resource to the field
    FldSetTextHandle(field,h);
	// get button id from form
	Int16 result = FrmDoDialog(form);
    FldSetTextHandle(field,NULL);
	FrmDeleteForm(form);
	// convert button ID to result
	if ( (result >= Roll1ButtonID) && 
		 (result <= Roll20ButtonID) ){// user pressed a number button
		result += -Roll1ButtonID + 1 ;
	} else {
		result = -1;
	}
	return result;
}

/*
 * Return the number of characters in a string.
 * Differs from strlen only for multi-byte characters.
 */
UInt16 TxtGetNumChar(const char* string) {
	UInt16 pos = 0;
	UInt16 len = StrLen( string );
	UInt16 result = 0;
	while( pos < len ) {
		pos += TxtGlueGetNextChar(string,pos,NULL);
		result++;
	}
	return result;
}

/*
 * Determine if a string is a valid integer or not.
 * Returns true if all character in string are digits.
 * First character may be a "-", or chrHyphenMinus, ASCII 0x2D.
 */
Boolean StrIsI(Char* string) {
	if (string==NULL) return false;
	UInt16 i = 0;
	UInt16 bufferLength = StrLen(string);
	WChar ch;
	i += TxtGlueGetNextChar(string, i, &ch);
	if ( (ch!=chrHyphenMinus) && (TxtGlueCharIsDigit(ch)==false) ) {
		return false;
	}
	while (	i < bufferLength ) {
		i += TxtGlueGetNextChar(string, i, &ch);
		if ( TxtGlueCharIsDigit(ch) == false ) {
			return false;
		}
	}
	return true;
}

/*
 * Sets the given field to the given text.
 * Assigns memory if needed. Draws the field, so can only be called after form is drawn. 
 */
void FldSet(FormType* form, UInt16 FieldID, Char* text) {
	// get and verify pointer to field
	FieldType* field = (FieldType*) GetObjectPtr(form,FieldID);
	if (field==NULL) return;
	// get old text from field, if any
    MemHandle handle = FldGetTextHandle(field);
    // detach field from old text, if any
    FldSetTextHandle(field,NULL);
	UInt16 textSize = StrSize(text);
    if (handle == 0) { // field does not yet have memory assigned: do so now
        handle = MemHandleNew(textSize);
    } else { // resize field's memory
		Err e = MemHandleResize(handle, textSize);
		if (e!=errNone) {
			ErrDisplay("Could not resize memory for field.");
			return;
		}
	}
    if (handle != 0) { // memory successfully acquired or allocated
        Char* s;
        // put given text in the field's memory
        s = (Char*) MemHandleLock(handle);
        if ( (s!=NULL) && (text!=NULL) ) StrCopy(s,text);
        MemHandleUnlock(handle);
        // (re)attach text to field
        FldSetTextHandle(field,handle);
    }
	// Update field
	FldDrawField(field);
}

/*
 * Set a field to display the number n. Wrapper for FldSet().
 */
void FldSetI(FormType* form, UInt16 FieldID, Int32 n) {
	Char s[maxStrIToALen];
	StrIToA(s,n);
	FldSet(form,FieldID,s);
}

/*
 * Set a field to display the same text as string resource StringID.
 * Copies text to the field, so resource is not attached afterwards.
 * Wrapper for FldSet().
 */
void FldSetS(FormType* form, UInt16 FieldID, UInt16 StringID) {
	// look for string resource
    MemHandle handle = DmGetResource('tSTR',StringID);
	if (handle==NULL) {
	    ErrDisplay("Failed to find string resource.");
		return;
	}
	Char* s = (Char*) MemHandleLock(handle);
	FldSet(form,FieldID,s);
	MemHandleUnlock(handle);
}

/*
 * Set the editable property of a field.
 */
void FldSetEditable(FieldType* field, Boolean editable) {
	FieldAttrType attr;
	FldGetAttributes(field, &attr);
	attr.editable = editable;
	FldSetAttributes(field, &attr);
}

/* 
 * Create, Open, and set Attributes of the current database.
 *
 */
static DmOpenRef CreateDatabase(Char* dbName) {
	Err e;
	/* Create */
	// false means create non-resource database
	e = DmCreateDatabase(DB_CARD, dbName, DB_ID, DB_TYPE, false);
    ErrFatalDisplayIf(e!=0,"Could not create database.");

	/* Find */
	LocalID dbID = DmFindDatabase(DB_CARD, dbName);
	ErrFatalDisplayIf(dbID==0,"Could not find just-created database.");

	/* Open */
    DmOpenRef result = DmOpenDatabase(DB_CARD,dbID,dmModeReadWrite);
    ErrFatalDisplayIf(result==0, "Could not open just-created database.");

    /* Set Attributes */
	// get ID. other fields are open count, open mode, card #, and resource boolean
    DmOpenDatabaseInfo(result,&dbID,NULL,NULL,NULL,NULL);
    // get database's attributes
	UInt16 attr;
	UInt16 version;
	LocalID appInfoID;
    DmDatabaseInfo(DB_CARD,dbID,NULL,&attr,&version,
		NULL,NULL,NULL,NULL,&appInfoID,NULL,NULL,NULL);

    // make sure this database gets backed up during synchronization
    attr |= dmHdrAttrBackup;

	// database should be "hidden from view" - Launcher does not show, #records shown as 0
	//attr |= dmHdrAttrHidden;
	// bundle database with application when app gets beamed
	// attr |= dmHdrAttrBundle;
	// prevent database from being beamed
	// attr |= dmHdrAttrCopyPrevention;
	
	// set current version number
	version = DB_VERSION;
	
	// If there isn't an app info block, make space for one
	if (appInfoID == NULL) {
		// assign memory
		MemHandle h = DmNewHandle(result, sizeof(EncounterInfoType) );
		ErrFatalDisplayIf(h==0,"Could not assign memory for database info block.");
		// convert to local ID
		appInfoID = MemHandleToLocalID(h);
		ErrFatalDisplayIf(appInfoID==0,"Could not convert handle to local ID");
	}
	// convert to locked pointer
	EncounterInfoType* appInfoP = (EncounterInfoType*) 
		MemLocalIDToLockedPtr(appInfoID,DB_CARD);
	// set app info block to zero
	DmSet(appInfoP, 0, sizeof(EncounterInfoType), 0);
	MemPtrUnlock(appInfoP);

    // assign modified attributes to database
    DmSetDatabaseInfo(DB_CARD,dbID,NULL,&attr,&version,
		NULL,NULL,NULL,NULL,&appInfoID,NULL,NULL,NULL);

	return result;
}

/* 
 * This is called by PilotMain after normal launch.
 */
static Err StartApplication(void) {
    Err err;
    UInt32 romVersion;
    Int16 version;
	SavedPreferences saved;
	UInt16 savedSize = sizeof(saved);
    UInt16 savedSizeReq = sizeof(saved);
	Char dbName[32];
	LocalID id;

    /* Verify OS version - fail if too low */
    err = FtrGet(sysFtrCreator,sysFtrNumROMVersion, &romVersion);
    if (romVersion<sysMakeROMVersion(2, 0, 0, 0, 0)) {
        FrmAlert(AlertOldOSID);
        return sysErrRomIncompatible;
    }
    
	/* Restore previous session's settings. */
    version = PrefGetAppPreferences(CREATOR_ID,PREFS_ID_MISC, &saved, &savedSize, true);
    if (version==noPreferenceFound) { // new application or preferences lost
        // assume this is the first execution after installation...
        firstExecution = true;
    }
    // if structure of save data changes, detect it with (version==1.96) conditional here
    else if (version==PREFERENCES_VERSION) {
        if (savedSize != savedSizeReq) {
            ErrFatalDisplayIf(true,"App preferences size changed.");
            return 1;
        }
		// use saved preferences
		// (CURRENTLY NO PREFERENCES ARE SAVED)
    }
	// If PREFERENCES_VERSION changes, add code to handle old version here.

	// get size and version number of saved database name
	savedSize = 0;
	version = PrefGetAppPreferences(CREATOR_ID,PREFS_ID_DB_NAME, NULL, 
									&savedSize, true);
	if (version==noPreferenceFound) { // new application or preferences lost
		// use default value
		StrCopy(dbName,DEFAULT_DB_NAME);
	}
	else if (version==PREFERENCES_VERSION) {
		// copy the saved name to the allocated memory
		PrefGetAppPreferences(CREATOR_ID,PREFS_ID_DB_NAME, dbName, 
							  &savedSize, true);
	}
	// If PREFERENCES_VERSION changes, add code to handle old version here.

	/* Open an "Encounter" database. */
	id = DmFindDatabase(DB_CARD,dbName);
	// if that failed, try the default name
	if (id==0) {
		StrCopy(dbName,DEFAULT_DB_NAME);
		id = DmFindDatabase(DB_CARD,dbName);
	}
	if (id!=0) { // Successfully found database
		// Open database
		dbEncounter = DmOpenDatabase(DB_CARD,id,dmModeReadWrite);
		if (dbEncounter==0) {
			ErrDisplay("Could not open encounter database.");
			ErrAlert(DmGetLastErr());
		}
		// get database local ID
		LocalID dbID;
		err = DmOpenDatabaseInfo(dbEncounter, &dbID, NULL, NULL, NULL, NULL);
		ErrFatalDisplayIf(err!=errNone,"Could not get database id.");
		// get database version and application data block
		UInt16 version;
		LocalID appInfoID;
		err = DmDatabaseInfo(DB_CARD, dbID, NULL,NULL,&version,NULL,NULL,NULL,NULL, &appInfoID,
			NULL,NULL,NULL);
		ErrFatalDisplayIf(err!=errNone,"Could not get database info.");
		ErrFatalDisplayIf(appInfoID==NULL,"Database does not have app info block!");
		
	   	/* Create objects for entries in encounter database. */
		LoadPerformers(version);

		if (version == 1) {
			// if database was v1, and we got here, it must be converted by now
			// update database's version number
			version = DB_VERSION;
			err = DmSetDatabaseInfo (DB_CARD, dbID, NULL,NULL,&version,
				NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
			ErrFatalDisplayIf(err!=errNone,"Could not update database version number.");
		}
		
        /* Get info from application data block of database */
        // lock the block
        EncounterInfoType* appInfoP = 
			(EncounterInfoType*)
        		MemLocalIDToLockedPtr(appInfoID,DB_CARD);
        ErrFatalDisplayIf(appInfoP==0,"Could not get locked pointer for app info block.");
        // copy data from database "header" to globals
        round = appInfoP->round;
        counterCharacterName = appInfoP->counterCharacterName;
        first = queryPerformerPfromID(appInfoP->firstID);
        current = queryPerformerPfromID(appInfoP->currentID);
        // unlock the block
        MemPtrUnlock(appInfoP);
        
        // validate header data
        if (round < 0) {
	        first = NULL;
    	    current = NULL;
        // if initiative list is not null then it will  be cleaned up later
        } else {
        	if ( (first==NULL) || (current==NULL) )
        	// error in initiative list pointers, end combat
        	round = -1;
        }
	}
	else { // Did not find database: create new one (with default name)
		dbEncounter = CreateDatabase(dbName);
		ErrFatalDisplayIf(dbEncounter==0,"Could not create encounter database.");
	}

#ifdef TRIAL
	FrmAlert(TrialAlertID);
#endif
	// must do this once before opening Overview Form
	InitOverviewForm();
	FrmGotoForm(OverviewFormID);
    if (firstExecution) {
        FrmPopupForm(LogoFormID);
    } 
    return err;
}

/* 
 * Called when EventLoop() exits in PilotMain.
 */
static void StopApplication(void)
{
	SavedPreferences saved;
    Err e;
	Char name[32];
	LocalID dbID, appInfoID;

	/* Close Windows */
    // need this to prevent memory leaks (according to POSE)
	FrmCloseAllForms();

	// must do this once after closing forms
	StopOverviewForm(); 
	
    /* Save current settings as application preferences */

	//(CURRENTLY NO PREFERENCES (besides encounter database name) ARE SAVED)
    PrefSetAppPreferences(CREATOR_ID,PREFS_ID_MISC,PREFERENCES_VERSION,
						  &saved,sizeof(saved),true);
	// get encounter database name
	e = DmOpenDatabaseInfo(dbEncounter,&dbID,NULL,NULL,NULL,NULL);
	ErrFatalDisplayIf(e!=errNone,"Error getting encounter database info");
	e = DmDatabaseInfo (DB_CARD,dbID,&name[0],
		NULL,NULL,NULL,NULL,NULL,NULL,&appInfoID,NULL,NULL,NULL);
	// save encounter database name to preferences
    PrefSetAppPreferences(CREATOR_ID,PREFS_ID_DB_NAME,PREFERENCES_VERSION,
						  name,StrSize(name),true);

	/* Save encounter info to application data block of encounter database */
	ErrFatalDisplayIf(appInfoID==NULL,"Database does not have app info block!");
	EncounterInfoType* appInfoP = 
		(EncounterInfoType*) MemLocalIDToLockedPtr(appInfoID,DB_CARD);
	ErrFatalDisplayIf(appInfoP==0,"Could not get locked pointer for app info block.");
	EncounterInfoType appInfo;
	appInfo.round = round;
	appInfo.counterCharacterName = counterCharacterName;
	appInfo.firstID = (first==NULL)? ID_MAXIMUM : first->getID();
	appInfo.currentID = (current==NULL)? ID_MAXIMUM : current->getID();
	e = DmWrite(appInfoP, 0, &appInfo, sizeof(appInfo));
	ErrFatalDisplayIf(e!=errNone,"Problem writing application data for encounter database.");
	MemPtrUnlock(appInfoP);

	/* Free Performers in encounter database before exiting. */
	UInt16 n = PERF_COUNT;
	for (UInt16 i=0; i<n; i++) {
		Performer* p = queryPerformerP(i);
		if (p!=NULL)
			MemPtrFree(p);
		// don't do "delete p", because destructors remove performer from database!
	}
    // close encounter database
    e = DmCloseDatabase(dbEncounter);
	ErrFatalDisplayIf(e!=errNone,"Error closing encounter database.");

#ifdef TRIAL
	// delete encounter database >:-)
	dbID = DmFindDatabase(DB_CARD, name); // reuse name[] from above
	e = DmDeleteDatabase(DB_CARD, dbID);
	ErrFatalDisplayIf(e!=errNone,"Error deleteing encounter database");
#endif
}

/* 
 * Initializes form resources and sets callback event handlers for forms
 */
static Boolean ApplicationHandleEvent(EventPtr event)
{
    FormType  *form;
    UInt16    formID;
    Boolean   handled = false;
    
    if (event->eType == frmLoadEvent) {
        formID = event->data.frmLoad.formID;
        // load form resource into memory and initialize data structure and return pointer to it
        form = FrmInitForm(formID);
        // make the form the active form
        FrmSetActiveForm(form);
                
        // set up an event handler for this form
        switch (formID) {
            case OverviewFormID:
                // this sets the callback function for this form, using pointer to function
                FrmSetEventHandler(form, EventHandlerOverviewForm);
                break;
            case RollInitFormID:
                FrmSetEventHandler(form, EventHandlerRollInitForm);
                break;
            case TextFormID:
                FrmSetEventHandler(form, EventHandlerTextForm);
                break;
            case LogoFormID:
                FrmSetEventHandler(form, EventHandlerLogoForm);
				break;
            case PlayFormID:
                FrmSetEventHandler(form, EventHandlerPlayForm);
				break;
            case DelayFormID:
                FrmSetEventHandler(form, EventHandlerDelayForm);
				break;
			case SelectFormID:
				FrmSetEventHandler(form, EventHandlerSelectForm);
				break;
			case EditEffectFormID:
				FrmSetEventHandler(form, EventHandlerEditEffectForm);
				break;
			case EditCharFormID:
				FrmSetEventHandler(form, EventHandlerEditCharForm);
				break;
            default:
                break;
        }
        handled = true;
    }

    return handled;
}

/*
 * Called by PilotMain after successful StartApplication.
 * Why do we have to have it? Seems pretty automatic...
 * Don't need to make changes here
 */
static void EventLoop(void)
{
    EventType  event;
    UInt16     error;
    do {
        // second parameter is time-out value in ticks (hundredths of seconds)
        // change for real-time game?
        EvtGetEvent(&event, evtWaitForever);
        if (! SysHandleEvent(&event))
            if (! MenuHandleEvent(0, &event, &error))
                // user function - see above
                if (! ApplicationHandleEvent(&event))
                    FrmDispatchEvent(&event);
    } while (event.eType != appStopEvent);
}

/* 
 * first thing executed in an application
 * param launchCode What OS wants application to do
 * param cmdPBP Used in abnormal launch, points to data of interest
 * param launchFlags Used in abnormal launch, flags related to this launch
 */
UInt32 PilotMain(UInt16 launchCode, MemPtr cmdPBP,
                 UInt16 launchFlags)
{
    Err err;
    switch (launchCode) {
        // the normal launch code...
        case sysAppLaunchCmdNormalLaunch:
            if ((err = StartApplication()) == errNone) {
                EventLoop();
                StopApplication();
            }
            break;

        default:
            break;
    }

    return err;
}
