#include "eth.h"

// Categories text definitions
menuCat_t menuCats[] = {
	#ifdef ETH_PRIVATE
		PRIVATE_CATS_NAME
	#endif
	{ CAT_AIMBOT,	"--- AIMBOT ---" },
	{ CAT_VISUAL,	"--- VISUAL ---" },
	{ CAT_ESP,		"--- ESP ---" },
	{ CAT_MISC,		"--- MISC ---" },
	{ CAT_SPYCAM,	"--- SPYCAM ---" },
	{ CAT_IRC,		"--- IRC ---" },
	{ CAT_HUD,		"--- HUD ---" },
	#ifdef ETH_DEBUG
		{ CAT_DEBUG,	"--- DEBUG ---" }
	#endif
};

void initUserVars() {
	// Aimbot 
	
	seth.vars[VAR_AIM_AUTO] = (ethVars_t) { "Auto aim", qfalse, "aimauto", CAT_AIMBOT, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_AUTOSHOOT] = (ethVars_t) { "Auto shoot", qtrue, "autoshoot", CAT_AIMBOT, qfalse,
		{ { "Off", AUTOSHOOT_OFF }, { "Left button", AUTOSHOOT_LBUTTON }, { "On", AUTOSHOOT_ON } } };
	seth.vars[VAR_AIM_PRIO] = (ethVars_t) { "Aim priority", AIM_PRIO_HEAD, "aimprio", CAT_AIMBOT, qfalse,
		{ { "Off", AIM_PRIO_OFF }, { "Head", AIM_PRIO_HEAD }, { "Head/body" , AIM_PRIO_HEAD_BODY }, { "Head only" , AIM_PRIO_HEAD_ONLY } } };
	seth.vars[VAR_AIMFOV] = (ethVars_t) { "Aimbot FOV", 360.0f, "aimfov", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_VECZ] = (ethVars_t) { "All weapons vecZ", 2.0f, "vecz", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_VECZ_STEN] = (ethVars_t) { "Sten vecZ", 3.0f, "vecz_sten", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_AIMVECZ_TYPE] = (ethVars_t) { "Aimvecz type", qfalse, "vecztype", CAT_AIMBOT, qfalse,
		{ { "Add vecZ", VECZ_ADD }, { "VectorMA vecZ", VECZ_MA }, { "Auto vecZ", VECZ_AUTO } } };
	seth.vars[VAR_AIMPREDICT] = (ethVars_t) { "Enemy prediction type", qfalse, "epredict", CAT_AIMBOT, qfalse,
		{ { "Off", AIMPREDICT_OFF }, { "Auto", AIMPREDICT_AUTO }, { "Manual", AIMPREDICT_MANUAL } , { "AUTO B", AIMPREDICT_AUTOB }, { "AUTO C", AIMPREDICT_AUTOC } } };
	seth.vars[VAR_AIMPREDICT_VALUE] = (ethVars_t) { "Enemy predict value", 0.0f, "epredict_value", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_SELFPREDICT] = (ethVars_t) { "Self prediction type", qfalse, "spredict", CAT_AIMBOT, qfalse,
		{ { "Off", SELFPREDICT_OFF }, { "Auto", SELFPREDICT_AUTO }, { "Manual", SELFPREDICT_MANUAL } } };
	seth.vars[VAR_SELFPREDICT_VALUE] = (ethVars_t) { "Self predict value", 0.0f, "spredict_value", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_SNIPERDELAY] = (ethVars_t) { "Sniper weapon delay", 700, "sniperdelay", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_OVERHEAT] = (ethVars_t) { "Anti weapon overheat", qfalse, "overheat", CAT_AIMBOT, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_AIM_DEAD] = (ethVars_t) { "Aim at dead player", qfalse, "aimdead", CAT_AIMBOT, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_BLOCK_MOUSE] = (ethVars_t) { "Block mouse when aiming", qfalse, "block_mouse", CAT_AIMBOT, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_SATCHEL_AUTOSHOOT] = (ethVars_t) { "Satchel autoshoot", qfalse, "satchel_autoshoot", CAT_AIMBOT, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_HUMAN] = (ethVars_t) { "Human aim", qfalse, "human", CAT_AIMBOT, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_HUMANVALUE] = (ethVars_t) { "Human value", 0.2f, "humanvalue", CAT_AIMBOT, qfalse,
		{} };
	seth.vars[VAR_ACTIONPRIOR] = (ethVars_t) { "Action Priority", A_SHOOTAIM, "aprior", CAT_AIMBOT, qfalse,
		{ { "Aim then Fire", A_AIMSHOOT }, { "Fire then Aim", A_SHOOTAIM } } };
	seth.vars[VAR_STEP] = (ethVars_t) { "Add/dec by value", 1.0f , "step", CAT_AIMBOT, qfalse,
		{ { "10.0", 10.0f }, { "1.0", 1.0f }, { "0.1", 0.1f }, { "0.01", 0.01f } } };
	
	// Visual
	
	seth.vars[VAR_ADVERT] = (ethVars_t) { "Advert", qtrue, "advert", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_RADAR] = (ethVars_t) { "Radar", qfalse, "radar", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "Huge", 1 }, { "Big", 2 }, { "Medium", 3 }, { "Small", 4 }, { "Big 2", 5 }, { "Medium2", 6 }, { "Small2", 7 } } };
	seth.vars[VAR_SNIPERZOOM] = (ethVars_t) { "Remove sniper zoom", 1, "sniperzoom", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_RAIL] = (ethVars_t) { "RailTrail to enemy", qfalse, "rail", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_RAILAXIS] = (ethVars_t) { "RailTrail ent axis", qfalse,"railaxis", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_MORTARTRACE] = (ethVars_t) { "Mortar trace", qfalse,"mortartrace", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_SPAWNTIMER] = (ethVars_t) { "Enemy spawntimer", qfalse, "spawntimer", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_PIGHEAD] = (ethVars_t) { "Pig enemy head", qfalse, "pighead", CAT_VISUAL, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_SMOKE] = (ethVars_t) { "Smoke", SMOKE_TRANSPARENT, "smoke", CAT_VISUAL, qfalse,
		{ { "Leave normal", SMOKE_NORMAL }, { "Make transparent", SMOKE_TRANSPARENT }, { "Remove completely", SMOKE_REMOVE } } };

	// Esp
	
	seth.vars[VAR_WALLHACK] = (ethVars_t) { "Wallhack", qtrue, "wallhack", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_COLORS] = (ethVars_t) { "Colors", 1, "colors", CAT_ESP, qfalse,
		{ { "Type 1", 0 }, { "Type 2", 1 }, { "Type 3", 2 } } };
	seth.vars[VAR_CHAMS] = (ethVars_t) { "Chams", CHAM_QUAKE, "chams", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "Glow", CHAM_GLOW }, { "Glow only", CHAM_GLOWONLY }, { "Flat", CHAM_FLAT } , { "QuakeIII A", CHAM_QUAKE } , { "QuakeIII B", CHAM_QUAKEB }, { "QuakeIII C", CHAM_QUAKEC } } };
	seth.vars[VAR_ESPNAME] = (ethVars_t) { "Players name", qtrue, "espname", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "Game color", 1 }, { "Unicolor", 2 } } };
	seth.vars[VAR_ESPNAME_BG] = (ethVars_t) { "Players name background", qfalse, "espname_bg", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_ESPENT] = (ethVars_t) { "Entity ESP", qtrue, "espent", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_ESPICON] = (ethVars_t) { "Players icons", qtrue, "espicon", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "Dist scaled", 1 }, { "Static size", 2 } } };
	seth.vars[VAR_ESPICON_BG] = (ethVars_t) { "Players icons background", qfalse, "espicon_bg", CAT_ESP, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };

	// Misc

	seth.vars[VAR_RANDOMNAME] = (ethVars_t) { "Random name", qfalse, "randomname", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "From other servers", 1 }, { "From other players", 2 } } };
	seth.vars[VAR_RANDOMNAME_DELAY] = (ethVars_t) { "Random name delay", 30, "randomname_delay", CAT_MISC, qtrue,
		{ { "2s", 2 }, { "5s", 5 }, { "10s", 10 }, { "15s", 15 }, { "20s", 20 }, { "30s", 30 }, { "45s", 45 }, { "1min", 60 }, { "2min", 60 * 2 } , { "3min", 60 * 3 } , { "5min", 60 * 5 } } };
	seth.vars[VAR_SPEC] = (ethVars_t) { "Show spectators", qfalse, "spec", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_SPECLOCK] = (ethVars_t) { "Remove spectator locking", qtrue, "speclock", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue }} };
	seth.vars[VAR_INACTIVITY] = (ethVars_t) { "Remove inactivity kick", qtrue, "inactivity", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_AUTOVOTE] = (ethVars_t) { "Auto voting", qtrue, "autovote", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_MEDICBOT] = (ethVars_t) { "Medicbot", qfalse, "medicbot", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_RENDER] = (ethVars_t) { "Disable render (medicbot)", qfalse, "render", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_REFLIST] = (ethVars_t) { "Referee counter", REF_NUM, "ref", CAT_MISC, qfalse,
		{ { "Off", REF_OFF }, { "List with nicks", REF_LIST }, { "Number of refs only", REF_NUM } } };
	seth.vars[VAR_ETHFIND] = (ethVars_t) { "Find other ETH users", qtrue, "find", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_KILLSPAM] = (ethVars_t) { "Killspam", qfalse, "killspam", CAT_MISC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };

	// Spycam

	seth.vars[VAR_MORTARCAM] = (ethVars_t) { "Show mortar cam", qtrue, "mortarcam", CAT_SPYCAM, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_PANZERCAM] = (ethVars_t) { "Show panzer cam", qtrue, "panzercam", CAT_SPYCAM, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_SATCHELCAM] = (ethVars_t) { "Show satchel cam", qtrue, "satchelcam", CAT_SPYCAM, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_MIRRORCAM] = (ethVars_t) { "Show mirror cam", qfalse, "mirrorcam", CAT_SPYCAM, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };

	// IRC stuff

	seth.vars[VAR_IRC] = (ethVars_t) { "Connect irc bot", qfalse, "irc", CAT_IRC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_IRC_FT] = (ethVars_t) { "Show irc fireteam", qtrue, "irc_ft", CAT_IRC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_IRC_FT_NICKS] = (ethVars_t) { "Show nick & client ID in FT", qfalse, "irc_ft_nicks", CAT_IRC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_IRC_TOPIC] = (ethVars_t) { "Show irc topic", qfalse, "irc_topic", CAT_IRC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };
	seth.vars[VAR_IRC_INVITE] = (ethVars_t) { "Accept invites to join a server", qtrue, "irc_invite", CAT_IRC, qfalse,
		{ { "Off", qfalse }, { "On", qtrue } } };

	// Custom HUD

	seth.vars[VAR_CHUD] = (ethVars_t) { "Draw custom HUD", HUD_OFF, "chud", CAT_HUD, qfalse,
		{ { "Off", HUD_OFF }, { "Type 1", HUD_1 }, { "Type 2", HUD_2 } } };
	seth.vars[VAR_CHUD_SYS_UPDATEDELAY] = (ethVars_t) { "System info update delay", 5, "chud_sysupdelay", CAT_HUD, qfalse,
		{ { "1s", 1 }, { "3s", 3 }, { "5s", 5 }, { "10s", 10 }, { "20s", 20 } } };
	seth.vars[VAR_CHUD_SYS_FPSUPDATEDELAY] = (ethVars_t) { "FPS update delay", 1000, "chud_fpsupdelay", CAT_HUD, qfalse,
		{ { "250ms", 250 }, { "500ms", 500 }, { "750ms", 750 }, { "1s", 1000 } } };
	seth.vars[VAR_CHUD_SYS_BAR_WIDTH] = (ethVars_t) { "System bar Width", 80, "chud_sbar_width", CAT_HUD, qfalse,
		{ { "50", 50 }, { "60", 60 }, { "70", 70 }, { "80", 80 }, { "90", 90 }, { "100", 100 }, { "110", 110 }, { "120", 120 } } };
	seth.vars[VAR_CHUD_SYS_BAR_HEIGHT] = (ethVars_t) { "System bar Height", 8, "chud_sbar_height", CAT_HUD, qfalse,
		{ { "7", 7 }, { "8", 8 }, { "9", 9 }, { "10", 10 }, { "11", 11 }, { "12", 12 } } };
	seth.vars[VAR_CHUD_OUTPUTNUM] = (ethVars_t) { "Amount of log messages", 5, "chud_lognum", CAT_HUD, qfalse,
		{ { "3", 3 }, { "5", 5 }, { "7", 7 }, { "10", 10 } } }; // must be <= MAX_OUTPUT_MSGS
	seth.vars[VAR_CHUD_OUTICONSIZE] = (ethVars_t) { "Log icon size", 12, "chud_logiconsize", CAT_HUD, qfalse,
		{ { "8x8", 8 }, { "10x10", 10 }, { "12x12", 12 }, { "14x14", 14 }, { "16x16", 16 } } };
	seth.vars[VAR_CHUD_LOG_FONTSIZE] = (ethVars_t) { "Log font size", 0.12f, "chud_logfontsize", CAT_HUD, qfalse,
		{ { "0.10", 0.10f }, { "0.11", 0.11f }, { "0.12", 0.12f }, { "0.13", 0.13f }, { "0.14", 0.14f }, { "0.15", 0.15f } } };
	seth.vars[VAR_CHUD_VICTIM_STAYTIME] = (ethVars_t) { "'You killed..' msg stay time", 6, "chud_victim_staytime", CAT_HUD, qfalse,
		{ { "Off", 0 }, { "2s", 2 }, { "3s", 3 }, { "4s", 4 }, { "5s", 5 }, { "6s", 6 }, { "7s", 7 }, { "8s", 8 }, { "9s", 9 }, { "10s", 10 } } };
	seth.vars[VAR_CHUD_VICTIM_FADETIME] = (ethVars_t) { "'You killed..' msg fade time", 2, "chud_victim_fadetime", CAT_HUD, qfalse,
		{ { "Off", 0 }, { "250ms", 0.25f }, { "500ms", 0.5f }, { "750ms", 0.75f }, { "1s", 1 }, { "1.5s", 1.5f }, { "2s", 2 }, { "3s", 3 }, { "4s", 4 }, { "5s", 5 } } };
	seth.vars[VAR_CHUD_HPOPUP_HEIGHT] = (ethVars_t) { "Health popup height", 80, "chud_hpp_height", CAT_HUD, qfalse,
		{ { "50", 50 }, { "60", 60 }, { "70", 70 }, { "80", 80 }, { "90", 90 }, { "100", 100 }, { "110", 110 }, { "120", 120 } } };
	seth.vars[VAR_CHUD_HPOPUP_STAYTIME] = (ethVars_t) { "Health popup stay time", 4, "chud_hpp_staytime", CAT_HUD, qfalse,
		{ { "Off", 0 }, { "2s", 2 }, { "3s", 3 }, { "4s", 4 }, { "5s", 5 }, { "6s", 6 }, { "7s", 7 }, { "8s", 8 } } };
	seth.vars[VAR_CHUD_CHAT_MSGS] = (ethVars_t) { "Amount of chat messages", 5, "chud_chatnum", CAT_HUD, qfalse,
		{ { "3", 3 }, { "5", 5 }, { "7", 7 }, { "10", 10 } } }; // must be <= MAX_CHAT_MSGS
	seth.vars[VAR_CHUD_CHAT_STAYTIME] = (ethVars_t) { "Chat stay time", 10, "chud_chat_staytime", CAT_HUD, qfalse,
		{ { "Off", 0 }, { "6s", 6 }, { "8s", 8 }, { "12s", 12 }, { "16s", 16 }, { "20s", 20 }, { "25s", 25 } } };
	seth.vars[VAR_CHUD_CONSOLE_MSGS] = (ethVars_t) { "Amount of console messages", 5, "chud_connum", CAT_HUD, qfalse,
		{ { "3", 3 }, { "5", 5 }, { "7", 7 }, { "10", 10 } } }; // must be <= MAX_CONSOLE_MSGS
	seth.vars[VAR_CHUD_CONSOLE_STAYTIME] = (ethVars_t) { "Console msgs stay time", 10, "chud_con_staytime", CAT_HUD, qfalse,
		{ { "Off", 0 }, { "6s", 6 }, { "8s", 8 }, { "12s", 12 }, { "16s", 16 }, { "20s", 20 }, { "25s", 25 } } };

	#ifdef ETH_DEBUG

		// Debug

		seth.vars[VAR_MEMDUMP] = (ethVars_t) { "Display mem starting from:", qfalse, "memdump", CAT_DEBUG, qtrue,
			{ { "Off", qfalse }, { "CG_Trace()", 1 } , { "BG_EvalTraj()", 2 } , { "cg.time", 3 } , { "cg.refdef", 4 } , { "cg.refdefViewAngles", 5 } , { "vmMain()", 6 } , { "CL_MOUSEDX_ADDR", 7 } , { "eth.cg_snap->ps.origin", 8 } } };
		seth.vars[VAR_AIM_POINT] = (ethVars_t) { "Show aim point", qfalse, "aimpoint", CAT_DEBUG, qtrue,
			{ { "Off", AIM_POINT_OFF }, { "2D Point", AIM_POINT_2D } , { "3D RailTrail Cross", AIM_POINT_3D } } };
		seth.vars[VAR_PREDICT_STATS] = (ethVars_t) { "Show prediction stats", qfalse, "predict_stats", CAT_DEBUG, qtrue,
			{ { "Off", qfalse }, { "On", qtrue } } };

	#endif

	#ifdef ETH_PRIVATE
		PRIVATE_VARS_DEF
	#endif
}

void drawMenu() {
	// Get item height
	int itemHeight = sizeTextHeight(MENU_TEXT_SIZE, &eth.fontVariable) + (2 * 2);

	// Get max menu text width size
	int maxItemWidth = 0;
	int count;
	for (count = 0; count < VARS_TOTAL; count++) {
		int size = sizeTextWidth(MENU_TEXT_SIZE, seth.vars[count].menuText, &eth.fontVariable);
		if (size > maxItemWidth)
			maxItemWidth = size;
	}
	maxItemWidth += itemHeight;

	// Get max choice text width size
	int maxValueWidth = 0;
	for (count = 0; count < VARS_TOTAL; count++) {
		int i = 0;
		for (; i < MAX_CHOICES; i++) {
			int size = sizeTextWidth(MENU_TEXT_SIZE, seth.vars[count].choices[i].name, &eth.fontVariable);
			if (size > maxValueWidth)
				maxValueWidth = size;
		}
	}
	maxValueWidth += itemHeight;
	
	static int openCategory = -1;
	int nextOpenCategory = openCategory;
	int y = MENU_SCREEN_BORDER;
	int catCount = 0;
	for (; catCount < CATS_MAX; catCount++) {
		qboolean isMouseOn = ((eth.mouseX > MENU_SCREEN_BORDER)
				&& (eth.mouseX < (MENU_SCREEN_BORDER + maxItemWidth))
				&& (eth.mouseY > y)
				&& (eth.mouseY < (y + itemHeight)));

		// Sanity check
		if (!menuCats[catCount].name)
			fatalError("Can't find text for a menu category.");

		// Draw category
		drawFillRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? colorBlack : gameBackColor);
		drawRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? gameBackColor : gameBorderColor, 1);
		drawText(MENU_SCREEN_BORDER + (itemHeight / 2), y + 2, MENU_TEXT_SIZE, menuCats[catCount].name, isMouseOn ? gameFontColor : colorWhite, qfalse, qfalse, &eth.fontVariable);

		y += itemHeight + MENU_ITEM_SPACE;

		// Category click
		if (((eth.mouseButton == Button1) || (eth.mouseButton == Button3)) && isMouseOn) {
			eth.mouseButton = 0;
			if (nextOpenCategory != catCount)
				syscall_CG_S_StartLocalSound(eth.cgs_media_sndLimboSelect, CHAN_LOCAL_SOUND);
			nextOpenCategory = catCount;
		}

		// Draw vars only for this category
		if (openCategory != catCount)
			continue;

		// Search vars in this category
		int varCount = 0;
		for (; varCount < VARS_TOTAL; varCount++) {
			if (menuCats[catCount].id != seth.vars[varCount].menuCategory)
				continue;

			ethVars_t *var = &seth.vars[varCount];
			isMouseOn = ((eth.mouseX > MENU_SCREEN_BORDER)
					&& (eth.mouseX < (MENU_SCREEN_BORDER + maxItemWidth + MENU_ITEM_SPACE + maxValueWidth))
					&& (eth.mouseY > y)
					&& (eth.mouseY < (y + itemHeight)));

			// Draw var
			drawFillRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? colorBlack : gameBackColor);
			drawRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? gameBackColor : gameBorderColor, 1);
			drawText(MENU_SCREEN_BORDER + (itemHeight / 2) + sizeTextWidth(MENU_TEXT_SIZE, " ", &eth.fontVariable), y + 2, MENU_TEXT_SIZE, var->menuText, colorLtGrey, qfalse, qfalse, &eth.fontVariable);

			// Search position of the current value
			int pos = 0;
			while ((var->choices[pos].name != NULL) // Check for valid choice
					&& (var->choices[pos].value != seth.value[varCount])
					&& (pos < MAX_CHOICES)) {
				pos++;
			}

			// Get name of this value
			char value[32];
			memset(value, 0, sizeof(value));
			if ((pos == MAX_CHOICES) || (var->choices[pos].name == NULL))
				sprintf(value, "%.2f", seth.value[varCount]);
			else
				strcpy(value, var->choices[pos].name);

			// Draw value of this var
			drawFillRect(MENU_SCREEN_BORDER + MENU_ITEM_SPACE + maxItemWidth, y, maxValueWidth, itemHeight, isMouseOn ? colorBlack : gameBackColor);
			drawRect(MENU_SCREEN_BORDER + MENU_ITEM_SPACE + maxItemWidth, y, maxValueWidth, itemHeight, isMouseOn ? gameBackColor : gameBorderColor, 1);
			int w = sizeTextWidth(MENU_TEXT_SIZE, value, &eth.fontVariable);
			drawText(MENU_SCREEN_BORDER + MENU_ITEM_SPACE + maxItemWidth + (maxValueWidth / 2) - (w / 2), y + 2, MENU_TEXT_SIZE, value, colorLtGrey, qfalse, qfalse, &eth.fontVariable);
			
			y += itemHeight + MENU_ITEM_SPACE;

			// If not a value click
			if (((eth.mouseButton != Button1) && (eth.mouseButton != Button3)) || !isMouseOn)
				continue;
				
			// Get max choices
			int max = 0;
			while (var->choices[max].name != NULL)
				max++;
				
			// Pre-define values
			if (max) {
				// If current value no found take 0
				if ((pos < 0) || (pos == MAX_CHOICES))
					pos = 0;

				// Set new position
				if (eth.mouseButton == Button1)
					pos++;
				else
					pos--;

				// Check bounds
				if (pos >= max)
					pos = 0;
				else if (pos < 0)
					pos = max - 1;

			    seth.value[varCount] = var->choices[pos].value;

			// Manual value
			} else {
				// Set new value
				if (eth.mouseButton == Button1) {
					seth.value[varCount] += seth.value[VAR_STEP];
					if ((varCount == VAR_AIMFOV) && (seth.value[varCount] > 360.0f))
						seth.value[varCount] -= 360.0f;
					if ((varCount == VAR_HUMANVALUE) && (seth.value[varCount] > 1.0f))
						seth.value[varCount] -= seth.value[VAR_STEP];
				}
				else {
					seth.value[varCount] -= seth.value[VAR_STEP];
					if ((varCount == VAR_AIMFOV) && (seth.value[varCount] < 0.01f)) // 0.01 to make it 360.0 instead of 0.0 when decreasing
						seth.value[varCount] += 360.0f;
					if ((varCount == VAR_HUMANVALUE) && (seth.value[varCount] < 0.0f))
						seth.value[varCount] += seth.value[VAR_STEP];
				}
			}

			// Set cvar
/*			char cmd[50];
			sprintf(cmd, "%s%s %i\n", ETH_CVAR_PREFIX, menuItems[pos].cvarName, menuItems[count].choices[pos].value);
			orig_syscall(CG_SENDCONSOLECOMMAND, cmd);*/
	
			syscall_CG_S_StartLocalSound(eth.cgs_media_sndLimboSelect, CHAN_LOCAL_SOUND);
			eth.mouseButton = 0;
		}
	}
	openCategory = nextOpenCategory;
	eth.mouseButton = 0;
}
