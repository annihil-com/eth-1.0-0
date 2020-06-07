// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credits us

/*
==============================
All offsets for supported mods

Only offsets in this file
==============================
*/

#ifndef HOOK_H_
#define HOOK_H_

#define AUTO_OFFSET -1

const static ethMod_t ethMods[] = {
	{ "etmain",	"2.55", 0x1ae3ee02,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2676,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d44,		// cg.refdef
		0x49ef0,		// cg.refdefviewangles
	},
	{ "etmain",	"2.56", 0x4153a646,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2676,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d44,		// cg.refdef
		0x49ef0,		// cg.refdefviewangles
	},
	{ "etmain",	"2.60", 0xcb26a1fd,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2676,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d44,		// cg.refdef
		0x49ef0,		// cg.refdefviewangles
	},
	{ "etpro",	"3.2.4", 0x6a96361f,
		0x44cd000,	// cg
		0x453b360, 2700,	// cg_entities
		0x75ef0,	// CG_Trace()
		0xa8940,	// BG_EvaluateTrajectory()
		0x4a0f0,	// CG_EntityEvent()
		0x964b0,	// CG_FinishWeaponChange()
		0x738f0,	// CG_DamageFeedback()
		0x92ab0,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d18,	// cg.refdef
		0x49ec4,	// cg.refdefviewangles
		// etpro guid_function
		0xa100,
	},
	{ "etpro",	"3.2.5", 0xc665c831,
		0x44cd000,	// cg
		0x453b360, 2700,	// cg_entities
		0x75ef0,	// CG_Trace()
		0xa8940,	// BG_EvaluateTrajectory()
		0x4a0f0,	// CG_EntityEvent()
		0x964b0,	// CG_FinishWeaponChange()
		0x738f0,	// CG_DamageFeedback()
		0x92ab0,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d18,	// cg.refdef
		0x49ec4,	// cg.refdefviewangles
		// etpro guid_function
		0xa100
	},
	{ "etpro",	"3.2.6-beta1", 0x2726f60c,
		0x44ce340,	// cg
		0x453c6a0, 2700,	// cg_entities
		0x762f0,	// CG_Trace()
		0xa8d70,	// BG_EvaluateTrajectory()
		0x4a4b0,	// CG_EntityEvent()
		0x968b0,	// CG_FinishWeaponChange()
		0x73cb0,	// CG_DamageFeedback()
		0x92eb0,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d18,	// cg.refdef
		0x49ec4,	// cg.refdefviewangles
		// etpro guid_function
		0xa360
	},
	{ "etpub",	"20051128", 0x3f93ed44,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d5c,	// cg.refdef
		0x49f08,	// cg.refdefviewangles
	},
	{ "etpub",	"20060205", 0x1e82f5d2,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d5c,	// cg.refdef
		0x49f08,	// cg.refdefviewangles
	},
	{ "etpub",	"20060304", 0x79c4963a,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	},
	{ "etpub",	"20060310", 0x7077b613,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	},
	{ "etpub",	"20060606", 0x95520381,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	},
	{ "jaymod",	"1.4", 0xbd9bb6ad,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d5c,	// cg.refdef
		0x49f08,	// cg.refdefviewangles
	},
	{ "jaymod",	"1.5-beta4", 0xb72cc38e,
		0xe4f520,	// cg
		0x29407c0, 2704,	// cg_entities
		0x6c7c0,	// CG_Trace()
		0xa8fa0,	// BG_EvaluateTrajectory()
		0x37700,	// CG_EntityEvent()
		0x91a90,	// CG_FinishWeaponChange()
		0x694b0,	// CG_DamageFeedback()
		0x8cd80,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	},
	{ "jaymod",	"1.5-beta5", 0x3ffcf1a4,
		0xe4f520,	// cg
		0x29407c0, 2704,	// cg_entities
		0x6c7c0,	// CG_Trace()
		0xa8fa0,	// BG_EvaluateTrajectory()
		0x37700,	// CG_EntityEvent()
		0x91a90,	// CG_FinishWeaponChange()
		0x694b0,	// CG_DamageFeedback()
		0x8cd80,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	},
	{ "jaymod",	"2.0.1", 0xc2051741,
		0xdd4960,	// cg
		0x28d6180, 2704,	// cg_entities
		0x692f0,	// CG_Trace()
		0xa2b70,	// BG_EvaluateTrajectory()
		0x3c360,	// CG_EntityEvent()
		0x8c450,	// CG_FinishWeaponChange()
		0x66170,	// CG_DamageFeedback()	7f430 7f400 7f3d0 7e6e0 7e270 7e160
		0x87c40,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	},
	{ "whale",	"1.3", 0x9285202,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2676,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d48,	// cg.refdef
		0x49ef4,	// cg.refdefviewangles
	},
	{ "whale",	"1.4-beta1", 0xe62c638e,
		AUTO_OFFSET,	// cg
		AUTO_OFFSET, 2700,	// cg_entities
		AUTO_OFFSET,	// CG_Trace()
		AUTO_OFFSET,	// BG_EvaluateTrajectory()
		AUTO_OFFSET,	// CG_EntityEvent()
		AUTO_OFFSET,	// CG_FinishWeaponChange()
		AUTO_OFFSET,	// CG_DamageFeedback()
		AUTO_OFFSET,	// CG_RailTrail2
		// cg_t structs offsets
		0x49d60,	// cg.refdef
		0x49f0c,	// cg.refdefviewangles
	}
};

// centity_t structs offsets
#define CG_ENTITIES_CURRENTSTATE_OFFSET 0x0
#define CG_ENTITIES_CURRENTVALID_OFFSET 0x244

/*
==============================
et.x86 addresses
==============================
*/

const static ethET_t ethET[] = {
	{	"2.55",		// version
		0x21e60afb,	// crc32
		0x080bae24, // Cvar_Set2
		0x080b561c,	// Cmd_AddCommand
		0x0804cbb4,	// COM_StripExtension
		0x080c1d68,	// FS_PureServerSetLoadedPaks
		0x087cd9c8,	// fs_searchpaths
		0x08f451d0,	// serverIP
		0x08ffc068,	// cl_mouseDx
		0x08ffc06c,	// cl_mouseDy
		#ifdef ETH_PRIVATE
			PRIV_ET255
		#endif
	},
	{	"2.56",		// version
		0x3d59a703,	// crc32
		0x080ba89c, // Cvar_Set2
		0x080b5098,	// Cmd_AddCommand
		0x0804cc34,	// COM_StripExtension
		0x080c17e0,	// FS_PureServerSetLoadedPaks
		0x087cdfa8,	// fs_searchpaths
		0x08f457d0,	// serverIP
		0x08ffc668,	// cl_mouseDx
		0x08ffc66c,	// cl_mouseDy
		#ifdef ETH_PRIVATE
			PRIV_ET256
		#endif
	},
	{	"2.60",		// version
		0x3b18a889,	// crc32
		0x08070010, // Cvar_Set2
		0x08069240,	// Cmd_AddCommand
		0x08097880,	// COM_StripExtension
		0x08076570,	// FS_PureServerSetLoadedPaks
		0x0888c7c8,	// fs_searchpaths
		0x09036108,	// serverIP
		0x0906c1c8,	// cl_mouseDx
		0x0906c1cc,	// cl_mouseDy
		#ifdef ETH_PRIVATE
			PRIV_ET260
		#endif
	},
	{	"2.60b",	// version
		0x6ab49f82,	// crc32
		0x08073160, // Cvar_Set2
		0x0806b640,	// Cmd_AddCommand
		0x0,		// COM_StripExtension
		0x0807cef0,	// FS_PureServerSetLoadedPaks
		0x088b1f18,	// fs_searchpaths
		0x090471c8,	// serverIP
		0x0907d288,	// cl_mouseDx
		0x0907d28c,	// cl_mouseDy
		#ifdef ETH_PRIVATE
			PRIV_ET260B
		#endif
	}
};

#endif // HOOK_H_
