/*
 * Header file used by all modules in application.
 * Copyright 2017 Peter L Mikelsons
 */

#ifndef __MAIN_H__
#define __MAIN_H__

// PalmOptErrorCheckLevel.h defines this unless it is already defined
#define ERROR_CHECK_LEVEL ERROR_CHECK_FULL

#include <PalmOS.h>
// Library that allows using post-3.5 text manipulation functions to work on pre-3.5 platforms
#include <TxtGlue.h>		// Text manager glue routines.
// resource constant file
#include "MainRsc.h"

//#define DEMO // Demonstration version with reduced functionality

// These definitions place functions in separate code segments
#define PERF_SECTION __attribute__ ((section ("perfsect")))
#define MISC_SECTION __attribute__ ((section ("miscsect")))

/* Convenience Macro - get the amount of memory used by a Char*. */
#define StrSize(STRING) StrLen(STRING) + TxtGlueCharSize(chrNull)

/* Convenience Macro - Get pointer to an object on a form. */
#define GetObjectPtr(FORM,ID) FrmGetObjectPtr(FORM,FrmGetObjectIndex(FORM,ID))

/* Convenience Macro - get number of performers that are not deleted and not archived. */
#define PERF_COUNT DmNumRecordsInCategory(dbEncounter,dmAllCategories)

#ifdef __cplusplus
extern "C" {
#endif

// application info block for the Encounter database
typedef struct {
	Int32 round;
	UInt32 firstID;
	UInt32 currentID;
	UInt16 counterCharacterName;
} EncounterInfoType;

/* The number appended to generic character names. */
extern UInt16 counterCharacterName;

// Is this the first time this application has been launched?
extern Boolean firstExecution;
/* Currently open encounter database */
extern DmOpenRef dbEncounter;

// LogoForm.c
extern Boolean EventHandlerLogoForm(EventPtr event);

// OverviewForm.cpp
extern UInt16 swapUpdateCodeForMaxRecordIndex(UInt16 in);
extern void InitOverviewForm(void);
extern void StopOverviewForm(void);
extern Boolean EventHandlerOverviewForm(EventPtr event);

// Main.cpp
extern Int16 PickNumber(UInt16 stringID);
extern void FldSet(FormType* form, UInt16 fieldID, Char* text);
extern void FldSetI(FormType* form, UInt16 FieldID, Int32 i);
extern void FldSetS(FormType* form, UInt16 FieldID, UInt16 StringID);
extern void FldSetEditable(FieldType* field, Boolean editable);
extern Boolean StrIsI(char* string);
extern UInt16 TxtGetNumChar(const char* string);

// Performer.cpp
// Read encounter database and re-instantiate all performer objects  
extern void LoadPerformers(UInt16 dbVersion);
// Function used to sort performers
extern Int16 ComparePerformers(void *rec1, void *rec2,
							   Int16 other, SortRecordInfoPtr rec1SortInfo,
							   SortRecordInfoPtr rec2SortInfo,
							   MemHandle appInfoH);
// Pop up new form to edit and create new effect
extern void FrmPopupEditCharForm(Char* creatorName);

// RollInitForm.cpp
extern Boolean EventHandlerRollInitForm(EventPtr event);

// TextForm.c
extern void FrmGotoTextForm(UInt16 title, UInt16 stringID);
extern void FrmPopupTextForm(UInt16 title, UInt16 stringID);
extern Boolean EventHandlerTextForm(EventPtr event);

// PlayForm.cpp
extern Boolean EventHandlerPlayForm(EventPtr event);

// DelayForm.cpp
extern Boolean EventHandlerDelayForm(EventPtr event) MISC_SECTION;

// SelectForm.cpp
extern void FrmPopupTriggerForm(void);
extern void FrmPopupKillForm(void);
extern void FrmPopupInterruptForm(void);
extern Boolean EventHandlerSelectForm(EventPtr event);

// EditEffectForm.cpp
extern Boolean EventHandlerEditEffectForm(EventPtr event) MISC_SECTION;

// EditCharForm.cpp
extern Boolean EventHandlerEditCharForm(EventPtr event) MISC_SECTION;

#ifdef __cplusplus
}
#endif

#endif
