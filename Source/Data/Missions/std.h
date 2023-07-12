//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

module ScenarioBrain1 : integer;

	const

		SCENARIO_TIMER_1			= 7;
		SCENARIO_TIMER_2			= 8;
		SCENARIO_TIMER_3			= 9;
		SCENARIO_TIMER_4			= 10;
		SCENARIO_TIMER_5			= 11;
		SCENARIO_TIMER_6			= 12;
		SCENARIO_TIMER_7			= 13;
		SCENARIO_TIMER_8			= 14;

		//----------------
		// Objective Type
		//----------------
		InvisibleGoal				= -1;
		PrimaryGoal					= 0;
		SecondaryGoal				= 1;
		OtherGoal					= 2;
		InvalidGoal					= 9999;

		//------------------
		// Objective Status
		//------------------
		Incomplete					= 0;
		Success						= 1;
		Failed						= 2;
		InvalidStatus				= 9999;

		//---------------
		// Objective IDs
		//---------------
		OBJECTIVE1                  = 0;
		OBJECTIVE2                  = 1;
		OBJECTIVE3                  = 2;
		OBJECTIVE4                  = 3;
		OBJECTIVE5                  = 4;
		OBJECTIVE6                  = 5;
		OBJECTIVE7                  = 6;
		OBJECTIVE8                  = 7;

		DESTROYED                   = 0;
		DAMAGED                     = 1;
		EXISTS                      = 2;
		VISUAL_RANGE                = 3;
		SPEC_RANGE                  = 4;
		NONE                        = 5;

		N/A                         = 1;
		SUCCESS                     = 6;
		PARTIAL                     = 5;
		FAIL                        = 3;
		RESOLVE                     = 7;

		//------------------
		// Scenario Results
		//------------------
		PLAYING						= 0;
		PLAYER_LOST_BIG				= 1;
		PLAYER_LOST_SMALL			= 2;
		PLAYER_DRAW					= 3;
		PLAYER_WIN_SMALL			= 4;
		PLAYER_WIN_BIG				= 5;

		//-------------------------
		// Music IDs
		//-------------------------
		MISSION_FAILED				= 9;
		MISSION_SUCCESS				= 8;
		OBJECTIVE_FAILED			= 11;
		OBJECTIVE_SUCCESS			= 10;

		//-------------------------
		// Alignments (Sides)
		CLAN						= -1;
		NEUTRAL						= 0;
		INNER_SPHERE				= 1;

		//-------------------------
		// Group Objects
		//-------------------------

		PLAYER_LANCE0				= 192;
		PLAYER_LANCE1				= 193;
		PLAYER_LANCE2				= 194;
		ALLIED_LANCE0				= 195;
		ALLIED_LANCE1				= 196;
		ALLIED_LANCE2				= 197;
		CLAN_STAR0					= 198;
		CLAN_STAR1					= 199;
		CLAN_STAR2					= 200;

		PLAYER_FORCE				= 128;
		ALLIED_FORCE				= 129;
		CLAN_FORCE					= 130;
