/*************************************************
 *
 * Definitions used only in the .rcp file
 *
 *************************************************/

// number of pixels to assign for font 0 fields
#define fieldCharWidth      5
// number of pixels to assign for font 1 fields
#define fieldCharWidthBold  7
// the X position of the center line for the screen
#define centerX             80

/*************************************************
 *
 * Definitions used in both the .rcp and .c files
 *
 * 1### Forms
 * 10## non-modal forms
 * 11## modal forms
 * 19## alerts
 * 2### controls
 * 20## buttons
 * 21## menu bars
 * 22## menu buttons
 * 23## fields
 * 24## labels
 * 25## tables
 * 26## lists
 * 27## check boxes
 * 3### Strings
 *
 *************************************************/
#define CREATOR_ID          'PMcq'

// Maximum number of bytes in Performer name fields
#define MAX_NAME_BYTES		255

// Maximum number of bytes in numeric fields
#define MAX_NUMERIC_BYTES	11    // maxStrIToALen = 12

// Generic Done button ID
#define DoneID				2001
// Generic Cancel button ID
#define CancelID			2002
// Buttons for scrolling up and down
#define ScrollUpID			2003
#define ScrollDownID		2004
// When there is exactly one table on a form
#define TableID				2501

/*************************************************
 * Version Number
 *************************************************/
#define	VersionID	1000

/*************************************************
 * Old OS Alert
 *************************************************/
#define AlertOldOSID        1901

/*************************************************
 * Overview Form
 *************************************************/
#define NUM_ROWS			10
#define NUM_COLUMNS			4
#define OverviewFormID      1001

#define RoundFieldID		2301
#define InitiativeFieldID	2302

#define	TrialAlertID	1918
#define	NoCombatantsErrorID	1904
#define NoneSelectedID		1915

#define	NewCharBID			2005
#define DetailBID			2006
#define StartCombatBID		2007

// Menu and buttons
#define OverviewMenuBarID   2101
#define NewCharID			2201
#define DeleteID			2203
#define SortTableID			2204
#define	CopyItemID			2205

#define StartCombatID		2220
#define EndCombatID			2221

#define HowToID            2251
#define AboutID            2252
#define OGLID              2253
// help file
#define OverviewHelpString 3001

/*************************************************
 * Roll Initiative Form
 *************************************************/
#define RollInitFormID		1002
#define ROLL_ROWS			10
#define ROLL_COLS		    3

 /*************************************************
 * Generic Text Display Form
 *************************************************/
#define TextFormID          1101
// Field to display text in
#define TextFormFieldID     2301

/*************************************************
 * Logo and Info display Form
 *************************************************/
#define LogoFormID          1102
#define	LogoFormVersionID	1000

/*************************************************
 * various string resources, for use with TextFormID
 *************************************************/
// Open Game License
#define StringOGLID         3002
#define StringOGLTitleID	3015
// About...
#define StringAboutID       3003
#define	StringAboutTitleID	3014

/*************************************************
 * Play Form
 *************************************************/
#define	PlayFormID				1003
// errors
#define	TieAlertID				1907
#define InitTieHelpString		3019
#define EncounterEndAlertID 	1908
#define	EncounterEndStringID	3009
// buttons
#define	CharNormalBID			2007
#define	CharStandardBID			2008
#define	CharCreateEffectBID		2009
#define	CharDelayBID			2010
#define	CharReadyBID			2011
#define	EffectContinueBID		2013
#define	KillBID					2014
#define	InterruptBID			2015
#define	TriggerBID				2016
// menu items
#define	PlayFormMenuID			2100
#define	CharActMID				2200
#define	CharCreateEffectMID		2201
#define	CharDelayMID			2203
#define	CharReadyMID			2204
#define	CharRefocusMID			2205
#define	EffectContinueMID		2206
#define	DieMID					2207
#define	KillMID					2208
#define	InterruptMID			2209
#define	TriggerMID				2210
#define	OverviewMID				2211
#define	EndMID					2214
//#define	EditMID					2215
//#define	UndoMID					2212
//#define	RedoMID					2213
// labels
#define	EffectDurationLabelID	2404
// strings
#define	SurpriseRoundString	3012
#define	EffectEndingString	3013
#define PlayHelpString 3016
/*************************************************
 * Delay Form
 *************************************************/
#define DelayFormID 			1104
// alert
#define	InitOutOfRangeErrorID	1909
// output fields
#define	MaxDelayFieldID			2301
// field
#define	DelayInitFieldID 		2302
// new button
#define	NoneBID					2005
// help string
#define DelayHelpString		3020
/*************************************************
 * Select Form(s)
 *************************************************/
#define SelectFormID 				1105
#define SelectListID 				2600
#define	SelectTriggerStringID		3006
#define	SelectKillStringID			3004
#define	SelectInterruptStringID		3005
#define	SelectTextFieldID			2303
#define	NoDelayingCharactersAlertID	1910
#define NoSelectionsAlertID 		1911
#define	NoOtherPerformersAlertID	1912
#define NoReadiedCharactersAlertID 	1913
#define	ReadiedTriggeringAlertID	1914
/*************************************************
 * Edit Effect Form
 *************************************************/
#define	EditEffectFormID 			1106
#define	EffectSummonBID				2000
#define	OwnerFieldID 				2300
#define	NameFieldID					2304
#define	DurationFieldID				2302
#define	RemainingFieldID 			2303
#define	DefaultEffectNameStringID 	3007
#define NoOwnerStringID 			3008
#define NominalDurationAlertID		1921
#define RemainingDurationAlertID	1922
#define EditEffectHelpString		3017
/*************************************************
 * Edit Character Form
 *************************************************/
#define EditCharFormID 		1107
#define	DelayingCBID	2700
#define	ReadiedCBID		2701
#define	AwareCBID		2702
#define	TakingReadiedCBID	2703
#define PlayerCharacterCBID	2704
#define	CountFieldID		2303
#define	ModifierFieldID		2305
#define	InvalidModifierID	1916
#define	VerifyDeleteAlertID	1917
#define	SpellNumberAlertID	1919
#define	DefaultDurationAlertID	1920
#define	DefaultCharNameStringID 	3021
#define EditCharHelpString	3018

/*************************************************
 * Select Number Form
 *************************************************/
#define	PickNumberID	1108
#define	PickFieldID	2300
#define	D20ResultStringID	3010
#define	NCopyStringID	3011
// horizontal position of buttons
#define RollButtonsX 19
// buttons it displays - must be sequential!
#define	Roll1ButtonID	1000
#define	Roll2ButtonID	1001
#define	Roll3ButtonID	1002
#define	Roll4ButtonID	1003
#define	Roll5ButtonID	1004
#define	Roll6ButtonID	1005
#define	Roll7ButtonID	1006
#define	Roll8ButtonID	1007
#define	Roll9ButtonID	1008
#define	Roll10ButtonID	1009
#define	Roll11ButtonID	1010
#define	Roll12ButtonID	1011
#define	Roll13ButtonID	1012
#define	Roll14ButtonID	1013
#define	Roll15ButtonID	1014
#define	Roll16ButtonID	1015
#define	Roll17ButtonID	1016
#define	Roll18ButtonID	1017
#define	Roll19ButtonID	1018
#define	Roll20ButtonID	1019
//
// Autogenerated symbols - 2003-12-02 3:31:59 PM
// Falch.net PilRC Designer
//
// Autogenerated symbols - 2003-12-21 4:45:26 PM
// Falch.net PilRC Designer
//
//
// Autogenerated symbols - 2003-12-21 4:51:55 PM
// Falch.net PilRC Designer
//
//
