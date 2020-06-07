// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

/*
==============================
All game functions hooked for get information from it.
==============================
*/

#include <sys/resource.h>

#include "eth.h"

void eth_CG_R_AddRefEntityToScene(refEntity_t *refEnt) {
	ethEntity_t* entity = &eth.entities[refEnt->entityNum];
	int entityNum = refEnt->entityNum;
	trace_t trace;

	// Backup refEntity for spycam
	memcpy(&eth.refEntities[eth.refEntitiesCount++], refEnt, sizeof(refEntity_t));

	if ((entityNum < MAX_CLIENTS) \
		&& (eth.cg_entities[entityNum].currentState->eType == ET_PLAYER) \
		&& eth.clientInfo[entityNum].infoValid \
		&& (eth.cg_clientNum != entityNum)) {
			entity->isValid = qtrue;
//	if (IS_PLAYER_VALID(entityNum)) {
		if (refEnt->hModel == eth.hHead) {
			// Skip already define head entity (hud head)
			if (!VectorCompare(vec3_origin, entity->head))
				return;

			// Backup head origin
			VectorCopy(refEnt->origin, entity->head);

			// Backup head axis for mortar trace
			memcpy(entity->headAxis, refEnt->axis, sizeof(vec3_t) * 3);

			// Apply vecz
			doVecZ(entityNum);
			
			#ifdef ETH_DEBUG
				if (!seth.value[VAR_PREDICT_STATS]) 
			#endif
					doPrediction(entityNum);

			
			// Draw a rail trail to see where the player look
			if (seth.value[VAR_RAILAXIS]) {
				vec3_t forward;
				VectorMA(entity->head, 64, refEnt->axis[0], forward);
				eth.CG_RailTrail2(NULL, entity->head, forward);
			}

			// If head visible
			if (isVisible(entity->head)) {
				entity->isVisible = qtrue;
				entity->isHeadVisible = qtrue;
			}

			// Pig head for enemy
			if (seth.value[VAR_PIGHEAD] && IS_PLAYER_ENEMY(entityNum)) {
				refEnt->hModel = eth.pigHeadModel;
				refEnt->customSkin = eth.pigHeadSkin;
			// Chams
			} else {
				if (seth.value[VAR_CHAMS]
						&& !seth.value[VAR_PIGHEAD]
						&& (seth.value[VAR_CHAMS] != CHAM_GLOWONLY)) {
					addChams(refEnt, entity->isVisible);
				}
			}

		// If body
		} else if (refEnt->torsoFrameModel) {
			// Backup bodyRefEnt for tag grabbing
			memcpy(&eth.cg_entities[entityNum].pe_bodyRefEnt, refEnt, sizeof(refEntity_t));

			// Find a visible body parts
			vec3_t target;
			if (getVisibleModelBodyTag(entityNum, &target)) {
				VectorCopy(target, entity->bodyPart);
				entity->isVisible = qtrue;
			}

			// Chams
			if (seth.value[VAR_CHAMS])
				addChams(refEnt, entity->isVisible);
		// Not body or head
		} else
			return;
	} else if (IS_MISSILE(entityNum)) {
		entity->isValid = qtrue;

		// If visible
		eth.CG_Trace(&trace, eth.cg.refdef->vieworg, NULL, NULL, refEnt->origin, eth.cg_snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_CORPSE);
		if (trace.fraction == 1.0f)
			entity->isVisible = qtrue;

		// Chams
		if (seth.value[VAR_CHAMS])
			addChams(refEnt, (trace.fraction == 1.0f));
	// Add rail trail here so it can have wallhack too
	} else if (refEnt->reType == RT_RAIL_CORE) {
	// Not player or missile
	} else
		return;

	// Wallhack
	if (seth.value[VAR_WALLHACK])
		refEnt->renderfx |= RF_DEPTHHACK | RF_NOSHADOW;
}

void eth_CG_S_UpdateEntityPosition(int entityNum, const vec3_t origin) {
	ethEntity_t* entity = &eth.entities[entityNum];
	entity->isValid = qtrue;

	if (IS_PLAYER_VALID(entityNum) || IS_MISSILE(entityNum)) {
		VectorCopy(origin, entity->origin);
		entity->distance = VectorDistance(eth.cg.refdef->vieworg, entity->origin);
		entity->isInScreen = worldToScreen(entity->origin, &entity->screenX, &entity->screenY);
		
		addEspColors(entityNum, entity->isVisible);
	}
}

void eth_CG_DrawActiveFrame(void) {

	// Don't draw stuff in intermission
	if (eth.cgs_gamestate != GS_INTERMISSION) {
		
		// *** Visuals stuff ***
	
		// Set esp colors from vars
		setColors();
	
		#ifdef ETH_DEBUG
			// Draw debugview 
			if (seth.value[VAR_MEMDUMP]) 
				drawMemDump(getVarOffset(seth.value[VAR_MEMDUMP]) + (eth.offsetMul * 200) + (eth.offsetSub * 20));
			if (seth.value[VAR_PREDICT_STATS]) 
				doPredictionStats();
		#endif
	
		// Custom HUD
		if (seth.value[VAR_CHUD] == HUD_1)
				drawCustomHud();
		else if (seth.value[VAR_CHUD] == HUD_2)
				drawETHhud();
	
		if (!eth.demoPlayback) { // These funcs are not needed when watching a demo
			// Draw spectators
			if (seth.value[VAR_SPEC] && (eth.cg_snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR)) {
				getSpectators();
				drawSpectators();
			}
			
			// Draw irc stuff
			if (seth.value[VAR_IRC] && seth.value[VAR_IRC_FT])
				drawIrcFireteam();
			drawIrcChat();
			if (seth.value[VAR_IRC_TOPIC] || seth.value[VAR_IRC_INVITE])
				drawIrcTopic();
	
			// Teamkillers / Friends add/remove list
			if (eth.isPlistOpen) {
				drawPlayerList();
				// Get mouse/key events
				orig_syscall(CG_KEY_SETCATCHER, KEYCATCH_UI | orig_syscall(CG_KEY_GETCATCHER));
			}

			// Referee counter
			if (seth.value[VAR_REFLIST])
				drawReferees();
	
			// *** Actions stuff ***
		
			// Autoshoot
			if (seth.value[VAR_AUTOSHOOT])
				setAction(ACTION_BINDMOUSE1, qfalse);
			else
				setAction(ACTION_BINDMOUSE1, qtrue);
		
			// Medicbot
			if (seth.value[VAR_MEDICBOT])
				doMedicBot();
		
			// Random name changer
			if (seth.value[VAR_RANDOMNAME]) {
				// If error stop it
				if (!setRandomName())
					seth.value[VAR_RANDOMNAME] = 0;
			}

			// Do aimbot
			doAimbot();
		}
			
		// Draw right spawntimer
		if (seth.value[VAR_SPAWNTIMER] && !seth.value[VAR_CHUD]) // Don't need 2 spawntimers at the same time
			drawSpawnTimerRight();
	
		// Draw advert
		if (seth.value[VAR_ADVERT])
			drawAdvert();
	
		// Draw players infos
		drawEspEntities();
	
		// Draw radar
		switch((int)seth.value[VAR_RADAR]) {
		case 1: drawRadar(200); break;
		case 2: drawRadar(150); break;
		case 3: drawRadar(100); break;
		case 4: drawRadar(50); break;
		case 5: drawRadarNg(150); break;
		case 6: drawRadarNg(100); break;
		case 7: drawRadarNg(50); break;
		default:
				break;
		}
	}
	
	if (eth.pointer)
	    orig_syscall(CG_KEY_SETCATCHER, KEYCATCH_UI | orig_syscall(CG_KEY_GETCATCHER));


	if (eth.isMenuOpen) {
		drawMenu();
		// Get mouse/key events
		orig_syscall(CG_KEY_SETCATCHER, KEYCATCH_UI | orig_syscall(CG_KEY_GETCATCHER));
	}

	// Remove spectator locking
	if (seth.value[VAR_SPECLOCK])
		eth.cg_snap->ps.powerups[PW_BLACKOUT] = 0;
}

qboolean eth_CG_R_RenderScene(refdef_t *refDef) {
	// Check if the scene is the mainview
	if (((refDef->x == 0) || (refDef->x == 1))	// Some mods use 0 or 1 for x or y
			&& ((refDef->y == 0) || (refDef->y == 1))) {

		if (seth.value[VAR_SNIPERZOOM]) {
			switch(eth.cg_snap->ps.weapon) {
				case WP_FG42SCOPE:
				case WP_GARAND_SCOPE:
				case WP_K43_SCOPE: {
					// fov_x - Get from cg_fov cvar
					char fov_x[8];
					syscall_CG_Cvar_VariableStringBuffer("cg_fov", fov_x, sizeof(fov_x));
					refDef->fov_x = atof(fov_x);

					// fov_y - Algo take from cg_view.c line 961
					float x = refDef->width / tan(refDef->fov_x / 360 * M_PI);
					refDef->fov_y = atan2(refDef->height, x);
					refDef->fov_y *= 360 / M_PI;

	        		break;
				}
				default:
					break;
			}
	    }

		// Call original function
		orig_syscall(CG_R_RENDERSCENE, refDef);
		
		// Weapons spy cam
		switch (eth.cg_entities[eth.cg_snap->ps.clientNum].currentState->weapon) {
			case WP_MORTAR_SET:
				if (seth.value[VAR_MORTARCAM])
					drawMortarCam();
				break;
			case WP_PANZERFAUST:
				if (seth.value[VAR_PANZERCAM])
					drawPanzerCam();
				break;
			case WP_SATCHEL_DET:
				if (seth.value[VAR_SATCHELCAM])
					drawSatchelCam();
				break;
			default:
				break;
		}

		if (seth.value[VAR_MIRRORCAM])
			drawMirrorCam();

		return qfalse;
	}
	return qtrue;
}

// Return qfalse if this pic don't have to be draw
qboolean eth_CG_R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader) {

 	adjustTo640(&x, &y, &w, &h);

 	// Remove sniper/binocular picture
	if (hShader == eth.cgs_media_reticleShaderSimple || hShader == eth.cgs_media_binocShaderSimple)
		return qfalse;
	// Remove sniper/binocular black area
	if (((int)y == 0) && ((int)w == 80) && (hShader == eth.cgs_media_whiteShader))
		return qfalse;
	// Remove zoom black fade in/out
	if (((int)x == -10) && ((int)y == -10) && ((int)w == 650) && ((int)h == 490) && (hShader == eth.cgs_media_whiteShader))
		return qfalse;

	return qtrue;
}

void eth_CG_Init(void) {
	loadCL_GUID();
	
	initHUD();
	
	srand(time(NULL));
	initActions();

	// Init stats
	eth.lastKillTime = 0;
	eth.firstKillSpreeTime = 0;

	#ifdef ETH_DEBUG
		// Init debug view vars
		eth.offsetSub = 0;
		eth.offsetMul = 0;
	#endif

	// Init eth medias
	registerEthMedias();
	
	//syscall_CG_S_StartLocalSound(eth.sounds[SOUND_PREPARE], CHAN_AUTO); // TODO: not work ??? wtf ?

	initActions();
	resetAllActions();

	// If we do \disconnect and then \connect without quitting the game, update our buddy slot
	ircCGinit();
}

void eth_CG_Shutdown(void) {
	setAction(ACTION_BINDMOUSE1, 1);
	blockMouse(qfalse);	// To avoid sensitivity 0
}

// This function was hook only to force sniper view
void eth_CG_FinishWeaponChange(int lastweap, int newweap) {
	qboolean callOriginal = qfalse;
	
	if (!((lastweap == WP_K43_SCOPE && newweap == WP_K43)
			|| (lastweap == WP_GARAND_SCOPE && newweap == WP_GARAND)
			|| (lastweap == WP_FG42SCOPE && newweap == WP_FG42)))
		callOriginal = qtrue;

	int weapon = lastweap;
	switch (weapon) {
		case WP_K43_SCOPE:
			weapon = WP_K43;
			break;
		case WP_GARAND_SCOPE:
			weapon = WP_GARAND;
			break;
		case WP_FG42SCOPE:
			weapon = WP_FG42;
			break;
		default:
			break;
	}

	if (eth.cg_snap->ps.ammoclip[weapon] == 0)
		callOriginal = qtrue;

	if (isKeyActionDown("+reload") || isKeyActionDown("weapalt"))
		callOriginal = qtrue;

	if (callOriginal)
		eth.CG_FinishWeaponChange(lastweap, newweap);
}

// Hooked for catching kills
void eth_CG_EntityEvent (centity_t *cent, vec3_t position) {
	entityState_t *es = &cent->currentState;;
	int event = es->event & ~EV_EVENT_BITS;

	switch (event) {
		case EV_OBITUARY:
			// Don't count kills in warmup.
			if (eth.cgs_gamestate != GS_PLAYING)
				break;
			// For now this way, report if it doesen't work on some mods
			int target, attacker;
			if (!strcmp(eth.mod.name, "etpro")) {
				target = es->time;
				attacker = es->time2;
			}
			else {
				target = es->otherEntityNum;
				attacker = es->otherEntityNum2;
			}
			strcpy(eth.VictimName, eth.clientInfo[target].name);
			if (attacker == eth.cg_clientNum) {
				eth.killCount++;
				eth.killCountNoDeath++;
				// If not a killing spree
				if ((eth.cg_time - eth.lastKillTime) > (SPREE_TIME * 1000)) {
					eth.firstKillSpreeTime = eth.cg_time;
					eth.killSpreeCount = 1;
				} else {
					eth.killSpreeCount++;
				}
				eth.lastKillTime = eth.cg_time;
				if (eth.killSpreeCount > 1)
					playSpreeSound();
				if (seth.value[VAR_KILLSPAM])
					killSpam();
			}
			if (target == eth.cg_clientNum) {
				eth.killCountNoDeath = 0;
				eth.killSpreeCount = 0;
			}
			break;
		default:
			break;
	}
	eth.CG_EntityEvent(cent, position);
}

void eth_CG_DamageFeedback(int yawByte, int pitchByte, int damage) {}

/*
==============================
cvars unlocker
==============================
*/

cvar_t *eth_Cvar_Set2(const char *var_name, const char *value, qboolean force) {
	#ifdef ETH_DEBUG
		ethDebug("cvar: [%s] forced to [%s]", var_name, value);
	#endif
	return orig_Cvar_Set2(var_name, value, qtrue);
}
