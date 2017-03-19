/*
 * Header file used by all modules in application that use non-OGC functions with OGC
 * parameters.
 * Copyright 2017 Peter L Mikelsons
 */

#ifndef __PERFORMER_H__
#define __PERFORMER_H__

// Current version number of encounter database format, should be UInt16
#define DB_VERSION 2

// A unique record ID that should never appear.
// (based on comments in Programmer's Companion that unique id is 3 bytes)
#define ID_MAXIMUM 0xFFFFFFFF 

// Get pointer to performer from ID code
extern Performer* queryPerformerPfromID(UInt32 id) PERF_SECTION;
// Send update event to Overview form, so that it displays the argument
extern void OverviewFormUpdate(Performer* p) PERF_SECTION;						   							   
// Pop up new form to edit and create new effect
extern void FrmPopupEditEffectForm(Character* owner) PERF_SECTION;
// Pop up new form to edit and create new summoned creature
extern void FrmPopupEditSummForm(Effect* owner) PERF_SECTION;

#endif
