#ifndef MENU_H_
#define MENU_H_

// Moved this here so it can be used with other menus (playerlist)
#define MENU_TEXT_SIZE 0.19f
#define MENU_SCREEN_BORDER 20
#define MENU_ITEM_SPACE 1

void initUserVars();
void drawMenu();

/*
==============================
eth menu

All ethVarsEnum_t enum must have an to be init in initUserVars()
==============================
*/

// All categories
typedef enum ethMenuCat_s {
	#ifdef ETH_PRIVATE
		PRIVATE_CATS
	#endif
	CAT_VISUAL,
	CAT_AIMBOT,
	CAT_ESP,
	CAT_MISC,
	CAT_IRC,
	CAT_SPYCAM,
	CAT_HUD,
	#ifdef ETH_DEBUG
		CAT_DEBUG,
	#endif
	CATS_MAX
} ethMenuCat_t;

typedef enum ethVarsEnum_s {
	// Aimbot 
	VAR_AIM_AUTO,
	VAR_AUTOSHOOT,
	VAR_AIM_PRIO,
	VAR_AIMFOV,
	VAR_VECZ,
	VAR_VECZ_STEN,
	VAR_AIMVECZ_TYPE,
	VAR_AIMPREDICT,
	VAR_AIMPREDICT_VALUE,
	VAR_SELFPREDICT,
	VAR_SELFPREDICT_VALUE,
	VAR_SNIPERDELAY,
	VAR_OVERHEAT,
	VAR_AIM_DEAD,
	VAR_BLOCK_MOUSE,
	VAR_SATCHEL_AUTOSHOOT,
	VAR_HUMAN,
	VAR_HUMANVALUE,
	VAR_ACTIONPRIOR,
	VAR_STEP,
	
	// Visual
	VAR_ADVERT,
	VAR_RADAR,
	VAR_SNIPERZOOM,
	VAR_RAIL,
	VAR_RAILAXIS,
	VAR_MORTARTRACE,
	VAR_SPAWNTIMER,
	VAR_PIGHEAD,
	VAR_SMOKE,

	// Esp
	VAR_WALLHACK,
	VAR_COLORS,
	VAR_CHAMS,
	VAR_ESPNAME,
	VAR_ESPNAME_BG,
	VAR_ESPENT,
	VAR_ESPICON,
	VAR_ESPICON_BG,

	// Misc
	VAR_RANDOMNAME,
	VAR_RANDOMNAME_DELAY,
	VAR_SPEC,
	VAR_SPECLOCK,
	VAR_INACTIVITY,
	VAR_AUTOVOTE,
	VAR_MEDICBOT,
	VAR_RENDER,
	VAR_REFLIST,
	VAR_ETHFIND,
	VAR_KILLSPAM,

	// Spycam
	VAR_MORTARCAM,
	VAR_PANZERCAM,
	VAR_SATCHELCAM,
	VAR_MIRRORCAM,

	// irc
	VAR_IRC,
	VAR_IRC_FT,
	VAR_IRC_FT_NICKS,
	VAR_IRC_TOPIC,
	VAR_IRC_INVITE,

	// Custom HUD

	VAR_CHUD,
	VAR_CHUD_SYS_UPDATEDELAY,
	VAR_CHUD_SYS_FPSUPDATEDELAY,
	VAR_CHUD_SYS_BAR_WIDTH,
	VAR_CHUD_SYS_BAR_HEIGHT,
	VAR_CHUD_OUTPUTNUM,
	VAR_CHUD_OUTICONSIZE,
	VAR_CHUD_LOG_FONTSIZE,
	VAR_CHUD_VICTIM_STAYTIME,
	VAR_CHUD_VICTIM_FADETIME,
	VAR_CHUD_HPOPUP_HEIGHT,
	VAR_CHUD_HPOPUP_STAYTIME,
	VAR_CHUD_CHAT_MSGS,
	VAR_CHUD_CHAT_STAYTIME,
	VAR_CHUD_CONSOLE_MSGS,
	VAR_CHUD_CONSOLE_STAYTIME,

	#ifdef ETH_DEBUG
		// Debug
		VAR_MEMDUMP,
		VAR_AIM_POINT,
		VAR_PREDICT_STATS,
	#endif
		
	#ifdef ETH_PRIVATE
		PRIVATE_VARS
	#endif

	VARS_TOTAL
} ethVarsEnum_t;

typedef enum {
	VECZ_ADD,
	VECZ_MA,
	VECZ_AUTO
} aimVecz_t;

typedef enum {
	AIM_POINT_OFF,
	AIM_POINT_2D,
	AIM_POINT_3D
} aimPoint_t;

typedef enum {
	AIM_PRIO_OFF,
	AIM_PRIO_HEAD,
	AIM_PRIO_HEAD_BODY,
	AIM_PRIO_HEAD_ONLY
} aimPriority_t;

typedef enum {
	AUTOSHOOT_OFF,
	AUTOSHOOT_LBUTTON,
	AUTOSHOOT_ON
} autoShoot_t;

typedef enum {
	AIMPREDICT_OFF,
	AIMPREDICT_AUTO,
	AIMPREDICT_AUTOB,
	AIMPREDICT_AUTOC,
	AIMPREDICT_MANUAL
} aimPredict_t;

typedef enum {
	SELFPREDICT_OFF,
	SELFPREDICT_AUTO,
	SELFPREDICT_MANUAL
} selfPredict_t;

typedef enum {
	A_AIMSHOOT,
	A_SHOOTAIM
} actionP_t;

typedef enum {
	HUD_OFF,
	HUD_1,
	HUD_2
} customHud_t;

typedef enum {
	SMOKE_NORMAL,
	SMOKE_TRANSPARENT,
	SMOKE_REMOVE
} smokeType_t;

typedef enum {
	REF_OFF,
	REF_LIST,
	REF_NUM
} refList_t;

/*
==============================
Menus structs
==============================
*/

// Vars choices
typedef struct choice_s {
	char *name;
	float value;
} choice_t;

typedef struct menuCat_s {
	ethMenuCat_t id;
	char *name;
} menuCat_t;

#define MAX_CHOICES 20
typedef struct ethVars_s {
	char *menuText;
	float defaultValue;
	char *cvarName;
	ethMenuCat_t menuCategory;
	qboolean forceDefault;	// Force cvar to default at load
	choice_t choices[MAX_CHOICES];
} ethVars_t;

#endif /*MENU_H_*/
