/*
	Effect by ShortCircuit908
*/

#include <stdafx.h>

static bool hasKilled = false;

static void OnTick()
{
	if (hasKilled) {
		return;
	}

	Ped playerPed = PLAYER_PED_ID();
	if (IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
	{
		float maxRange = GET_MAX_RANGE_OF_CURRENT_PED_WEAPON(playerPed);
		Vector3 playerCoord = GET_ENTITY_COORDS(playerPed, false);
		for (Ped ped : GetAllPeds())
		{
			REQUEST_PED_VISIBILITY_TRACKING(ped);
			if (ped != playerPed && IS_TRACKED_PED_VISIBLE(ped) && !IS_PED_DEAD_OR_DYING(ped, true) && IS_PED_HUMAN(ped) && !IS_PED_A_PLAYER(ped) && IS_TRACKED_PED_VISIBLE(ped))
			{
				Vector3 pedCoord = GET_ENTITY_COORDS(ped, 0);
				float distance = GET_DISTANCE_BETWEEN_COORDS(playerCoord.x, playerCoord.y, playerCoord.z, pedCoord.x, pedCoord.y, pedCoord.z, true);
				if (distance <= maxRange)
				{
					Vector3 headVector = GET_PED_BONE_COORDS(ped, 0x796E, 0, 0, 0); // Head
					SET_PED_SHOOTS_AT_COORD(playerPed, headVector.x, headVector.y, headVector.z, true);
					SET_ENTITY_HEALTH(ped, 0, false); // Ensure deadness
					hasKilled = true;
					break;
				}
			}
		}
	}
}

static void OnStop()
{
	hasKilled = false;
}

static RegisterEffect registerEffect(EFFECT_PLAYER_KILLS_NEAREST_PED, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Kill Nearest Ped",
		.Id = "player_kill_nearest_ped",
		.IsTimed = true
	}
);