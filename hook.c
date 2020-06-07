// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

/*
==============================
All game/system function hook.
==============================
*/

#define _GNU_SOURCE

#include "ghf/ghf.h"

#include "eth.h"
#include "offsets.h"

#include <link.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

// Lib constructor
void __attribute__ ((constructor)) ethInit(void) {
	// Search program name
	char *exeName;
	char link[PATH_MAX];
	memset(link, 0, sizeof(link));
	if (readlink("/proc/self/exe", link, sizeof(link)) <= 0)
		exit(1);
	exeName = strrchr(link, PATH_SEP);
	if (exeName == NULL)
		exit(1);
	exeName++;

	// Modify GOT entries only in et.x86
	if (!strcmp(exeName, "et.x86")) {
		// Active verbose debug file
		#ifdef ETH_DEBUG
			char *debugFileName = getenv("ETH_LOG_FILE");
			if (debugFileName && !debugFile) {
				debugFile = fopen(debugFileName, "w");
				if (!debugFile)
					printf("eth: error: Can't write debug file %s\n", debugFileName);
				else
					ethLog("debug log start: '%s'", debugFileName);
			}
		#endif

		// Init ET version dependant thing
		initETstruct();

		pltHook("dlsym", eth_dlsym);
		pltHook("dlopen", eth_dlopen);
		pltHook("XNextEvent", eth_XNextEvent);

		orig_Cmd_AddCommand = (void *)sethET->Cmd_AddCommand;

		// Only ET version < 2.60b need that
		if (sethET->COM_StripExtension != 0)
			orig_COM_StripExtension = detourFunction((void *)sethET->COM_StripExtension, eth_COM_StripExtension);
		
		orig_FS_PureServerSetLoadedPaks = detourFunction((void *)sethET->FS_PureServerSetLoadedPaks, eth_FS_PureServerSetLoadedPaks);

		// cvar unlocker
		orig_Cvar_Set2 = detourFunction((void *)sethET->Cvar_Set2, eth_Cvar_Set2);
		
		#ifdef ETH_PRIVATE
			priv_Init();
		#endif
	}
}

// Don't allow ET to grab mouse pointer. Usefull for debug.
#if 0
__attribute__ ((visibility ("default")))
int XWarpPointer(Display *display , Window src_w , Window dest_w , int src_x , int src_y , unsigned int src_width , unsigned int src_height , int dest_x , int dest_y ) {
	return 0;
}
__attribute__ ((visibility ("default")))
int XChangePointerControl ( Display *display , Bool do_accel , Bool do_threshold , int accel_numerator , int accel_denominator , int threshold ) {
	return 0;
}
__attribute__ ((visibility ("default")))
int XGrabPointer(Display *display, Window grab_window, Bool owner_events, unsigned int event_mask, int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time) {
	return 0;
}
__attribute__ ((visibility ("default")))
int XGetPointerControl ( Display *display , int *accel_numerator_return , int *accel_denominator_return , int *threshold_return ) {
	return 0;
}
#endif

void *eth_dlopen(const char *filename, int flag) {
	void *result = dlopen(filename, flag);
	
	if (result && filename) {
		if (strstr(filename, "cgame.mp.i386.so")) {
			eth.cgameFilename = strdup(filename);
			eth.cgameLibHandle = result;
		} else if (strstr(filename, "ui.mp.i386.so")) {
			eth.uiLibHandle = result;
		}
	}
	
	return result;
}

void *eth_dlsym(void *handle, const char *symbol) {
	void *result = dlsym(handle, symbol);
	
	if (handle == eth.cgameLibHandle) {
		if (!strcmp(symbol, "dllEntry")) {
			orig_CG_dllEntry = result;
			return eth_CG_dllEntry;
		} else if (!strcmp(symbol, "vmMain")) {
			orig_CG_vmMain = result;
			return eth_CG_vmMain;
		}
	} else if (handle == eth.uiLibHandle) {
		if (!strcmp(symbol, "dllEntry")) {
			orig_UI_dllEntry = result;
			return eth_UI_dllEntry;			
		} else if (!strcmp(symbol, "vmMain")) {
			orig_UI_vmMain = result;
			return eth_UI_vmMain;
		}
	}

	#ifdef ETH_PRIVATE
		void *function = priv_dlsym(symbol, result);
		if (function)
			return function;
	#endif

	return result;
}

// Hooked for direct keys/mouse access
int eth_XNextEvent(Display *display, XEvent *event) {
	int result = XNextEvent(display, event);

	switch (event->type) {
		case KeyPress:
			switch (XLookupKeysym(&event->xkey, 0)) {
				case ETH_MENU_KEY:
					eth.mouseButton = 0;
					eth.isMenuOpen = !eth.isMenuOpen;
					break;
				case XK_Escape:
					if (eth.isMenuOpen)
						eth.isMenuOpen = qfalse;
					if (eth.isPlistOpen)
						eth.isPlistOpen = qfalse;
					break;
				case ETH_POINTER_KEY:
					eth.pointer = !eth.pointer;
					break;
				case ETH_PLAYERLIST_KEY:
					eth.mouseButton = 0;
					eth.isPlistOpen = !eth.isPlistOpen;
					break;
#ifdef ETH_DEBUG
				case XK_Page_Up:
					if (seth.value[VAR_MEMDUMP])
						eth.offsetMul--;
					break;
				case XK_Page_Down:
					if (seth.value[VAR_MEMDUMP])
						eth.offsetMul++;
					break;
				case XK_Up:
					if (seth.value[VAR_MEMDUMP])
						eth.offsetSub--;
					break;
				case XK_Down:
					if (seth.value[VAR_MEMDUMP])
						eth.offsetSub++;
					break;
#endif // ETH_DEBUG
				default:
					break;
			}
			break;
		case ButtonPress:
			eth.mouseButton = event->xbutton.button;
			break;
		default:
			break;
	}

    return result;
}

int eth_CG_vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 ) {
	switch (command) {
		case CG_INIT: {
			eth.cg_clientNum = arg2; 
			initUserVars();
			readConfig();
			//static for test

			// Init only once!
			if (!seth.windowsInitialized) {
				initWindow("radarwinNG",640,0,0,0,0.16f,0.2f,0.17f,0.6f);
				initWindow("radar2win",640,0,0,0,0.2f,0.9f,0.2f,0.25f);
				initWindow("advertwin",0,50,0,0,0.0f,0.0f,0.0f,0.0f);
				initWindow("ircFTwin",10,50,0,0,0.16f,0.2f,0.17f,0.4f);
				initWindow("specwin",10,150,0,0,0.16f,0.2f,0.17f,0.4f);
				initWindow("irctopicwin",0,30,0,0,0.16f,0.2f,0.17f,0.4f);
				initWindow("refwin",10,250,0,0,0.16f,0.2f,0.17f,0.4f);
				initWindow("ircchatwin",10,400,0,0,0.16f,0.2f,0.17f,0.4f);
				seth.windowsInitialized = qtrue;
			}
			
			initEthCgameStruct();

			#ifdef ETH_PRIVATE
				priv_pre_CG_Init();
			#endif

			break;
		}
		case CG_DRAW_ACTIVE_FRAME:
			eth.cg_frametime = arg0 - eth.cg_time;
			eth.cg_time = arg0;
			eth.demoPlayback = arg2;
			#ifdef ETH_PRIVATE
				priv_pre_CG_DrawFrame();
			#endif
			break;
		case CG_SHUTDOWN: {
			writeConfig();
			eth_CG_Shutdown();
			eth.hookLoad = qfalse;
			// Some free'ing
			if (eth.CG_EntityEvent) {
				if (eth.mod.CG_EntityEvent == AUTO_OFFSET)
					undetourFunction(dlsym(eth.cgameLibHandle, "CG_EntityEvent"), eth.CG_EntityEvent);
				else
					undetourFunction((void *)(eth.cgameLibAddress + eth.mod.CG_EntityEvent), eth.CG_EntityEvent);
			}
			if (eth.CG_FinishWeaponChange) {
				if (eth.mod.CG_FinishWeaponChange == AUTO_OFFSET)
					undetourFunction(dlsym(eth.cgameLibHandle, "CG_FinishWeaponChange"), eth.CG_FinishWeaponChange);
				else
					undetourFunction((void *)(eth.cgameLibAddress + eth.mod.CG_FinishWeaponChange), eth.CG_FinishWeaponChange);
			}
			if (eth.CG_DamageFeedback) {
				if (eth.mod.CG_DamageFeedback == AUTO_OFFSET)
					undetourFunction(dlsym(eth.cgameLibHandle, "CG_DamageFeedback"), eth.CG_DamageFeedback);
				else
					undetourFunction((void *)(eth.cgameLibAddress + eth.mod.CG_DamageFeedback), eth.CG_DamageFeedback);
			}
			free(eth.cgameFilename);
			// Reset eth main struct
			memset(&eth, 0, sizeof(eth_t));
			break;
		}
		default:
			break;
	}

	int result = orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

	switch (command) {
		case CG_INIT: {
			#ifdef ETH_DEBUG
				ethDebug("CG_vmMain: CG_INIT");
			#endif
			// Get glconfig
			orig_syscall(CG_GETGLCONFIG, &eth.cgs_glconfig);
			eth.cgs_screenXScale = eth.cgs_glconfig.vidWidth / 640.0f;
			eth.cgs_screenYScale = eth.cgs_glconfig.vidHeight / 480.0f;

			// Init server addr
			sprintf(eth.server.hostname, "%i.%i.%i.%i", ((netadr_t *)sethET->serverIP)->ip[0], ((netadr_t *)sethET->serverIP)->ip[1], ((netadr_t *)sethET->serverIP)->ip[2], ((netadr_t *)sethET->serverIP)->ip[3]); 
			eth.server.port = htons(((netadr_t *)sethET->serverIP)->port);

			// Init eth
			eth.hookLoad = qtrue;
			eth_CG_Init();
			
			// We don't need more file from pk3 now
			unreferenceBadPk3();
			return 0;
		}
		case CG_DRAW_ACTIVE_FRAME: {
			#ifdef ETH_DEBUG
				ethDebug("CG_vmMain: CG_DRAW_ACTIVE_FRAME at server time: %i", eth.cg_time);
				ethDebug("last frame time: %i", eth.cg_frametime);
			#endif
			// Check for a valid snapshot
			if (eth.cg_snap && eth.hookLoad) {
				eth_CG_DrawActiveFrame();
				
				// End of frame so delete all ethEntities/refEntities.
				memset(eth.entities, 0, sizeof(eth.entities));
				memset(eth.refEntities, 0, sizeof(eth.refEntities));
				eth.refEntitiesCount = 0;
			}
			return 0;
		}
		default:
			break;
	}

	return result;
}

int eth_UI_vmMain (int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	int result = orig_UI_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

	// Good place for check irc event when not connected to a server
	ircNextEvent();

	switch (command) {
		case UI_INIT:
			registerGameCommands();
			break;
		default:
			break;
	}

	return result;
}

void eth_CG_dllEntry(int (*syscallptr) (int arg,...)) {
	orig_syscall = syscallptr;
	orig_CG_dllEntry(eth_CG_syscall);
}

void eth_UI_dllEntry(int (*syscallptr) (int arg,...)) {
	orig_syscall = syscallptr;
	orig_UI_dllEntry(eth_UI_syscall);
}

int eth_CG_syscall(int command, ...) {
	// Get all arguments to send them to the original syscall
	int arg[10];
	va_list arglist;
	va_start(arglist, command);
	int count = 0;
	for (; count < 10; count++)
		arg[count] = va_arg(arglist, int);
	va_end(arglist);

	if (eth.hookLoad && (command == CG_GETSNAPSHOT)) {
		#ifdef ETH_DEBUG
			ethDebug("eth_CG_syscall: CG_GETSNAPSHOT");
		#endif
		eth.cg_snap = (snapshot_t *)arg[1];
	}

	if (eth.hookLoad && eth.cg_snap) {
		qboolean callOriginal = qtrue;
		switch (command) {
			case CG_PRINT: {
				char *analise_str = (char *)arg[0];	
				
				// 'skip notify' msg
				#define MSG_SKIPNOTIFY "[skipnotify]"
				if (!strncmp(analise_str, MSG_SKIPNOTIFY, strlen(MSG_SKIPNOTIFY))) {
					// Auto vote
					if ((seth.value[VAR_AUTOVOTE] == 1) && strstr(analise_str, "^7 called a vote.  Voting for: ")) {
						doAutoVote(analise_str + strlen(MSG_SKIPNOTIFY));

						char addMsg[512];
						int a;
						for (a = 12 ; a < strlen(analise_str) ; a++)
							addMsg[a - 12] = analise_str[a];
						
						addMsg[strlen(addMsg)-1] = '\0';
						addHudConsoleMsg(addMsg);
					}
				
					// Anti inactivity kick
					if (seth.value[VAR_INACTIVITY] == 1)
						if (!strcmp(analise_str + strlen(MSG_SKIPNOTIFY), "^310 seconds until inactivity drop!\n"))
							// TODO: find an invisible way for generate server activity
							syscall_CG_SendConsoleCommand("+leanright; -leanright\n");
					
					// For hud txts 
					if (analise_str[12] == ':') { // voice chat
						int a;
						for (a = 13 ; a < strlen(analise_str) ; a++)
							chat_p[eth.currentChat][a - 13] = analise_str[a];
						
						chat_p[eth.currentChat][strlen(analise_str)-14] = '\0';
						addHudConsoleChat(chat_p[eth.currentChat]);
						if (eth.currentChat == 2)
							eth.currentChat = 0;
						else eth.currentChat++;
					}
				// not skipnotify
				} else {
					strcpy(console_p[eth.currentTxt], analise_str);
					console_p[eth.currentTxt][strlen(console_p[eth.currentTxt])-1] = '\0';
					if (isChatMsg(console_p[eth.currentTxt]))
						addHudConsoleChat(console_p[eth.currentTxt]);
					else
						addHudConsoleMsg(console_p[eth.currentTxt]);
					if (eth.currentTxt == 4) 
						eth.currentTxt = 0;
					else eth.currentTxt++;
				}
				break;
			}
			case CG_R_ADDREFENTITYTOSCENE: {
				eth_CG_R_AddRefEntityToScene((refEntity_t *)arg[0]);
				break;
			}
			case CG_S_UPDATEENTITYPOSITION: {
				eth_CG_S_UpdateEntityPosition(arg[0], *(vec3_t *)arg[1]);
				break;
			}
			case CG_R_RENDERSCENE: {
				#ifdef ETH_DEBUG
					ethDebug("eth_CG_syscall: CG_R_RENDERSCENE");
				#endif

				if (seth.value[VAR_RENDER])
					callOriginal = qfalse;
				else
					callOriginal = eth_CG_R_RenderScene((refdef_t *)arg[0]);
				break;
			}
			case CG_R_DRAWSTRETCHPIC: {
				callOriginal = eth_CG_R_DrawStretchPic(*(float *)&arg[0], *(float *)&arg[1], *(float *)&arg[2], *(float *)&arg[3], *(float *)&arg[4], *(float *)&arg[5], *(float *)&arg[6], *(float *)&arg[7], (qhandle_t)arg[8]);
				break;
			}
			case CG_SETUSERCMDVALUE: {
				// Remove game sensitivity change
				if (seth.value[VAR_SNIPERZOOM] == 1)
					*(float *)&arg[2] = 1.0f;
				break;
			}
		    case CG_R_ADDPOLYTOSCENE: {
			if (seth.value[VAR_SMOKE] == SMOKE_TRANSPARENT) {
				if (arg[0] == eth.cgs_media_smokePuffshader) {
					polyVert_t *verts = (polyVert_t *)arg[2];
					int count = 0;
					for (; count < 4; count ++)
						verts[count].modulate[3] = 0x20;
				}
		    	}
			if (seth.value[VAR_SMOKE] == SMOKE_REMOVE)
				callOriginal = qfalse;
		    	break;
		    }
			default:
				break;
		}
		if (callOriginal == qfalse)
			return 0;
	}

	int result = orig_syscall(command, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);

	switch (command) {
	    case CG_GETSNAPSHOT: {
	    	if (!result || !eth.hookLoad)
	    		break;
	    	
			// HUD reload info // TODO: move this elsewhere
			if (eth.cg_snap->ps.weaponstate == WEAPON_RELOADING && eth.reloading == 0) {
				eth.reloading = 1; 
				eth.reloadScale = eth.cg_snap->ps.weaponTime; 
			}
			if (eth.reloading == 1 && eth.cg_snap->ps.weaponstate != WEAPON_RELOADING) {
				eth.reloadScale = 0;
				eth.reloading = 0;
			}
			
			break;
	    }
		case CG_R_REGISTERSHADER:
		case CG_R_REGISTERSHADERNOMIP:
		case CG_R_REGISTERMODEL:
		case CG_R_REGISTERSKIN:
		case CG_S_REGISTERSOUND: {
			char *name = (char *)arg[0];
			
			// If invalid name
			if (!name || !strlen(name))
				break;

			if (!strcmp(name, "white"))
				eth.cgs_media_whiteShader = result;
			else if (!strcmp(name, "gfx/misc/reticlesimple"))
				eth.cgs_media_reticleShaderSimple = result;
			else if (!strcmp(name, "gfx/misc/binocsimple"))
				eth.cgs_media_binocShaderSimple = result;
			else if (!strcmp(name, "ui/assets/3_cursor3"))
				eth.cgs_media_cursorIcon = result;
			else if (!strcmp(name, "models/weapons2/shells/sm_shell.md3"))
				eth.cgs_media_smallgunBrassModel = result;
			else if (!strcmp(name, "models/multiplayer/ammopack/ammopack_pickup.md3"))
				eth.hAmmoPack = result;
			else if (!strcmp(name, "models/multiplayer/medpack/medpack_pickup.md3"))
				eth.hMedicPack = result;
			else if (!strcmp(name, "models/players/hud/head.md3"))
				eth.hHead = result;
			else if (!strcmp(name, "sound/menu/select.wav"))
				eth.cgs_media_sndLimboSelect = result;
			else if (!strcmp(name, "railCore"))
				eth.cgs_media_railCoreShader = result;
			else if (!strcmp(name, "smokePuff"))
				eth.cgs_media_smokePuffshader = result;
			break;
		}
		case CG_GETSERVERCOMMAND:	{
			const char *cmd = syscall_CG_Argv(0);
			
			// If not valid cmd
			if (!cmd || !strlen(cmd))
				break;

			if (!strcmp(cmd, "chat")) {
				#define CHAT_MARKER "^7: ^2"
				int fromPlayer = atoi(syscall_CG_Argv(2));
				char *msg = strstr(syscall_CG_Argv(1), CHAT_MARKER);

				// Sanity check
				if (!msg)
					break;
				
				msg += strlen(CHAT_MARKER);
				
				// Check for medicbot request
				if (!strncmp(msg, MEDICBOT_REQUEST, strlen(MEDICBOT_REQUEST))) {
					msg += strlen(MEDICBOT_REQUEST);
					doMedicBotCommand(fromPlayer, msg);
				}
			} else if (!strcmp(cmd, "vchat")) {
				int fromPlayer = atoi(syscall_CG_Argv(2));
				const char *str = syscall_CG_Argv(4);
	
				if ((fromPlayer != eth.cg_clientNum) && str) {
					if (!seth.value[VAR_ETHFIND])
						break;

					// Put an eth users in your friend list
					if (!strncmp(str, ETH_ANNOUNCE_STR, strlen(ETH_ANNOUNCE_STR))) {
						#ifdef ETH_DEBUG
							ethLog("%s is using ETH", eth.clientInfo[fromPlayer].name);
						#endif
						if (eth.clientInfo[fromPlayer].targetType != PLIST_FRIEND) {
							eth.clientInfo[fromPlayer].targetType = PLIST_FRIEND;
							addHudOutputMsg(hudinfo, "%s recognized as an eth user", eth.clientInfo[fromPlayer].cleanName);
						}
						// Say to others im an eth users
						if (!strncmp(str, ETH_REQUEST_STR, strlen(ETH_REQUEST_STR)))
							syscall_CG_SendConsoleCommand("vsay " ETH_ANNOUNCE_STR "\n");
					}

					// Backup last near medic request for medicbot
					if (!strcasecmp(str, "Medic")
							&& (VectorDistance(eth.cg.refdef->vieworg, eth.entities[fromPlayer].origin) < 500.0f)) {
						eth.lastMedicRequest = fromPlayer;
						eth.lastMedicRequestTime = eth.cg_time;
					}
				}
			} else if (!strcmp(cmd, "cs")) {
				// Only update if cgs.gameState already exist
				if (!eth.cgs_gameState)
					break;
				
				int num = atoi(syscall_CG_Argv(1));
				const char *str = eth.cgs_gameState->stringData + eth.cgs_gameState->stringOffsets[num];
				switch (num) {
					case CS_WOLFINFO:
						eth.cgs_gamestate = atoi(eth_Info_ValueForKey(str, "gamestate"));
						break;
					default:
						break;
				}
			}
			break;
		}
		case CG_GETGAMESTATE: {
			eth.nameCounter++;	// ??
			eth.cgs_gameState = (gameState_t *)arg[0];
			char *info;
			int client = 0;
			seth.refCount = 0;
			for (; client < MAX_CLIENTS; client++) {
				int offset = eth.cgs_gameState->stringOffsets[client + CS_PLAYERS];
				if (offset != 0) {
					// Get name
					eth.clientInfo[client].infoValid = qtrue;
					info = eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "n");
					if (eth.nameCounter >= 2) { // store name when gs struct updated - random nick
						strcpy(eth.clientInfo[client].name, info);
						strcpy(eth.clientInfo[client].cleanName, Q_CleanStr(info));
					} else if (client != eth.cg_clientNum) {  // store names always 
						strcpy(eth.clientInfo[client].name, info);
						strcpy(eth.clientInfo[client].cleanName, Q_CleanStr(info));
					}    
					// Get team
					info = eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "t");
					eth.clientInfo[client].team = atoi(info);
					// Get class
					info = eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "c");
					eth.clientInfo[client].cls = atoi(info);
					// Get referee status
					info = eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "ref");
					int ref = atoi(info);
					if (ref) {
						seth.refCount++;
						strcpy(seth.refNames[seth.refCount-1], eth.clientInfo[client].name);
					}
				} else {
					eth.clientInfo[client].infoValid = qfalse;
					eth.clientInfo[client].targetType = PLIST_UNKNOWN;
				}
			}
	
			// Get limbotimes
			int offset = eth.cgs_gameState->stringOffsets[CS_SERVERINFO];
			if (offset) {
				eth.cg_redlimbotime = atoi(eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "g_redlimbotime"));
				eth.cg_bluelimbotime = atoi(eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "g_bluelimbotime"));
			}
			// Get charge times
			offset = eth.cgs_gameState->stringOffsets[CS_CHARGETIMES];
			if (offset) {
				eth.cg_soldierChargeTime[0] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "axs_sld" ));
				eth.cg_soldierChargeTime[1] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "ald_sld" ));
				eth.cg_medicChargeTime[0] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "axs_mdc" ));
				eth.cg_medicChargeTime[1] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "ald_mdc" ));
				eth.cg_engineerChargeTime[0] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "axs_eng" ));
				eth.cg_engineerChargeTime[1] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "ald_eng" ));
				eth.cg_ltChargeTime[0] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "axs_lnt" ));
				eth.cg_ltChargeTime[1] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "ald_lnt" ));
				eth.cg_covertopsChargeTime[0] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "axs_cvo" ));
				eth.cg_covertopsChargeTime[1] = atoi(eth_Info_ValueForKey( (char *)(eth.cgs_gameState->stringData + offset), "ald_cvo" ));
			}
			// Get cgs.levelStartTime
			offset = eth.cgs_gameState->stringOffsets[CS_LEVEL_START_TIME];
			if (offset)
				eth.cgs_levelStartTime = atoi(eth.cgs_gameState->stringData + offset);
			// Get cgs.aReinfOffset
			offset = eth.cgs_gameState->stringOffsets[CS_REINFSEEDS];
			if (offset)
				eth_CG_ParseReinforcementTimes(eth.cgs_gameState->stringData + offset);
			// Get punkbuster state
			offset = eth.cgs_gameState->stringOffsets[CS_SERVERINFO];
			info = eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "sv_punkbuster");
			if (offset && info)
				eth.punkbusterState = atoi(info);
			// Get cgs.gamestate
			offset = eth.cgs_gameState->stringOffsets[CS_WOLFINFO];
			if (offset)
				eth.cgs_gamestate = atoi(eth_Info_ValueForKey((char *)(eth.cgs_gameState->stringData + offset), "gamestate"));
			
			break;
		}
		default:
			break;
	}

	return result;
}

int eth_UI_syscall(int command, ...) {
	// Get all arguments to send them to the original syscall
	int arg[10];
	va_list arglist;
	va_start(arglist, command);
	int count = 0;
	for (; count < 10; count++)
		arg[count] = va_arg(arglist, int);
	va_end(arglist);
	
	if (eth.hookLoad) {
		qboolean callOriginal = qtrue;
	
		switch (command) {
			case UI_R_DRAWSTRETCHPIC: {
				// Get game mouse position - TODO: dirty find another way
				int width = *(float *)&arg[2] / eth.cgs_screenXScale;
				int height = *(float *)&arg[3] / eth.cgs_screenYScale;
				qhandle_t hShader = (qhandle_t)arg[8];
				// Find cursor picture
				if (width == 32 && height == 32 && hShader == eth.cgs_media_cursorIcon) {
					eth.mouseX = *(float *)&arg[0] / eth.cgs_screenXScale;
					eth.mouseY = *(float *)&arg[1] / eth.cgs_screenYScale;
				}
				break;
			}
			default:
				break;
		}
		if (callOriginal == qfalse)
			return 0;
	}

	// Had to move this here, otherwise customized menu isn't loaded
	switch (command) {
		case UI_PC_LOAD_SOURCE: {
			char *name = (char *)arg[0];
	
			// If invalid name
			if (!name || !strlen(name))
				break;
	
			if (!strcmp(name, "ui/ingame_messagemode.menu"))
				arg[0] = (int) "ui/nix_msgpopup.menu";
	
			break;
		}
		default:
			break;
	}

	return orig_syscall(command, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
}

void initETstruct() {
	char link[PATH_MAX];
	memset(link, 0, sizeof(link));
	if (readlink("/proc/self/exe", link, sizeof(link)) <= 0)
		exit(1);

	unsigned long crc32 = crc32FromFile(link);
	
	int count = 0;
	for (; count < (sizeof(ethET) / sizeof(ethET_t)); count++) {
		if (ethET[count].crc32 == crc32)
			break;
	}

	if (count == (sizeof(ethET) / sizeof(ethET_t))) {
		ethLog("This ET version is not supported.");
		#ifdef ETH_DEBUG			
			ethLog("et.x86 crc32 %p", crc32);
		#endif
		exit(1);			
	} else {
		sethET = &ethET[count];
		#ifdef ETH_DEBUG			
			ethLog("found ET version %s", sethET->version);
		#endif
	}
}

// This function was use by 'dl_iterate_phdr()' as a callback
int dlIterateCallback (struct dl_phdr_info *info, size_t size, void *data) {
	// If cgame lib, get the load address
    if (!strcmp(info->dlpi_name, eth.cgameFilename))
   		eth.cgameLibAddress = (void *)info->dlpi_addr;
    return 0;
}

void initEthCgameStruct() {
	u_int crc32 = crc32FromFile(eth.cgameFilename);

	#ifdef ETH_DEBUG
		ethLog("mod: crc32 0x%x", crc32);
	#endif
	
	// Find the mod
	int count = 0;
	for (; count < (sizeof(ethMods) / sizeof(ethMod_t)); count++) {
		if (ethMods[count].crc32 == crc32)
			break;
	}
	
	// mod not found
	if (count == (sizeof(ethMods) / sizeof(ethMod_t)))
		fatalError("This mod was not supported.");

	eth.mod = ethMods[count];

	#ifdef ETH_DEBUG
		ethDebug("mod: found %s v%s", eth.mod.name, eth.mod.version);
	#endif
	
	// Wait for cgameLib load FIXME: wtf ? why must wait
	int tryCount = 0;
	while (eth.cgameLibAddress == NULL) {
		tryCount++;
		// Find cgame lib load addresse
		dl_iterate_phdr(dlIterateCallback, NULL);
		if (tryCount > 5)
			fatalError("Hook failed. Can't find 'cgame.mp.i386.so' in memory.");

		sleep(1);
	}

	#ifdef ETH_DEBUG
		ethDebug("mod: load @ %p", eth.cgameLibAddress);
	#endif
	
	// CG_Trace()
	if (eth.mod.CG_Trace == AUTO_OFFSET)
		eth.CG_Trace = dlsym(eth.cgameLibHandle, "CG_Trace");
	else
		eth.CG_Trace = (void *)(eth.cgameLibAddress + eth.mod.CG_Trace);

	// BG_EvaluateTrajectory()
	if (eth.mod.CG_Trace == AUTO_OFFSET)
		eth.BG_EvaluateTrajectory = dlsym(eth.cgameLibHandle, "BG_EvaluateTrajectory");
	else
		eth.BG_EvaluateTrajectory = (void *)(eth.cgameLibAddress + eth.mod.BG_EvaluateTrajectory);

	// CG_RailTrail2()
	if (eth.mod.CG_RailTrail2 == AUTO_OFFSET)
		eth.CG_RailTrail2 = dlsym(eth.cgameLibHandle, "CG_RailTrail2");
	else
		eth.CG_RailTrail2 = (void *)(eth.cgameLibAddress + eth.mod.CG_RailTrail2);

	// cg based
	if (eth.mod.cg == AUTO_OFFSET) {
		eth.cg.refdefViewAngles = (float *)(dlsym(eth.cgameLibHandle, "cg") + eth.mod.cg_refdefViewAngles);
		eth.cg.refdef = (refdef_t *)(dlsym(eth.cgameLibHandle, "cg") + eth.mod.cg_refdef);
	} else {
		eth.cg.refdefViewAngles = (float *)(eth.cgameLibAddress + eth.mod.cg + eth.mod.cg_refdefViewAngles);
		eth.cg.refdef = (refdef_t *)(eth.cgameLibAddress + eth.mod.cg + eth.mod.cg_refdef);
	}

	int entityCount = 0;
	for (; entityCount < MAX_GENTITIES; entityCount++) {
		int offset;
		if (eth.mod.cg_entities == AUTO_OFFSET)
			offset = (u_int)dlsym(eth.cgameLibHandle, "cg_entities") + (entityCount * eth.mod.centity_t_size);
		else
			offset = (u_int)eth.cgameLibAddress + eth.mod.cg_entities + (entityCount * eth.mod.centity_t_size);
		
		eth.cg_entities[entityCount].currentState = (entityState_t *)(offset + CG_ENTITIES_CURRENTSTATE_OFFSET);
		eth.cg_entities[entityCount].currentValid = (qboolean *)(offset + CG_ENTITIES_CURRENTVALID_OFFSET);
	}
	
	// Hooked functions - Don't forget to call undetour for free'ing the tramp
	// CG_EntityEvent()
	if (eth.mod.CG_EntityEvent == AUTO_OFFSET)
		eth.CG_EntityEvent = detourFunction(dlsym(eth.cgameLibHandle, "CG_EntityEvent"), eth_CG_EntityEvent);
	else
		eth.CG_EntityEvent = detourFunction((void *)(eth.cgameLibAddress + eth.mod.CG_EntityEvent), eth_CG_EntityEvent);

	// CG_FinishWeaponChange()
	if (eth.mod.CG_FinishWeaponChange == AUTO_OFFSET)
		eth.CG_FinishWeaponChange = detourFunction(dlsym(eth.cgameLibHandle, "CG_FinishWeaponChange"), eth_CG_FinishWeaponChange);
	else
		eth.CG_FinishWeaponChange = detourFunction((void *)(eth.cgameLibAddress + eth.mod.CG_FinishWeaponChange), eth_CG_FinishWeaponChange);

	// CG_DamageFeedback()
	if (eth.mod.CG_DamageFeedback == AUTO_OFFSET)
		eth.CG_DamageFeedback = detourFunction(dlsym(eth.cgameLibHandle, "CG_DamageFeedback"), eth_CG_DamageFeedback);
	else
		eth.CG_DamageFeedback = detourFunction((void *)(eth.cgameLibAddress + eth.mod.CG_DamageFeedback), eth_CG_DamageFeedback);

	// etpro stuff
	if (eth.mod.guid_function) {
		orig_etproGuidFunction = detourFunction((void *)(eth.cgameLibAddress + eth.mod.guid_function), etproGuidFunction);
		*((unsigned char *)(orig_etproGuidFunction + 4)) = 0xbb;	// movl imm32, %ebx
		*((void **)(orig_etproGuidFunction + 5)) = eth.cgameLibAddress + eth.mod.guid_function + 9;	// lame but neccessary
	}
	if (!strcmp(eth.mod.name, "etpro"))
		etproCvarProof();

	return;
}

// pure .pk3 unlocker

// Helper function 
pack_t *getPack(char *filename) {
	searchpath_t *browse = *(searchpath_t **)sethET->fs_searchpaths;
	for (; browse; browse = browse->next)
		if (browse->pack && strstr(browse->pack->pakFilename, filename))
			return browse->pack;
	return NULL;
}

void eth_FS_PureServerSetLoadedPaks(const char *pakSums, const char *pakNames) {
	static char fakePakSums[BIG_INFO_STRING];
	static char fakePakNames[BIG_INFO_STRING];

	strncpy(goodChecksumsPak, pakSums, sizeof(goodChecksumsPak));
	pack_t *ethPack = getPack(ETH_PK3_FILE);

	// If not pure server
	if (!strlen(pakSums) && !strlen(pakNames)) {
		orig_FS_PureServerSetLoadedPaks(pakSums, pakNames);
	// Add eth pk3
	} else if (ethPack) {
		sprintf(fakePakSums, "%s%i ", pakSums, ethPack->checksum);
		sprintf(fakePakNames, "%s %s/%s", pakNames, ethPack->pakGamename, ethPack->pakBasename);
		orig_FS_PureServerSetLoadedPaks(fakePakSums, fakePakNames);
	} else {
		ethLog("pk3: error: '" ETH_PK3_FILE "' not found.\n");
		orig_FS_PureServerSetLoadedPaks(pakSums, pakNames);
	}
}

void unreferenceBadPk3() {
	// Don't find bad pak on not pure server
	if (!strlen(goodChecksumsPak))
		return;
	
	// Parse all paks
	searchpath_t *browse = *(searchpath_t **)sethET->fs_searchpaths;
	for (; browse; browse = browse->next) {
		if (!browse->pack)
			continue;

		// Parse all checksums for find this one
		qboolean purePak = qfalse;
		char *checksum = goodChecksumsPak;
		char *nextChecksum;
		while ((nextChecksum = strchr(checksum, ' '))) {
			char *sum = strndup(checksum, nextChecksum - checksum);

			// Check if in pure list
			if (browse->pack->checksum == atoi(sum))
				purePak = qtrue;

			checksum = nextChecksum + 1;
			free(sum);
		};

		// If not pure and referenced
		if (!purePak && browse->pack->referenced) {
			browse->pack->referenced = 0;
			if (!strstr(browse->pack->pakFilename, ETH_PK3_FILE))
				ethLog("pk3: invalid pak found: %s\n", browse->pack->pakFilename);
		}
	}
}

// fix the vulnerable com_stripextension function of q3
void eth_COM_StripExtension (const char *in, char *out) {
	if (strlen(in) > 60)
		return;
	orig_COM_StripExtension(in, out);
}

/*
==============================
etpro guid
==============================
*/

// hooked etpro getGuid function. Can be init by environnement var
void etproGuidFunction(void *a, void *b, void *c, void *d, void *e, char *orig_guid) {
	char *envGuid = getenv("GUID");

	if (!etproGuid) {
		etproGuid = calloc(ETPRO_GUID_SIZE, 1);
		// If there is an environnement var
		if (envGuid)
			strncpy(etproGuid, envGuid, (strlen(envGuid) <= ETPRO_GUID_SIZE) ? strlen(envGuid) : ETPRO_GUID_SIZE);
		// Else get the original
		else
			memcpy(etproGuid, orig_guid, ETPRO_GUID_SIZE);
	}

	orig_etproGuidFunction(a, b, c, d, e, etproGuid);
}

/*
==============================
 etpro cvars proof
==============================
*/

#define GET_PAGE(addr) ((void *)(((u_int32_t)addr) & ~((u_int32_t)(getpagesize() - 1))))
void etproCvarProof(void) {
	void *ptr;
	
	if (!strcmp(eth.mod.version, "3.2.4") || !strcmp(eth.mod.version, "3.2.5"))
		ptr = eth.cgameLibAddress + 0xf368; // disable a loop
	else if (!strcmp(eth.mod.version, "3.2.6-beta1"))
		ptr = eth.cgameLibAddress + 0xf718; // disable a loop
	else
		return;

	mprotect(GET_PAGE(ptr), 4096, PROT_READ|PROT_WRITE|PROT_EXEC);
	memcpy(ptr, "\x31", 1); // xorl %edx, %edx
	mprotect(GET_PAGE(ptr), 4096, PROT_READ|PROT_EXEC);
}
