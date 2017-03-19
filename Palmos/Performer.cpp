#include "Main.h"
#include "CombatQOGC.h"
#include "Performer.h"

// Define Byte - a generic 8-bit data type
typedef UInt8 Byte;

/***************************************************************************/
/* The following define how data are stored in encounter database records. */
/***************************************************************************/
/* Fields used by all performers in database. */
#define OFFSET_POINTER 0
#define SIZE_POINTER sizeof(Performer*)

#define OFFSET_PREV OFFSET_POINTER + SIZE_POINTER
#define SIZE_PREV sizeof(UInt32)

#define OFFSET_NEXT OFFSET_PREV + SIZE_PREV
#define SIZE_NEXT sizeof(UInt32)

#define OFFSET_INITIATIVE OFFSET_NEXT + SIZE_NEXT
#define SIZE_INITIATIVE sizeof(Int16)

#define OFFSET_NAMESIZE OFFSET_INITIATIVE + SIZE_INITIATIVE
#define SIZE_NAMESIZE sizeof(UInt16)

#define OFFSET_ACTIVE OFFSET_NAMESIZE + SIZE_NAMESIZE
#define SIZE_ACTIVE sizeof(Boolean)

#define OFFSET_TYPE OFFSET_ACTIVE + SIZE_ACTIVE
#define SIZE_TYPE sizeof(PerformerType)

#define OFFSET_PERFDATA OFFSET_TYPE + SIZE_TYPE

/* Character-specific fields of database records. */

#define OFFSET_DELAYING OFFSET_PERFDATA
#define SIZE_DELAYING sizeof(Boolean)
#define BIT_DELAYING 0 // pack these four booleans into one byte

#define OFFSET_READIED OFFSET_DELAYING + 0
#define SIZE_READIED sizeof(Boolean)
#define BIT_READIED 1

#define OFFSET_TAKINGREADIED OFFSET_READIED + 0
#define SIZE_TAKINGREADIED sizeof(Boolean)
#define BIT_TAKINGREADIED 2

#define OFFSET_AWARE OFFSET_TAKINGREADIED + 0
#define SIZE_AWARE sizeof(Boolean)
#define BIT_AWARE 3

#define OFFSET_PC OFFSET_AWARE + 0
#define SIZE_PC sizeof(Boolean)
#define BIT_PC 4

#define OFFSET_COUNTER OFFSET_PC + SIZE_PC
#define SIZE_COUNTER sizeof(UInt8)

#define OFFSET_DURATION OFFSET_COUNTER + SIZE_COUNTER
#define SIZE_DURATION sizeof(UInt8)

#define OFFSET_MODIFIER OFFSET_DURATION + SIZE_DURATION
#define SIZE_MODIFIER sizeof(Int16)

#define OFFSET_NAME_CHAR OFFSET_MODIFIER + SIZE_MODIFIER

/* Effect-specific fields of database records. */

#define OFFSET_NOMINAL OFFSET_PERFDATA
#define SIZE_NOMINAL sizeof(UInt8)

#define OFFSET_REMAINING OFFSET_NOMINAL + SIZE_NOMINAL
#define SIZE_REMAINING sizeof(UInt8)

#define OFFSET_OWNER OFFSET_REMAINING + SIZE_REMAINING
#define SIZE_OWNER sizeof(UInt32)

#define OFFSET_N_SUMMONED OFFSET_OWNER + SIZE_OWNER  // DB_VERSION=2
#define SIZE_N_SUMMONED sizeof(UInt8) // DB_VERSION=2

#define OFFSET_NAME_EFFECT OFFSET_N_SUMMONED + SIZE_N_SUMMONED


/***************************************************************************
 * utility functions
 ***************************************************************************/

/* Create objects for entries in encounter database. */
void LoadPerformers(UInt16 dbVersion) {
	UInt16 nPerf, i;
	UInt16 nEncounter = PERF_COUNT;
	MemHandle h;
	Byte* dataP;
	PerformerType type;
	
	for (i=0; i<nEncounter; i++) {
		// Determine what kind of performer object to create
		h = DmQueryRecord(dbEncounter,i);
		ErrFatalDisplayIf(h==0,"Could not query encounter database for performer.");
		dataP = (Byte*) MemHandleLock(h);
		MemMove(&type,dataP+OFFSET_TYPE,sizeof(type));
		MemHandleUnlock(h);

		// Instantiate appropriate object for this database entry
		switch (type) {
			case CHARACTER:
				new Character(dbVersion,i);
				break;
			case EFFECT:
				new Effect(dbVersion,i);
				break;
			default:
				ErrNonFatalDisplay("Invalid Performer type!");
				break;
		}
	}

}

/*
 * Get pointer to the object associated with a database entry. Leaves record unbusy 
 * and unlocked.
 * Input argument iEncounter - index of object in encounter database. 
 * Returns - reference to object. NULL if there is no entry for that index.
 */
Performer* queryPerformerP(UInt16 iEncounter) {
	Performer* result = NULL;
	// get record from database
	MemHandle h = DmQueryRecord(dbEncounter,iEncounter);
	if (h==NULL) {
		ErrDisplay("Could not get performer record.");
		//ErrAlert(DmGetLastErr());
		return NULL;
	}
	Byte* p = (Byte*) MemHandleLock(h);
	// get object reference stored in database
	MemMove(&result, p+OFFSET_POINTER, SIZE_POINTER);
	MemHandleUnlock(h);
	return result;
}

/*
 * Get pointer to the object associated with a database entry. Leaves record unbusy 
 * and unlocked.
 * Input argument id - unique identification code of object in encounter database. 
 * Returns - reference to object. NULL if there is no entry for that index.
 */
Performer* queryPerformerPfromID(UInt32 id) {
	if (id >= ID_MAXIMUM) return NULL;
	UInt16 iEncounter;
	Err e = DmFindRecordByID(dbEncounter,id,&iEncounter);
	if (e!=errNone) {
		//ErrDisplay("Performer ID not found.");
		//ErrAlert(e);
		return NULL;
	}
	return queryPerformerP(iEncounter);
}

/*
 * Sort two Performers by their names. Utility function used below.
 */
static Int16 ComparePerformersNames(Byte* pd1, Byte* pd2) {
		// get pointers to names
		PerformerType type;
		Char* name1;
		MemMove(&type,pd1+OFFSET_TYPE,SIZE_TYPE);
		switch (type) {
			case CHARACTER:
				name1 = (Char*) (pd1 + OFFSET_NAME_CHAR);
				break;
    		case EFFECT:
				name1 = (Char*) (pd1 + OFFSET_NAME_EFFECT);
				break;
			default:
				ErrFatalDisplay("Invalid performer type.");
				return 0;
		}
		Char* name2;
		MemMove(&type,pd2+OFFSET_TYPE,SIZE_TYPE);
		switch (type) {
			case CHARACTER:
				name2 = (Char*) (pd2 + OFFSET_NAME_CHAR);
				break;
    		case EFFECT:
				name2 = (Char*) (pd2 + OFFSET_NAME_EFFECT);
				break;
			default:
				ErrFatalDisplay("Invalid performer type.");
				return 0;
		}
		// Do the comparison
		Int16 result = StrCaselessCompare(name1,name2);
		if (result==0) { // same name
			// Sort by ID number, so that sort order is consistent
			Performer* po1, * po2;
			MemMove(&po1,pd1+OFFSET_POINTER,SIZE_POINTER);
			MemMove(&po2,pd2+OFFSET_POINTER,SIZE_POINTER);
			Int32 temp = po1->getID() - po2->getID();
			return (temp < 0)? -1 : 1;
		}
		return result;
}

/*
 * Application defined function for comparing database records.
 * See DmComparF on p. 583 of API.
 *
 */
Int16 ComparePerformers(void *rec1, void *rec2, 
                                        Int16 other, 
										SortRecordInfoPtr rec1SortInfo,
                                        SortRecordInfoPtr rec2SortInfo,
                                        MemHandle appInfoH) {
	// TODO - F2 - Refactor ComparePerformers with subroutines for readability
	
    Byte* pd1 = (Byte*) rec1;
    Byte* pd2 = (Byte*) rec2;

	if ( (other==0) && (round < 0) ) { // default DM screen and no combat in progress
	/*
	Player Character
		name
	not Player Character
		name
	*/
		// compare Player Character flags
		PerformerType type;
		Byte buffer;

		MemMove(&type, pd1 + OFFSET_TYPE, SIZE_TYPE);
		Boolean isChar1 = type != EFFECT;
		Boolean isPC1 = false;
		if (isChar1 == true) {
			MemMove(&buffer, pd1 + OFFSET_PC, SIZE_PC);
			isPC1 = (buffer & 1<<BIT_PC) != 0;
		}
			
		MemMove(&type, pd2 + OFFSET_TYPE, SIZE_TYPE);
		Boolean isChar2 = type != EFFECT;
		Boolean isPC2 = false;
		if (isChar2 == true) {
			MemMove(&buffer, pd2 + OFFSET_PC, SIZE_PC);
			isPC2 = (buffer & 1<<BIT_PC) != 0;
		}
		
		if (isChar1 && isChar2) { // Both performers are characters
			if ( isPC1 && !isPC2) { // Only first performer is a PC
				return -1;
			} else if ( !isPC1 && isPC2 ) { // Only second performer is a PC
				return 1;
			}
		} else if (isChar1) { // Only first performer is a character
			if (isPC1) // Only first performer is a PC
				return -1;
		} else if (isChar2) { // Only second performer is a character
			if (isPC2) // Only second performer is a PC
				return 1;
		}

		// sort alphabetically
		return ComparePerformersNames(pd1,pd2);

	} else if (other==1) { // Roll Initiative screen
	/*
	Character
		Active
			Player Character
				name
			not Player Character
				name
	not Active or not Character
	*/
		// sort active by alpha. Effects and Inactive are lower and irrelevant
		PerformerType type;
		MemMove(&type, pd1 + OFFSET_TYPE, SIZE_TYPE);
		Boolean a1 = type == EFFECT;
		MemMove(&type, pd2 + OFFSET_TYPE, SIZE_TYPE);
		Boolean a2 = type == EFFECT;
		if ( a1 && a2 ) { // don't care
			return 0;
		} else if ( a1 && !a2 ) { // only second is character
			return 1;
		} else if ( !a1 && a2 ) { // only first is character
			return -1;
		} // else, continue

		MemMove(&a1, pd1 + OFFSET_ACTIVE, SIZE_ACTIVE);
		MemMove(&a2, pd2 + OFFSET_ACTIVE, SIZE_ACTIVE);
		if ( !a1 && !a2 ) { // neither active
			return 0; // don't care
		} else if ( a1 && !a2 ) { // only first is active
			return -1; 
		} else if ( !a1 && a2 ) { // only second is active
			return 1; 
		} else { // both active
			Byte buffer;
			MemMove(&buffer, pd1 + OFFSET_PC, SIZE_PC);
			a1 = (buffer & 1<<BIT_PC) != 0;
			MemMove(&buffer, pd2 + OFFSET_PC, SIZE_PC);
			a2 = (buffer & 1<<BIT_PC) != 0;

			if ( a1 && !a2 ) { // only first is player character
				return -1;
			} else if ( !a1 && a2 ) { // only second is player character
				return 1;
			}		
		
			// sort alphabetically
			return ComparePerformersNames(pd1,pd2);
		}
	} else if ( (round>=0) && (other==0) ) { // default DM screen during combat
		/* Order hierarchy:
		active
			initiative
				order in list
		inactive
			PC
				name
			not PC
				name
		*/
		Boolean a1;
		Boolean a2;
		MemMove(&a1, pd1 + OFFSET_ACTIVE, SIZE_ACTIVE);
		MemMove(&a2, pd2 + OFFSET_ACTIVE, SIZE_ACTIVE);
		if ( !a1 && !a2 ) { // neither active

    		PerformerType type;
    		Byte buffer;
    
    		MemMove(&type, pd1 + OFFSET_TYPE, SIZE_TYPE);
    		Boolean isChar1 = type != EFFECT;
    		Boolean isPC1 = false;
    		if (isChar1 == true) {
    			MemMove(&buffer, pd1 + OFFSET_PC, SIZE_PC);
    			isPC1 = (buffer & 1<<BIT_PC) != 0;
    		}
    			
    		MemMove(&type, pd2 + OFFSET_TYPE, SIZE_TYPE);
    		Boolean isChar2 = type != EFFECT;
    		Boolean isPC2 = false;
    		if (isChar2 == true) {
    			MemMove(&buffer, pd2 + OFFSET_PC, SIZE_PC);
    			isPC2 = (buffer & 1<<BIT_PC) != 0;
    		}
    		
    		if (isChar1 && isChar2) { // Both performers are characters
    			if ( isPC1 && !isPC2) { // Only first performer is a PC
    				return -1;
    			} else if ( !isPC1 && isPC2 ) { // Only second performer is a PC
    				return 1;
    			}
    		} else if (isChar1) { // Only first performer is a character
    			if (isPC1) // Only first performer is a PC
    				return -1;
    		} else if (isChar2) { // Only second performer is a character
    			if (isPC2) // Only second performer is a PC
    				return 1;
    		}
			
			// sort alphabetically
			return ComparePerformersNames(pd1,pd2);
		} else if ( a1 && !a2 ) { // only first is active
			return -1; 
		} else if ( !a1 && a2 ) { // only second is active
			return 1; 
		} else { // both active
			Performer* po1, * po2;
			MemMove(&po1, pd1 + OFFSET_POINTER, SIZE_POINTER);
			MemMove(&po2, pd2 + OFFSET_POINTER, SIZE_POINTER);

			if (po1 == first) {
				// first performer comes before any other
				return -1;
			}
			if (po2 == first) {
				// second performer comes before any other
				return 1;
			}
			// sort by initiative value
			Int16 i1, i2;
			MemMove(&i1, pd1 + OFFSET_INITIATIVE, SIZE_INITIATIVE);
			MemMove(&i2, pd2 + OFFSET_INITIATIVE, SIZE_INITIATIVE);
			Int32 temp = i2 - i1;
			if (temp != 0) {
				return (temp<0)? -1 : 1;
			}
			// sort by initiative list
			Performer* nextP = po1->getNext();
			if (nextP==NULL) { // initiative list is not yet active
				// sort alphabetically
				return ComparePerformersNames(pd1,pd2);
			}
			UInt16 nextInit = nextP->getInitiative();
			while ( (nextInit == i1) &&  // still on same init count 
			        (nextP != po1) && // not back to start again
					(nextP != first) ) { // not at end of list
				if (nextP == po2) {
					// first performer comes before second
					return -1;
				}
				nextP = nextP->getNext();
				nextInit = nextP->getInitiative();
			}
			// first performer comes after second
			return 1;
		}
	} // endif for default DM screen during combat

	ErrFatalDisplay("Invalid search parameters.");
    return 0;
}

/***************************************************************************
 * Performer Methods
 ***************************************************************************/

/* Constructor - instantiates performer using encounter database record at index. */
Performer::Performer(UInt16 dbVersion, UInt16 index) {
	/* get performer from encounter database */
	MemHandle h = DmGetRecord(dbEncounter, index);
	ErrFatalDisplayIf(h==0,"Error getting performer record from encounter db.");

	// lock the record's data
	Byte* p = (Byte*) MemHandleLock(h);

	// get object fields from record data
	Err e = DmRecordInfo(dbEncounter,index,NULL,&id,NULL);
	ErrFatalDisplayIf(e!=errNone,"Could not get record info for performer.");

	// set record fields for this launch of the app
	Performer* stackThis = this;
	DmWrite(p, OFFSET_POINTER, &stackThis, sizeof(stackThis));

	// release record memory
	MemHandleUnlock(h);
	// release record
	DmReleaseRecord(dbEncounter, index, true); // true = dirty
}

Character::Character(UInt16 dbVersion, UInt16 index) : Performer(dbVersion,index) {
}

Effect::Effect(UInt16 dbVersion, UInt16 index) : Performer(dbVersion,index) {
	// Only need to do this stuff if database is version 1
	if (dbVersion != 1) return;

	MemHandle h;
	Byte* p;
	
	// determine old record size
   	h = DmGetRecord(dbEncounter,index);
	UInt32 oldRecSize = MemHandleSize(h);
	DmReleaseRecord(dbEncounter,index,false);

	// calculate new record size
	UInt32 newRecSize = oldRecSize + SIZE_N_SUMMONED;

	// Resize record. 
	h = DmResizeRecord(dbEncounter,index,newRecSize);
	ErrFatalDisplayIf(h==NULL,"Not enough memory for updated effect record.");
	p = (Byte*) MemHandleLock(h);

	// buffer name for moving it	
	UInt16 nameSize = getNameSize();
	UInt16 nameOffset = getNameOffset();
	MemPtr nameP = MemPtrNew(nameSize);
	ErrFatalDisplayIf(nameP==NULL,"Not enough memory for updating effect record.");
	MemMove(nameP, p + nameOffset - SIZE_N_SUMMONED, nameSize);
	
	// insert default number of summoned creatures field
	UInt8 nSummoned = 0;
	DmWrite(p, OFFSET_N_SUMMONED, &nSummoned, SIZE_N_SUMMONED); 
	
	// put name at new location
	DmWrite(p, nameOffset, nameP, nameSize);

	// release memory
	MemHandleUnlock(h);
	MemPtrFree(nameP);
	
	DmReleaseRecord(dbEncounter,index,true);
}

Performer::~Performer(void) {
    UInt16 iEnc = getIEncounter();
    Err e = DmRemoveRecord(dbEncounter,iEnc);
    ErrFatalDisplayIf(e!=errNone,"Error removing performer's record.");
    /* DmArchiveRecord marks record as deleted but keeps data.
    Then the Backup Conduit saves it. Record never goes away.
    Either need a custom conduit to remove record, or need to
    handle sysAppLaunchCmdSyncNotify and remove archived records. */
    // TODO - F2 - Implement some scheme to backup deleted performers
    /*Err e = DmArchiveRecord(dbEncounter,iEnc);
    // Move to end of database
    e = DmMoveRecord (dbEncounter,iEnc,DmNumRecords(dbEncounter));
    ErrFatalDisplayIf(e!=errNone,"Error moving archived record.");
    */
}

/*
 * Utility function for Character and Effect constructors.
 * Creates and locks a new encounter database record, and writes performer's
 * name, pointer, ID, previous, next, and flags to it. 
 * Parameters:
 * -> name	pointer to string with performer's name
 * <- h		pointer to handle of performer's record. Must be unlocked later.
 * <- p		pointer to pointer to performer's record.
 * <- iDb	performer's encounter database index. Must be released later
 */
Err Performer::OpenRecord(Char* name, MemHandle* h, MemPtr* p, UInt16* iDb) {
	Err e; 
	const UInt16 nameSize = StrSize(name);
	*iDb = PERF_COUNT; // append new record to database 
	// Create record in database
	*h = DmNewRecord(dbEncounter, iDb, getNameOffset()+nameSize);
	if (*h == 0) {
		ErrDisplay("Error creating performer record in encounter db.");
		e = DmGetLastErr();
		ErrAlert(e);
		return e;
	}
	// lock database memory so we can write to it
	*p = MemHandleLock(*h);

	// get unique code for performer, assign to object
	e = DmRecordInfo(dbEncounter,*iDb,NULL,&id,NULL);
	if (e != errNone) {
		ErrDisplay("Could not get record info for performer.");
		ErrAlert(e);
		return e;
	}	

	// reference to this instance
	const Performer* stackThis = this;
	e |= DmWrite(*p, OFFSET_POINTER, &stackThis, SIZE_POINTER);
	// Not in initiative list, so initialize previous and next to itself
	e |= DmWrite(*p, OFFSET_PREV, &id, SIZE_PREV);
	e |= DmWrite(*p, OFFSET_NEXT, &id, SIZE_NEXT);
	Boolean temp = false;
	e |= DmWrite(*p, OFFSET_ACTIVE, &temp, SIZE_ACTIVE);
	// size of name
	e |= DmWrite(*p, OFFSET_NAMESIZE, &nameSize, SIZE_NAMESIZE);
	if (e != errNone) {
		ErrDisplay("Could not write record data for performer.");
		ErrAlert(e);
		return e;
	}
	// name
	e = DmWrite(*p, getNameOffset(), name, nameSize);
	ErrFatalDisplayIf(e!=errNone,"Error writing character name to encounter db.");
	if (e != errNone) {
		ErrDisplay("Error writing performer name to encounter db.");
		ErrAlert(e);
		return e;
	}
	return errNone;
}

UInt32 Performer::getID(void) {
	return id; 
};

/* Set object data in database record. See DmWrite regarding arguments. */
void Performer::set(UInt32 offset, const void* srcP, UInt32 bytes) {
	Err e;
	// determine which database entry to modify
	UInt16 iEncounter = getIEncounter();
	// get record and mark as busy
	MemHandle h = DmGetRecord(dbEncounter,iEncounter);
	ErrFatalDisplayIf(h==NULL,"Could not get performer's record for writing.");
	MemPtr p = MemHandleLock(h);
	// write data
	e = DmWrite(p, offset, srcP, bytes);
	ErrFatalDisplayIf(e!=errNone,"Problem writing data to record.");
	// release memory and record
	MemHandleUnlock(h);
	e = DmReleaseRecord(dbEncounter,iEncounter,true); // true = dirty
}

/* Set object data in database record. 
 * Assigns value at offset to bit. */
void Performer::set(UInt32 offset, UInt8 bit, Boolean value) {
	ErrFatalDisplayIf(bit>7,"Attempting to set bit outside byte boundary!");
	Err e;
	// determine which database entry to modify
	UInt16 iEncounter = getIEncounter();
	// get record and mark as busy
	MemHandle h = DmGetRecord(dbEncounter,iEncounter);
	ErrFatalDisplayIf(h==NULL,"Could not get performer's record for writing.");
	Byte* p = (Byte*) MemHandleLock(h);
	// get byte
	Byte buffer;
	MemMove(&buffer,p+offset,sizeof(buffer));
	// set/reset bit
	if (value==true) {
		buffer |= 1 << bit;
	} else {
		Byte mask = 1 << bit;
		buffer &= ~mask;
	}
	// (re)write data
	e = DmWrite(p, offset, &buffer, sizeof(buffer));
	ErrFatalDisplayIf(e!=errNone,"Problem writing data to record.");
	// release memory and record
	MemHandleUnlock(h);
	e = DmReleaseRecord(dbEncounter,iEncounter,true); // true = dirty
}

/* Return pointer to unbusy record for object. 
 * Calling method MUST unlock pointer after using it!
 */
MemPtr Performer::get(void) {
	UInt16 iEncounter = getIEncounter();
	MemHandle h = DmQueryRecord(dbEncounter, iEncounter);
	if (h==0) {
		ErrFatalDisplay("Could not get performer's record for reading.");
		return NULL;
	}
	MemPtr p = MemHandleLock(h);
	return p;
}

/*
 * Gets numBytes bytes starting at offset in this performer's database
 * record, and copies them to dstP.
 */
void Performer::get(Int32 offset, void* dstP, Int32 numBytes) {
	UInt16 iEncounter = getIEncounter();
	MemHandle h = DmQueryRecord(dbEncounter, iEncounter);
	if (h==0) {
		ErrAlert(DmGetLastErr());
		ErrFatalDisplay("Could not get performer's record for reading.");
		return;
	}
	// (Pointer arithmatic is forbidden on void* by ANSI C++.)
	Byte* p = (Byte*) MemHandleLock(h);
	MemMove(dstP,p + offset,numBytes);
	MemPtrUnlock(p); 
}
// Convenience Macro - use for most accessor functions.
#define RETURNGET(TYPE,OFFSET) \
	TYPE result;\
	get(OFFSET, &result, sizeof(result));\
	return result;
	
Performer* Performer::getPrev(void){
	// Get id code of previous performer
	UInt32 value;
	get(OFFSET_PREV, &value, SIZE_PREV);
	// Get reference to that performer's object
	return queryPerformerPfromID(value);
}
void Performer::setPrev(Performer* prev) {
	set(OFFSET_PREV, &(prev->id), SIZE_PREV);
}

Performer* Performer::getNext(void){
	// Get id code of next performer
	UInt32 value;
	get(OFFSET_NEXT, &value, SIZE_NEXT);
	// Get reference to that performer's object
	return queryPerformerPfromID(value);
}
void Performer::setNext(Performer* next) {
	set(OFFSET_NEXT, &(next->id), SIZE_NEXT);
}
/*
 * Insert the given performer after this performer in the initiative list.
 * Argument - newbie - performer that is not in the list
 */
void Performer::insertAfter(Performer* newbie){
	Err e;

	// Lock this performer's record
	UInt16 iEnc = getIEncounter();
	MemHandle h  = DmGetRecord(dbEncounter, iEnc);
	if (h==0) {
		ErrFatalDisplay("Could not get performer's record.");
		return;
	}
	Byte* p  = (Byte*) MemHandleLock(h);

	// Lock newbie's record
	UInt16 iEncN = newbie->getIEncounter();
	MemHandle hN = DmGetRecord(dbEncounter, iEncN);
	if (hN==0) {
		ErrFatalDisplay("Could not get newbie's record.");
		return;
	}
	Byte* pN = (Byte*) MemHandleLock(hN);

	// lock record of old follower
	UInt16 iEncOldF;
	MemHandle hOldF;
	Byte* pOldF;
	UInt32 nextID;
	MemMove(&nextID,p+OFFSET_NEXT,SIZE_NEXT);
	if (nextID == id) { // this performer is the old follower (not in list yet)
		iEncOldF = dmMaxRecordIndex;
		hOldF = 0;
		pOldF = p;
	} else {
		e = DmFindRecordByID(dbEncounter,nextID,&iEncOldF);
		ErrFatalDisplayIf(e!=errNone,"Performer record not found for next ID.");
		hOldF = DmGetRecord(dbEncounter, iEncOldF);
    	if (hOldF==0) {
    		ErrFatalDisplay("Could not get old follower's record.");
    		return;
    	}
		pOldF  = (Byte*) MemHandleLock(hOldF);
	}

	// set newbie's previous to this
	e = DmWrite(pN, OFFSET_PREV, &id, SIZE_PREV);
	ErrFatalDisplayIf(e!=errNone,"Problem writing prev to newbie.");

	// set newbie's next to old next
	e = DmWrite(pN, OFFSET_NEXT, &nextID, SIZE_NEXT);
	ErrFatalDisplayIf(e!=errNone,"Problem writing next to newbie.");

	// set old follower's previous to newbie
	e = DmWrite(pOldF, OFFSET_PREV, &(newbie->id), SIZE_PREV);
	ErrFatalDisplayIf(e!=errNone,"Problem writing prev to old follower.");

	// set next to newbie
	e = DmWrite(p, OFFSET_NEXT, &(newbie->id), SIZE_NEXT);
	ErrFatalDisplayIf(e!=errNone,"Problem writing next.");

	// release memory as appropriate
	MemHandleUnlock(h);
	MemHandleUnlock(hN);
	if (hOldF!=0) MemHandleUnlock(hOldF);

	DmReleaseRecord(dbEncounter, iEnc, true);
	DmReleaseRecord(dbEncounter, iEncN, true);
	if (iEncOldF < dmMaxRecordIndex) DmReleaseRecord(dbEncounter, iEncOldF, true);
}
/*
 * Insert the given performer before this performer in the initiative list.
 * Argument - newbie - performer that is not in the list
 */
void Performer::insertBefore(Performer* newbie){
	Err e;

	// Lock this performer's record
	UInt16 iEnc = getIEncounter();
	MemHandle h  = DmGetRecord(dbEncounter, iEnc);
	if (h==0) {
		ErrFatalDisplay("Could not get performer's record.");
		return;
	}
	Byte* p  = (Byte*) MemHandleLock(h);

	// Lock newbie's record
	UInt16 iEncN = newbie->getIEncounter();
	MemHandle hN = DmGetRecord(dbEncounter, iEncN);
	if (hN==0) {
		ErrFatalDisplay("Could not get newbie's record.");
		return;
	}
	Byte* pN = (Byte*) MemHandleLock(hN);

	// lock record of old previous
	UInt16 iEncOldP;
	MemHandle hOldP;
	Byte* pOldP;
	UInt32 prevID;
	MemMove(&prevID, p + OFFSET_PREV, SIZE_PREV);
	if (prevID == id) { // this performer is the old previous (only one in list)
		iEncOldP = dmMaxRecordIndex;
		hOldP = 0;
		pOldP = p;
	} else {
		e = DmFindRecordByID(dbEncounter,prevID,&iEncOldP);
		ErrFatalDisplayIf(e!=errNone,"Performer record not found for previous ID.");
		hOldP = DmGetRecord(dbEncounter, iEncOldP);
    	if (hOldP==0) {
    		ErrFatalDisplay("Could not get old previous's record.");
    		return;
    	}
		pOldP  = (Byte*) MemHandleLock(hOldP);
	}

	// set newbie's next to this
	e = DmWrite(pN, OFFSET_NEXT, &id, SIZE_NEXT);
	ErrFatalDisplayIf(e!=errNone,"Problem writing next to newbie.");

	// set newbie's previous to old previous
	e = DmWrite(pN, OFFSET_PREV, &prevID, SIZE_PREV);
	ErrFatalDisplayIf(e!=errNone,"Problem writing previous to newbie.");

	// set old previous's next to newbie
	e = DmWrite(pOldP, OFFSET_NEXT, &(newbie->id), SIZE_NEXT);
	ErrFatalDisplayIf(e!=errNone,"Problem writing next to old previous.");

	// set previous to newbie
	e = DmWrite(p, OFFSET_PREV, &(newbie->id), SIZE_NEXT);
	ErrFatalDisplayIf(e!=errNone,"Problem writing previous.");

	// release memory as appropriate
	MemHandleUnlock(h);
	MemHandleUnlock(hN);
	if (hOldP!=0) MemHandleUnlock(hOldP);

	DmReleaseRecord(dbEncounter, iEnc, true);
	DmReleaseRecord(dbEncounter, iEncN, true);
	if (iEncOldP < dmMaxRecordIndex) DmReleaseRecord(dbEncounter, iEncOldP, true);
}

/* Return a handle for the performer's name, and set the offset and size. */
MemHandle Performer::getNameHandle(Int16* dataOffset, Int16* dataSize) {
	*dataOffset = getNameOffset();
	*dataSize = getNameSize();

	/* determine which database record to use */
	UInt16 index = getIEncounter();
	// get handle to database record, leave it unbusy
	MemHandle result = DmQueryRecord(dbEncounter,index);
	ErrFatalDisplayIf(result==NULL,"Could not get record for character.");

	/* Validate name size */
	// get name
	Char* name = (Char*) MemHandleLock(result);
	name += *dataOffset;
	UInt16 len = StrSize(name);
	MemHandleUnlock(result);
	// name size may have changed due to editing of field 
	if (len != *dataSize) setNameSize(len);	 // update database
	*dataSize = len;
	
	return result;
}

/* 
 * Returns a handle to record and pointer to the performer's name in memory.
 * This leaves the record locked, and it must be unlocked separately.
 */
MemHandle Performer::getNameHandle(Char** string) {
	Int16 offset, size; // dummy variables
    MemHandle h = getNameHandle(&offset,&size);
	*string = (Char*) MemHandleLock(h);
	*string += getNameOffset();
	return h;
}

/* Copy the performer's name to the memory pointed to by the argument.
 * Allocate or resize memory, if necessary.
 * The calling function is responsible for disposing of the memory.
 */
void Performer::nameCopy(Char** dest) {
	//CharacterNameType* dataP = (CharacterNameType*) get();
	UInt16 size = getNameSize();
	if (*dest == NULL) {
		*dest = (Char*) MemPtrNew( size );
	}
	if (*dest != NULL) {
		Err e = MemPtrResize(*dest,size);
		if (e==errNone) {
			Char* nameP;
			MemHandle h = getNameHandle(&nameP);
			StrCopy(*dest,nameP);
			MemHandleUnlock(h);
		}
	}
}

void Performer::setName(Char* newName) {
	Err e;
	MemHandle h;
	MemPtr p;
	UInt32 offset = getNameOffset();

	// determine which database entry to modify
	UInt16 iEncounter = getIEncounter();

	// see if size of name changed
	UInt16 oldNameSize = getNameSize();
	UInt16 newNameSize = StrSize(newName);

   	h = DmGetRecord(dbEncounter,iEncounter);

	if (oldNameSize != newNameSize) {
		/* Resize record */

		// determine old record size
    	UInt32 oldRecSize = MemHandleSize(h);
    	DmReleaseRecord(dbEncounter,iEncounter,false);
    
    	// calculate new record size, by adding change in name sizes
    	UInt32 newRecSize = oldRecSize + newNameSize - oldNameSize;
    	ErrFatalDisplayIf(oldRecSize + newNameSize < oldNameSize,
			"New name causes negative record size.");
	
    	// Resize record. 
    	h = DmResizeRecord(dbEncounter,iEncounter,newRecSize);
    	if (h==NULL) {
    		ErrDisplay("Not enough room for resized record.");
    		return;
    	}
		
		// Mark record as busy
		h = DmGetRecord(dbEncounter,iEncounter);
		// lock record
    	p = MemHandleLock(h);
    
    	// write new name size
    	e = DmWrite(p, OFFSET_NAMESIZE, &newNameSize, sizeof(newNameSize));
    	ErrFatalDisplayIf(e!=errNone,"Problem writing new name size to record.");

	} else { // name size did not change
		/* Just lock the record */
		p = MemHandleLock(h);
	}
	
	// write new name, at appropriate location
	e = DmWrite(p, offset, newName, newNameSize);
	ErrFatalDisplayIf(e!=errNone,"Problem writing new name to record.");

	// release memory
	MemHandleUnlock(h);
	
	DmReleaseRecord(dbEncounter,iEncounter,true);
}

/* Utility function for copy() functions.
   Appends a space, possibly one zero, and the argument to performer's name.
   Allocates immobile memory for name, which must be freed later. */
Char* Performer::getNamePlusNumber(Int16 labelI) {
    // return  "<name><space><index>"
    // get <name>
    Char* name;
	MemHandle h = getNameHandle(&name);
    // get <constant string>
    const Char* space = " ";
	// get a zero?
	const Char* zero = ( (labelI>-10) && (labelI<10) )? "0" : "" ;
	// get <index>
	Char label[maxStrIToALen];
	StrIToA(label,labelI);
    // concatenate above strings
    Char* result = (Char*) MemPtrNew(MAX_NAME_BYTES);

	// Copy name to result
    WChar ch;
    UInt16 suffixSize = StrLen(space) + StrLen(zero) + StrSize(label);
	UInt16 iNext = 0;
	UInt16 nameLength = StrLen(name);
	UInt16 iLast = 0;
    while ( (iNext < MAX_NAME_BYTES - suffixSize) && (iNext < nameLength) ) {
        iNext += TxtGlueGetNextChar(name, iLast, &ch);
        TxtGlueSetNextChar(result, iLast, ch);
		iLast = iNext;
    }
	// Null-terminate result
	TxtGlueSetNextChar(result, iLast, chrNull);

	// Append remaining strings to result
    StrCat(result,space);
	StrCat(result,zero);
    StrCat(result,label);

    MemHandleUnlock(h); // done with name's record
	return result;
}

UInt16 Performer::getNameSize(void){
	RETURNGET(UInt16,OFFSET_NAMESIZE);
}
void Performer::setNameSize(UInt16 newSize) {
	set(OFFSET_NAMESIZE, &newSize, sizeof(newSize));
}

Int16 Performer::getInitiative(void){
	RETURNGET(Int16,OFFSET_INITIATIVE);
}
void Performer::setInitiative(Int16 initiative){
	set(OFFSET_INITIATIVE, &initiative, sizeof(initiative));
}

UInt16 Performer::getIEncounter(void) {
	UInt16 iEnc;
	Err e = DmFindRecordByID(dbEncounter,id,&iEnc);
	ErrFatalDisplayIf(e!=errNone,"Performer record not found for ID.");
	return iEnc;
}

PerformerType Performer::getType(void){
	RETURNGET(PerformerType,OFFSET_TYPE);
}

Boolean Performer::isActive(void){
	RETURNGET(Boolean,OFFSET_ACTIVE);
}
void Performer::setActive(Boolean value) {
	set(OFFSET_ACTIVE, &value, SIZE_ACTIVE);
}

/***************************************************************************
 * Character methods.
 ***************************************************************************/
/* Constructor - instantiate new Character and add it to encounter database. */
Character::Character(Char* name, Int16 mod, UInt8 counter, UInt8 duration){
	Err e = errNone;
	
	MemHandle h;
	MemPtr p;
	UInt16 iDb;
	e = OpenRecord(name, &h, &p, &iDb);
	if (e == errNone){
    	/* Data specific to Character class */
    	// performer type
    	const PerformerType type = CHARACTER;
    	e |= DmWrite(p, OFFSET_TYPE, &type, SIZE_TYPE);
    	// default initiative
    	const Int16 init = 10 + mod;
    	e |= DmWrite(p, OFFSET_INITIATIVE, &init, SIZE_INITIATIVE);
    	// arguments
    	e |= DmWrite(p, OFFSET_MODIFIER, &mod, SIZE_MODIFIER);
    	e |= DmWrite(p, OFFSET_COUNTER, &counter, SIZE_COUNTER);
    	e |= DmWrite(p, OFFSET_DURATION, &duration, SIZE_DURATION);
    	// set flags to defaults
    	const Boolean temp = 
			0<<BIT_DELAYING | 0<<BIT_READIED | 0<<BIT_TAKINGREADIED | 1<<BIT_AWARE | 0<<BIT_PC;
    	e |= DmWrite(p, OFFSET_DELAYING, &temp, SIZE_DELAYING);
    	if (e != errNone) {
    		ErrDisplay("Error writing record data for character.");
    		ErrAlert(e);
    	}
	}
	// release record memory
	MemHandleUnlock(h);
	// release record
	e = DmReleaseRecord(dbEncounter, iDb, true); // true = dirty
	if (e != errNone) {
		ErrDisplay("Error releasing record data for character.");
		ErrAlert(e);
		return;
	}
}

Character::~Character(void) {
    UInt16 iEnc = getIEncounter();
    // Set character's effects' owners to NULL
	UInt16 n = PERF_COUNT;
   	for (UInt16 i=0; i < n; i++) {
		if (i != iEnc) { // skip this performer's record
       		Performer* other = queryPerformerP(i);
       		if (other->getType() == EFFECT) {
       			Effect* e = (Effect*) other;
       			if (e->getOwner() == this) {
       				e->setOwner(NULL);
       			}
       		}
		}
   	}
}

/*
 * Create a new Character that has the same values as this Character.
 * Only differences are ID (duh), database index (duh), and presence in initiative list.
 * The copy is not in init list.
 * The argument is used to append a label to the copy's name.
 * Returns pointer to the newly created character.
 */
Performer* Character::copy(Int16 labelI) {
	Char* name = getNamePlusNumber(labelI);
	Character* copy = new Character(name, getModifier(), getCounter(), getDuration());
	MemPtrFree(name);
	copy->setInitiative( getInitiative() );
	copy->setAware( isAware() );
	copy->setDelaying( isDelaying() );
	copy->setReadied( isReadied() );
	copy->setTakingReadied( isTakingReadied() );
	return copy;
}

UInt16 Character::getNameOffset(void) {
	return OFFSET_NAME_CHAR;
};

Int16 Character::getModifier(void){
	RETURNGET(Int16,OFFSET_MODIFIER);
}
void Character::setModifier(Int16 bonus){
	set(OFFSET_MODIFIER, &bonus, sizeof(bonus));
}

Boolean Character::isDelaying(void){
	Byte buffer;
	get(OFFSET_DELAYING, &buffer, SIZE_DELAYING);
	return (buffer & 1<<BIT_DELAYING) != 0;
}
void Character::setDelaying(Boolean value){
	set(OFFSET_DELAYING, BIT_DELAYING, value);
}

Boolean Character::isReadied(void){
	Byte buffer;
	get(OFFSET_READIED, &buffer, SIZE_READIED);
	return (buffer & 1<<BIT_READIED) != 0;
}
void Character::setReadied(Boolean value){
	set(OFFSET_READIED, BIT_READIED, value);
}

Boolean Character::isTakingReadied(void){
	Byte buffer;
	get(OFFSET_TAKINGREADIED, &buffer, SIZE_TAKINGREADIED);
	return (buffer & 1<<BIT_TAKINGREADIED) != 0;
}
void Character::setTakingReadied(Boolean value){
	set(OFFSET_TAKINGREADIED, BIT_TAKINGREADIED, value);
}

Boolean Character::isAware(void){
	Byte buffer;
	get(OFFSET_AWARE, &buffer, SIZE_AWARE);
	return (buffer & 1<<BIT_AWARE) != 0;
}
void Character::setAware(Boolean value){
	set(OFFSET_AWARE, BIT_AWARE, value);
}

Boolean Character::isPC(void){
	Byte buffer;
	get(OFFSET_PC, &buffer, SIZE_PC);
	return (buffer & 1<<BIT_PC) != 0;
}
void Character::setPC(Boolean value){
	set(OFFSET_PC, BIT_PC, value);
}

UInt8 Character::getCounter(void){
	RETURNGET(UInt8,OFFSET_COUNTER);
}
void Character::setCounter(UInt8 value){
	set(OFFSET_COUNTER, &value, sizeof(value));
}
/*
 * Increments "counter" and returns old value.
 */
UInt8 Character::incrementCounter(void){
	// get current counter value
	UInt8 result = getCounter();
	// calculate and set new counter value
	setCounter(result+1);
	// return old counter value
	return result;
}

UInt8 Character::getDuration(void){
	RETURNGET(UInt8,OFFSET_DURATION);
}
void Character::setDuration(UInt8 duration){
	set(OFFSET_DURATION, &duration, sizeof(duration));
}

/***************************************************************************
* Effect Methods
 ***************************************************************************/
/* Constructor - create new effect with given name, duration, rounds remaining
 * and owner.
 * Creates unowned effect if owner is NULL. */
Effect::Effect(Char* name, UInt8 nominal, UInt8 remaining, Character* owner){
	Err e = errNone;
	
	MemHandle h;
	MemPtr p;
	UInt16 iDb;
	e = OpenRecord(name, &h, &p, &iDb);
	if (e == errNone){
    	// performer type
    	const PerformerType type = EFFECT;
    	e |= DmWrite(p, OFFSET_TYPE, &type, SIZE_TYPE);
    	// prepare to insert in initiative list
     	Boolean temp = (round >= 0);
    	e |= DmWrite(p, OFFSET_ACTIVE, &temp, SIZE_ACTIVE);
    	const Int16 init = (owner==NULL)? -99 : owner->getInitiative();
    	e |= DmWrite(p, OFFSET_INITIATIVE, &init, SIZE_INITIATIVE);
    	// EFFECT specific data
    	e |= DmWrite(p, OFFSET_NOMINAL, &nominal, SIZE_NOMINAL);
    	e |= DmWrite(p, OFFSET_REMAINING, &remaining, SIZE_REMAINING);
    	UInt32 ownerID = (owner==NULL)? getID() : owner->getID();
    	e |= DmWrite(p, OFFSET_OWNER, &ownerID, SIZE_OWNER);
		UInt8 nSummoned = 0;
		e |= DmWrite(p, OFFSET_N_SUMMONED, &nSummoned, SIZE_N_SUMMONED);
    	if (e != errNone) {
    		ErrDisplay("Error writing record data for effect.");
    		ErrAlert(e);
    	}
	}
	// release record memory
	MemHandleUnlock(h);
	// release record
	e = DmReleaseRecord(dbEncounter, iDb, true); // true = dirty
	if (e != errNone) {
		ErrDisplay("Error releasing record data for effect.");
		ErrAlert(e);
		return;
	}
	// if playing, insert this effect before any characters with this initiative
	if (round >= 0) {
		addToInitList();
	}
}

Effect::~Effect(void) {
    UInt16 iEnc = getIEncounter();
    // Set effect's summoned creatures' owners to NULL
}

/*
 * Create a new Effect that has the same values as this Effect.
 * Only differences are ID (duh) and database index (duh).
 * If owner is active, then the copy is added to the init list.
 * The argument is used to append a label to the copy's name.
 * Returns pointer to the newly created effect.
 */
Performer* Effect::copy(Int16 labelI) {
	Char* name = getNamePlusNumber(labelI);
	Effect* copy = new Effect(name, getNominal(), getRemaining(), getOwner());
	MemPtrFree(name);
	copy->setInitiative( getInitiative() );
	return copy;
}

UInt16 Effect::getNameOffset(void) {
	return OFFSET_NAME_EFFECT;
};

UInt8 Effect::getNominal(void){
	RETURNGET(UInt8,OFFSET_NOMINAL);
}
void Effect::setNominal(UInt8 value){
	set(OFFSET_NOMINAL,&value,sizeof(value));
}

UInt8 Effect::getRemaining(void){
	RETURNGET(UInt8,OFFSET_REMAINING);
}
void Effect::setRemaining(UInt8 value){
	set(OFFSET_REMAINING,&value,sizeof(value));
}

Character* Effect::getOwner(void){
	// Get id code of owner
	UInt32 idOwner;
	get(OFFSET_OWNER, &idOwner, SIZE_OWNER);
	// Get reference to that character's object
	if (idOwner == getID()) return NULL; // no owner if it owns itself
	
	Character* owner = (Character*) queryPerformerPfromID(idOwner);
	if ( owner == NULL ) // owner is not itself, but ID was not found in database
		setOwner(NULL);

	return owner;
}
void Effect::setOwner(Character* owner){
	// if no owner, then it owns itself
	UInt32 idOwner = (owner==NULL)? getID() : owner->getID(); 
	set(OFFSET_OWNER,&idOwner,SIZE_OWNER);
}

Boolean Effect::isPC(void){
	return false;
}

void Effect::addSummoned(Performer* summoned) {
	Err e;
	MemHandle h;
	MemPtr p;
	UInt32 id;

	// determine which database entry to modify
	UInt16 iEncounter = getIEncounter();

	// determine old record size
   	h = DmGetRecord(dbEncounter,iEncounter);
	UInt32 oldRecSize = MemHandleSize(h);
	DmReleaseRecord(dbEncounter,iEncounter,false);

	// calculate new record size, by adding change in name sizes
	UInt32 newRecSize = oldRecSize + sizeof(id);

	// Resize record. 
	h = DmResizeRecord(dbEncounter,iEncounter,newRecSize);
	if (h==NULL) {
		ErrDisplay("Not enough room for resized record.");
		return;
	}
	p = MemHandleLock(h);
	
	// increment number of Summoned creatures
	UInt8 count;
	MemMove(&count,(Byte*)p + OFFSET_N_SUMMONED,sizeof(count));
	count++;
	e = DmWrite(p, OFFSET_N_SUMMONED, &count, sizeof(count));
	ErrFatalDisplayIf(e!=errNone,"Problem writing summoned creature count to record.");

	// write ID of new summoned creature, at appropriate location
	id = summoned->getID();
	UInt32 offset = OFFSET_NAME_EFFECT;
	offset += getNameSize();
	offset += sizeof(id) * (count - 1);
	e = DmWrite(p, offset, &id, sizeof(id));
	ErrFatalDisplayIf(e!=errNone,"Problem writing new summoned creature ID to record.");

	// release memory
	MemHandleUnlock(h);
	
	DmReleaseRecord(dbEncounter,iEncounter,true);
}

Performer* Effect::removeSummoned(void) {
	Performer* result = NULL;
	Err e;
	MemHandle h;
	MemPtr p;
	UInt32 id;
	UInt8 count;

	// determine which database entry to modify
	UInt16 iEncounter = getIEncounter();

	// get record
   	h = DmGetRecord(dbEncounter,iEncounter);
	p = MemHandleLock(h);	
	
	// get number of Summoned creatures
	MemMove(&count,(Byte*)p + OFFSET_N_SUMMONED,sizeof(count));

	// check for empty list
	if (count <= 0) {
		MemHandleUnlock(h);
		DmReleaseRecord(dbEncounter,iEncounter,false);
		return NULL;
	}
	
	// get last summoned creature
	UInt32 offset = OFFSET_NAME_EFFECT;
	offset += getNameSize();
	offset += sizeof(id) * (count - 1);
	MemMove(&id,(Byte*)p + offset,sizeof(id));
	result = queryPerformerPfromID(id);
	
	// determine old record size
	UInt32 oldRecSize = MemHandleSize(h);

	// release record, because resizing it could move it
	MemHandleUnlock(h);	

	// calculate new record size, by subtracting change in name sizes
	UInt32 newRecSize = oldRecSize - sizeof(id);

	// Resize record. 
	h = DmResizeRecord(dbEncounter,iEncounter,newRecSize);
	if (h==NULL) {
		ErrDisplay("Not enough room for resized record.");
		DmReleaseRecord(dbEncounter,iEncounter,false);
		return NULL;
	}
	p = MemHandleLock(h);
	
	// decrement number of Summoned creatures, now that resizing worked
	count--;
	e = DmWrite(p, OFFSET_N_SUMMONED, &count, sizeof(count));
	ErrFatalDisplayIf(e!=errNone,"Problem writing summoned creature count to record.");

	// release memory
	MemHandleUnlock(h);
	
	DmReleaseRecord(dbEncounter,iEncounter,true);

	// if result==NULL, assume it is because summoned creature was deleted from database
	// and instead remove the next one
	if (result == NULL)
		if (count == 0)
			// this is what would happen on the next recursion anyway...
			return NULL;
		else
			// remove the next one recursively
			return removeSummoned();
	else
		return result;
	
}
