#include "eth.h"

/*
==============================
 Eth medias register
==============================
*/

void registerEthMedias() {
	// Register all shaders chams
	#include "shaders.h"
	
	// Custom models/skins
	eth.pigHeadModel = orig_syscall(CG_R_REGISTERMODEL, "models/players/hud/pig.md3");
	eth.pigHeadSkin = orig_syscall(CG_R_REGISTERSKIN, "models/players/hud/pig.skin");

	// Irc fireteam team pin
	eth.bg_alliedpin = syscall_CG_R_RegisterShaderNoMip("gfx/2d/pin_allied");
	eth.bg_axispin = syscall_CG_R_RegisterShaderNoMip("gfx/2d/pin_axis");
	
	// All fonts used
	orig_syscall(CG_R_REGISTERFONT, "ariblk", 27, &eth.fontVariable);
	orig_syscall(CG_R_REGISTERFONT, "courbd", 21, &eth.fontFixed);

	// Custom HUD gfx, fonts
	registerHudStuff();
	
	// ----- Custom sounds -----
	// Misc sound
	eth.sounds[SOUND_HUMILIATION] = syscall_CG_S_RegisterSound("sound/misc/humiliation.wav", qfalse);
	eth.sounds[SOUND_PREPARE] = syscall_CG_S_RegisterSound("sound/misc/prepare.wav", qfalse);
	eth.sounds[SOUND_HEADSHOT1] = syscall_CG_S_RegisterSound("sound/misc/headshot1.wav", qfalse);
	eth.sounds[SOUND_HEADSHOT2] = syscall_CG_S_RegisterSound("sound/misc/headshot2.wav", qfalse);
	// Spree sound
	// level 1
	eth.sounds[SOUND_DOUBLEKILL1] = syscall_CG_S_RegisterSound("sound/spree/level1/doublekill1.wav", qfalse);
	eth.sounds[SOUND_EXCELLENT] = syscall_CG_S_RegisterSound("sound/spree/level1/excelent.wav", qfalse);
	eth.sounds[SOUND_IMPRESSIVE] = syscall_CG_S_RegisterSound("sound/spree/level1/impressive.wav", qfalse);
	eth.sounds[SOUND_KILLINGSPREE1] = syscall_CG_S_RegisterSound("sound/spree/level1/killingspree1.wav", qfalse);
	eth.sounds[SOUND_KILLINGSPREE2] = syscall_CG_S_RegisterSound("sound/spree/level1/killingspree2.wav", qfalse);
	eth.sounds[SOUND_PERFECT] = syscall_CG_S_RegisterSound("sound/spree/level1/perfect.wav", qfalse);
	// level 2
	eth.sounds[SOUND_GODLIKE1] = syscall_CG_S_RegisterSound("sound/spree/level2/godlike1.wav", qfalse);
	eth.sounds[SOUND_GODLIKE2] = syscall_CG_S_RegisterSound("sound/spree/level2/godlike2.wav", qfalse);
	eth.sounds[SOUND_HOLYSHIT] = syscall_CG_S_RegisterSound("sound/spree/level2/holyshit.wav", qfalse);
	eth.sounds[SOUND_MULTIKILL1] = syscall_CG_S_RegisterSound("sound/spree/level2/multikill1.wav", qfalse);
	eth.sounds[SOUND_MULTIKILL2] = syscall_CG_S_RegisterSound("sound/spree/level2/multikill2.wav", qfalse);
	eth.sounds[SOUND_MULTIKILL3] = syscall_CG_S_RegisterSound("sound/spree/level2/multikill3.wav", qfalse);
	eth.sounds[SOUND_TRIPLEKILL] = syscall_CG_S_RegisterSound("sound/spree/level2/tripplekill.wav", qfalse);
	// level 3
	eth.sounds[SOUND_DOMINATING1] = syscall_CG_S_RegisterSound("sound/spree/level3/dominating1.wav", qfalse);
	eth.sounds[SOUND_DOMINATING2] = syscall_CG_S_RegisterSound("sound/spree/level3/dominating2.wav", qfalse);
	eth.sounds[SOUND_ULTRAKILL1] = syscall_CG_S_RegisterSound("sound/spree/level3/ultrakill1.wav", qfalse);
	eth.sounds[SOUND_ULTRAKILL2] = syscall_CG_S_RegisterSound("sound/spree/level3/ultrakill2.wav", qfalse);
	eth.sounds[SOUND_ULTRAKILL3] = syscall_CG_S_RegisterSound("sound/spree/level3/ultrakill3.wav", qfalse);
	// level 4
	eth.sounds[SOUND_MONSTERKILL1] = syscall_CG_S_RegisterSound("sound/spree/level4/monsterkill1.wav", qfalse);
	eth.sounds[SOUND_MONSTERKILL2] = syscall_CG_S_RegisterSound("sound/spree/level4/monsterkill2.wav", qfalse);
	eth.sounds[SOUND_UNREAL] = syscall_CG_S_RegisterSound("sound/spree/level4/unreal.wav", qfalse);
	eth.sounds[SOUND_UNSTOPPABLE1] = syscall_CG_S_RegisterSound("sound/spree/level4/unstoppable1.wav", qfalse);
	eth.sounds[SOUND_UNSTOPPABLE2] = syscall_CG_S_RegisterSound("sound/spree/level4/unstoppable2.wav", qfalse);
	eth.sounds[SOUND_WICKEDSICK] = syscall_CG_S_RegisterSound("sound/spree/level4/wickedsick.wav", qfalse);

	// Rank Icons esp
	eth.rankIcons[0] = syscall_CG_R_RegisterShaderNoMip("gfx/loading/pin_neutral");
	eth.rankIcons[1] = syscall_CG_R_RegisterShaderNoMip("gfx/loading/pin_neutral");
	eth.rankIcons[2] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank2");
	eth.rankIcons[3] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank3");
	eth.rankIcons[4] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank4");
	eth.rankIcons[5] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank5");
	eth.rankIcons[6] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank6");
	eth.rankIcons[7] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank7");
	eth.rankIcons[8] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank8");
	eth.rankIcons[9] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank9");
	eth.rankIcons[10] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank10");
	eth.rankIcons[11] = syscall_CG_R_RegisterShaderNoMip("gfx/hud/ranks/rank11");

	// Class Icons esp
	eth.classIcons[PC_SOLDIER] = syscall_CG_R_RegisterShaderNoMip("gfx/limbo/ic_soldier");
	eth.classIcons[PC_MEDIC] = syscall_CG_R_RegisterShaderNoMip("gfx/limbo/ic_medic");
	eth.classIcons[PC_ENGINEER] = syscall_CG_R_RegisterShaderNoMip("gfx/limbo/ic_engineer");
	eth.classIcons[PC_FIELDOPS] = syscall_CG_R_RegisterShaderNoMip("gfx/limbo/ic_fieldops");
	eth.classIcons[PC_COVERTOPS] = syscall_CG_R_RegisterShaderNoMip("gfx/limbo/ic_covertops");

	// Weapon Icons esp
	eth.weaponIcons[MOD_BROWNING] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_browning_1_select");
	eth.weaponIcons[WP_MORTAR] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mortar_1_select");
	eth.weaponIcons[WP_AKIMBO_SILENCEDLUGER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	eth.weaponIcons[WP_AKIMBO_LUGER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	eth.weaponIcons[WP_AKIMBO_COLT] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	eth.weaponIcons[WP_AKIMBO_SILENCEDCOLT] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	eth.weaponIcons[WP_NONE] = syscall_CG_R_RegisterShaderNoMip("icons/noammo");
	eth.weaponIcons[WP_SMOKE_MARKER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_smokegrenade_1_select");
	eth.weaponIcons[WP_SMOKETRAIL] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_smokegrenade_1_select");
	eth.weaponIcons[WP_MAPMORTAR] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_grenade_1_select");
	eth.weaponIcons[WP_ARTY] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_syringe_1_select");
	eth.weaponIcons[WP_K43_SCOPE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	eth.weaponIcons[WP_GPG40] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_kar98_1_select");
	eth.weaponIcons[WP_M7] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_m1_garand_1_select");
	eth.weaponIcons[WP_GARAND_SCOPE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	eth.weaponIcons[WP_FG42SCOPE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_fg42_1_select");
	eth.weaponIcons[WP_MORTAR_SET] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mortar_1_select");
	eth.weaponIcons[WP_SMOKE_BOMB] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_dynamite_1_select");
	eth.weaponIcons[WP_TRIPMINE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_dynamite_1_select");
	eth.weaponIcons[WP_MOBILE_MG42_SET] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mg42_1_select");
	eth.weaponIcons[WP_SILENCED_COLT] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	eth.weaponIcons[WP_MEDKIT] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_medheal_select");
	eth.weaponIcons[WP_MOBILE_MG42] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mg42_1_select");
	eth.weaponIcons[WP_THOMPSON] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_thompson_1_select");
	eth.weaponIcons[WP_KNIFE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_knife_1_select");
	eth.weaponIcons[WP_LUGER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_luger_1_select");
	eth.weaponIcons[WP_MP40] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_MP40_1_select");
	eth.weaponIcons[WP_GRENADE_LAUNCHER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_grenade_1_select");
	eth.weaponIcons[WP_PANZERFAUST] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_panzerfaust_1_select");
	eth.weaponIcons[WP_FLAMETHROWER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_flamethrower_1_select");
	eth.weaponIcons[WP_COLT] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	eth.weaponIcons[WP_GRENADE_PINEAPPLE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_pineapple_1_select");
	eth.weaponIcons[WP_STEN] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_sten_1_select");
	eth.weaponIcons[WP_MEDIC_SYRINGE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_syringe_1_select");
	eth.weaponIcons[WP_AMMO] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_ammopack_1_select");
	eth.weaponIcons[WP_SILENCER] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_silencer_1_select");
	eth.weaponIcons[WP_DYNAMITE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_dynamite_1_select");
	eth.weaponIcons[WP_MEDKIT] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_medheal_1_select");
	eth.weaponIcons[WP_BINOCULARS] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_binoculars_1_select");
	eth.weaponIcons[WP_PLIERS] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_pliers_1_select");
	eth.weaponIcons[WP_KAR98] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_kar98_1_select");
	eth.weaponIcons[WP_CARBINE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	eth.weaponIcons[WP_GARAND] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_m1_garand_1_select");
	eth.weaponIcons[WP_LANDMINE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_landmine_1_select");
	eth.weaponIcons[WP_SATCHEL] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_satchel_1_select");
	eth.weaponIcons[WP_SATCHEL_DET] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_radio_1_select");
	eth.weaponIcons[WP_K43] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	eth.weaponIcons[WP_FG42] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_fg42_1_select");
	eth.weaponIcons[WP_MEDIC_ADRENALINE] = syscall_CG_R_RegisterShaderNoMip("icons/iconw_syringe_1_select");

	// Nice q3 numbers
	eth.number[0] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/zero_32b");
	eth.number[1] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/one_32b");
	eth.number[2] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/two_32b");
	eth.number[3] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/three_32b");
	eth.number[4] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/four_32b");
	eth.number[5] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/five_32b");
	eth.number[6] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/six_32b");
	eth.number[7] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/seven_32b");
	eth.number[8] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/eight_32b");
	eth.number[9] = syscall_CG_R_RegisterShaderNoMip("gfx/2d/numbers/nine_32b");
}
