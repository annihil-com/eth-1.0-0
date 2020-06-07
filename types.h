#ifndef TYPES_H_
#define TYPES_H_

/*
==============================
structs use with et.x86
==============================
*/

typedef struct {
	byte ip[4];
	byte UNUSED[10];
	unsigned short port;
} netadr_t;

typedef struct fileInPack_s {
	char					*name;		// name of the file
	unsigned long			pos;		// file info position in zip
	struct	fileInPack_s*	next;		// next file in the hash
} fileInPack_t;

typedef void *unzFile;

typedef struct {
	char			pakFilename[MAX_OSPATH];	// c:\quake3\baseq3\pak0.pk3
	char			pakBasename[MAX_OSPATH];	// pak0
	char			pakGamename[MAX_OSPATH];	// baseq3
	unzFile			handle;						// handle to zip file
	int			checksum;					// regular checksum
	int			pure_checksum;				// checksum for pure
	int			numfiles;					// number of files in pk3
	int			referenced;					// referenced file flags
	int			hashSize;					// hash table size (power of 2)
	fileInPack_t*	*hashTable;					// hash table
	fileInPack_t*	buildBuffer;				// buffer with the filenames etc.
} pack_t;

typedef struct searchpath_s {
	struct searchpath_s *next;
	pack_t *pack;
} searchpath_t;

/*
==============================
eth structs
==============================
*/

// For all console actions/commands
#define MAX_ACTION_LEN 128
typedef struct ethAction_s {
	int state;
	int defaultState;
	char startAction[MAX_ACTION_LEN];
	char stopAction[MAX_ACTION_LEN];
} ethAction_t;

typedef enum ethActions_s {
	ACTION_ATTACK,
	ACTION_BACKWARD,
	ACTION_BINDMOUSE1,
	ACTION_CROUCH,
	ACTION_JUMP,
	ACTION_PRONE,
	ACTION_RUN,
	ACTIONS_TOTAL // Number of actions
} ethActions_t;

// Sounds
typedef enum ethSounds_s {
	// Misc sound
	SOUND_HEADSHOT1,
	SOUND_HEADSHOT2,
	SOUND_HUMILIATION,
	SOUND_PREPARE,
	// Spree sound
	// Level 1
	SOUND_DOUBLEKILL1,
	SOUND_EXCELLENT,
	SOUND_IMPRESSIVE,
	SOUND_KILLINGSPREE1,
	SOUND_KILLINGSPREE2,
	SOUND_PERFECT,
	// Level 2
	SOUND_GODLIKE1,
	SOUND_GODLIKE2,
	SOUND_HOLYSHIT,
	SOUND_MULTIKILL1,
	SOUND_MULTIKILL2,
	SOUND_MULTIKILL3,
	SOUND_TRIPLEKILL,
	// Level 3
	SOUND_DOMINATING1,
	SOUND_DOMINATING2,
	SOUND_ULTRAKILL1,
	SOUND_ULTRAKILL2,
	SOUND_ULTRAKILL3,
	// Level 4
	SOUND_MONSTERKILL1,
	SOUND_MONSTERKILL2,
	SOUND_UNREAL,
	SOUND_UNSTOPPABLE1,
	SOUND_UNSTOPPABLE2,
	SOUND_WICKEDSICK,
	SOUNDS_TOTAL
} ethSounds_t;

/*
==============================
multi-game version support struct
==============================
*/

typedef struct ethET_s {
	char *version;
	u_int crc32;
	u_int32_t Cvar_Set2;
	u_int32_t Cmd_AddCommand;
	u_int32_t COM_StripExtension;	// Only need before 2.60b - http://www.milw0rm.com/exploits/1750
	u_int32_t FS_PureServerSetLoadedPaks;
	u_int32_t fs_searchpaths;
	u_int32_t serverIP;
	u_int32_t cl_mouseDx;
	u_int32_t cl_mouseDy;
	#ifdef ETH_PRIVATE
		PRIV_ET_SUPPORT
	#endif
} ethET_t;

/*
==============================
multi-mod support structs
==============================
*/

// cgame cg struct
typedef struct eth_cg_s {
	refdef_t *refdef;
	float *refdefViewAngles;
} eth_cg_t;

// cgame centity struct
typedef struct eth_cg_entities_s {
	entityState_t *currentState;
	qboolean *currentValid;
	refEntity_t pe_bodyRefEnt;
} eth_centity_t;

typedef struct ethMod_s {
	char *name;
	char *version;
	u_int crc32;

	// Offset from cgame lib
	int cg;
	int cg_entities;
	int centity_t_size;

	int CG_Trace;
	int BG_EvaluateTrajectory;
	int CG_EntityEvent;
	int CG_FinishWeaponChange;
	int CG_DamageFeedback;
	int CG_RailTrail2;
	
	// Offset from cg
	int cg_refdef;
	int cg_refdefViewAngles;

	// for etpro
	int guid_function;
} ethMod_t;

/*
==============================
players structs
==============================
*/

typedef enum ethGfxColors_s {
	GFX_NONE,
	GFX_RED,
	GFX_BLUE,
	GFX_GREEN,
	GFX_CYAN,
	GFX_TEAL,
	GFX_YELLOW,
	GFXS_TOTAL
} ethGfxColors_t;

typedef enum ethChams_s {
	CHAM_GLOW = 1,
	CHAM_GLOWONLY,
	CHAM_FLAT,
	CHAM_QUAKE,
	CHAM_QUAKEB,
	CHAM_QUAKEC,
	CHAMS_TOTAL
} ethChams_t;

typedef struct ethColorModel_s {
	ethGfxColors_t chams;
	float *esp;	// vec3_t
} ethColorModel_t;

typedef struct playerColor_s {
	ethColorModel_t friendVisible;
	ethColorModel_t friendNotVisible;
	ethColorModel_t enemyVisible;
	ethColorModel_t enemyNotVisible;
} ethPlayerColor_t;

typedef struct ethClientInfo_s {
	qboolean infoValid;
	char name[MAX_QPATH];
	char cleanName[MAX_QPATH];
	team_t team;
	int cls;
//	int rank;
//	int weapon;

	int targetType;	// For Teamkillers / Friends
} ethClientInfo_t;

// All entity infos
typedef struct ethEntity_s {
	qboolean isValid;
	qboolean isInScreen;
	qboolean isVisible;	// true if body or head is visible
	qboolean isHeadVisible;
	
	float *espColor;	// vec4_t
	int screenX, screenY;
	float distance;

	vec3_t origin;		// Body origin
	vec3_t bodyPart;	// vec3_origin if no body parts visible
	vec3_t head;

	vec3_t headAxis[3];
} ethEntity_t;

/*
==============================
 Enemy/Friend player list selector
==============================
*/

typedef enum {
	PLIST_UNKNOWN,
	PLIST_TEAMKILLER,
	PLIST_FRIEND,
	PLIST_MAX
} playerList_t;

/*
==============================
 Custom HUD stuff
==============================
*/

typedef enum hudFonts_s {
	FONT_BERSOLID,
	FONT_COMIC,
// 	FONT_CREEPY,
// 	FONT_PALETTEN,
	FONT_SPACETOA,
// 	FONT_TEAMBD
	FONT_MAX
} hudFontList_t;

/*
==============================
eth main structs
==============================
*/

// 'Static' eth struct. never zero'ed.
typedef struct seth_s {
	ethVars_t vars[VARS_TOTAL];
	float value[VARS_TOTAL];

	// Store players spec'ing you
	char specNames[MAX_CLIENTS][MAX_QPATH];
	int specCount;

	// Initialize windows only once
	qboolean windowsInitialized;

	// IRC chan topic
	char ircTopic[IRC_TOPIC_SIZE];

	// IRC server invites
	int lastInvite;
	int lastInviteID;

	// Store referees
	char refNames[MAX_CLIENTS][MAX_QPATH];
	int refCount;
} seth_t;

// Main eth struct - zero'ed every CG_INIT
typedef struct eth_s {
	qboolean hookLoad;

	// cgame lib infos
	char *cgameFilename;
	void *cgameLibHandle;
	void *cgameLibAddress;
	ethMod_t mod;
	
	// ui lib infos
	void *uiLibHandle;

	server_t server;
	qboolean punkbusterState;

	// cgame vars/functions
	eth_centity_t cg_entities[MAX_GENTITIES];
	// cg
	eth_cg_t cg;
	int cg_time;
	snapshot_t *cg_snap;
	int cg_clientNum;
	int cg_redlimbotime;
	int cg_bluelimbotime;
	int cg_frametime;
	// cgs
	glconfig_t cgs_glconfig;
	gameState_t *cgs_gameState;
	int cgs_levelStartTime;
	int cgs_aReinfOffset[TEAM_NUM_TEAMS];
	float cgs_screenXScale;
	float cgs_screenYScale;
	qhandle_t cgs_media_whiteShader;
	qhandle_t cgs_media_cursorIcon;
	qhandle_t cgs_media_reticleShaderSimple;
	qhandle_t cgs_media_binocShaderSimple;
	qhandle_t cgs_media_smallgunBrassModel;
	qhandle_t cgs_media_railCoreShader;
	qhandle_t cgs_media_sndLimboSelect;
	qhandle_t cgs_media_smokePuffshader;
	qhandle_t bg_alliedpin;
	qhandle_t bg_axispin;
	gamestate_t cgs_gamestate;
	// functions
	void (*CG_Trace)(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask);	
	void (*BG_EvaluateTrajectory)(const trajectory_t *tr, int atTime, vec3_t result, qboolean isAngle, int splinePath);
	void *(*CG_EntityEvent)(centity_t *cent, vec3_t position);
	void (*CG_FinishWeaponChange)(int lastweap, int newweap);
	void (*CG_DamageFeedback)(int, int, int);
	void (*CG_RailTrail2)(clientInfo_t *ci, vec3_t start, vec3_t end);

	// Fonts
	fontInfo_t fontFixed;
	fontInfo_t fontVariable;

	// Zero'ed every CG_DRAW_ACTIVE_FRAME
	ethEntity_t entities[MAX_GENTITIES];
	refEntity_t refEntities[MAX_GENTITIES];
	int refEntitiesCount;

	// All custom shaders
	qhandle_t friendVisibleShader;
	qhandle_t friendNotVisibleShader;
	qhandle_t enemyVisibleShader;
	qhandle_t enemyNotVisibleShader;
	vec3_t friendVisibleColor;
	vec3_t friendNotVisibleColor;
	vec3_t enemyVisibleColor;
	vec3_t enemyNotVisibleColor;

	// All custom sounds
	sfxHandle_t sounds[SOUNDS_TOTAL];
	ethAction_t actions[ACTIONS_TOTAL];
	ethClientInfo_t clientInfo[MAX_CLIENTS];
	
	// cgame models
	qhandle_t hAmmoPack;
	qhandle_t hMedicPack;
	qhandle_t hHead;

	// Esp icons
	qhandle_t weaponIcons[WP_NUM_WEAPONS];
	qhandle_t classIcons[NUM_PLAYER_CLASSES];
	qhandle_t rankIcons[NUM_EXPERIENCE_LEVELS];
	qhandle_t number[10];
	
	// Shaders
	qhandle_t shaders[GFXS_TOTAL][CHAMS_TOTAL];
	qhandle_t whiteFlatShader;
	qhandle_t friendMortarShader;
	qhandle_t enemyMortarShader;

	// Custom model/skin
	qhandle_t pigHeadModel;
	qhandle_t pigHeadSkin;

	// Medicbot
	int lastMedicRequest;
	int lastMedicRequestTime;

	// Stats
	int killCount;
	int killSpreeCount;
	int firstKillSpreeTime;
	int lastKillTime;
	int spreelevel;
	int startFadeTime;
	int s_level;
	int killCountNoDeath; // For killspam

	// Menu
	int mouseX, mouseY, mouseButton;
	int isMenuOpen;

	// Playerlist
	int isPlistOpen;
	
	#ifdef ETH_DEBUG
		// Debug view stuff
		int offsetMul;
		int offsetSub;
	#endif

	// HUD
	int reloading;
	int reloadScale;
	int currentTxt;
	int currentChat;
	char VictimName[MAX_QPATH];

	int nameCounter; // TODO: what that ?

	// Custom HUD

	fontInfo_t hFont[FONT_MAX - 1];
	qhandle_t wIcon;
	qhandle_t iIcon;
	qhandle_t gStar;
	qhandle_t cStar;

	int cg_covertopsChargeTime[2];
	int cg_engineerChargeTime[2];
	int cg_soldierChargeTime[2];
	int cg_medicChargeTime[2];
	int cg_ltChargeTime[2];

	qboolean demoPlayback;

	//windowing support
	int pointer;
	int winInUse;
} eth_t;

/*
==============================
Target filters
==============================
*/
// player
#define TARGET_PLAYER			(1<<0)
#define TARGET_PLAYER_VULN		(1<<1)
#define TARGET_PLAYER_ALIVE		(1<<2)
#define TARGET_PLAYER_DEAD		(1<<3)
// player aim target
#define TARGET_PLAYER_HEAD		(1<<4)
#define TARGET_PLAYER_BODY		(1<<5)
// missile common
#define TARGET_MISSILE			(1<<7)
#define TARGET_MISSILE_ARMED	(1<<8)
#define TARGET_MISSILE_NOTARMED	(1<<9)
// missile
#define TARGET_MISSILE_DYNAMITE	(1<<10)
#define TARGET_MISSILE_LANDMINE	(1<<11)
#define TARGET_MISSILE_ROCKET	(1<<12)
// common
#define TARGET_ENEMY			(1<<13)
#define TARGET_FRIEND			(1<<14)
#define TARGET_VISIBLE			(1<<15)
#define TARGET_NOTVISIBLE		(1<<16)
// helper
#define TARGET_PLAYER_HEAD_BODY TARGET_VISIBLE
#define TARGET_PLAYER_ENEMY		(TARGET_PLAYER | TARGET_PLAYER_VULN | TARGET_ENEMY)
/*
==============================
Some filter macro
==============================
*/
//		&& eth.cg_entities[entityNum].currentValid

#define IS_PLAYER_VALID(entityNum) ((entityNum < MAX_CLIENTS) \
		&& (eth.cg_entities[entityNum].currentState->eType == ET_PLAYER) \
		&& eth.clientInfo[entityNum].infoValid \
		&& eth.entities[entityNum].isValid \
		&& (eth.cg_clientNum != entityNum))

#define IS_PLAYER_VULNERABLE(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (!(eth.cg_entities[entityNum].currentState->powerups & (1 << PW_INVULNERABLE))))

#define IS_PLAYER_ENEMY(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (((eth.clientInfo[entityNum].team != eth.clientInfo[eth.cg_clientNum].team) \
				&& (eth.clientInfo[entityNum].targetType != PLIST_FRIEND)) \
			|| (eth.clientInfo[entityNum].targetType == PLIST_TEAMKILLER)))

#define IS_PLAYER_DEAD(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->eFlags & EF_DEAD))

/*#define IS_PLAYER_PRONE(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (eth.cg_entities[entityNum].currentState.eFlags & EF_PRONE))

#define IS_PLAYER_CROUCH(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (eth.cg_entities[entityNum].currentState.eFlags & EF_CROUCHING))*/

// grenade, dynamite, landmine, satchel, ... are missiles
#define IS_MISSILE(entityNum) ((*eth.cg_entities[entityNum].currentValid) \
		&& (eth.cg_entities[entityNum].currentState->eType == ET_MISSILE))

#define IS_MISSILE_ARMED(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->teamNum < 4))

#define IS_MISSILE_NOTARMED(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->teamNum > 4))

#define IS_MISSILE_ENEMY(entityNum) (IS_MISSILE(entityNum) \
		&& (IS_MISSILE_NOTARMED(entityNum) ? \
			((eth.cg_entities[entityNum].currentState->teamNum - 4) != eth.clientInfo[eth.cg_snap->ps.clientNum].team) : \
			(eth.cg_entities[entityNum].currentState->teamNum != eth.clientInfo[eth.cg_snap->ps.clientNum].team)))

#define IS_DYNAMITE(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->weapon == WP_DYNAMITE))

#define IS_LANDMINE(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->weapon == WP_LANDMINE))

// Grenade: axis = LAUNCHER and allies = PINEAPPLE
#define IS_GRENADE(entityNum) (IS_MISSILE(entityNum) \
		&& ((eth.cg_entities[entityNum].currentState->weapon == WP_GRENADE_LAUNCHER) \
			|| (eth.cg_entities[entityNum].currentState->weapon == WP_GRENADE_PINEAPPLE)))

#define IS_MORTAR(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->weapon == WP_MORTAR_SET))

#define IS_PANZER(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->weapon == WP_PANZERFAUST))

#define IS_SATCHEL(entityNum) (IS_MISSILE(entityNum) \
		&& (eth.cg_entities[entityNum].currentState->weapon == WP_SATCHEL))

#endif /*TYPES_H_*/
