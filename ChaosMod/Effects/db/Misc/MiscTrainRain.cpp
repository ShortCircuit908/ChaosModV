#include <stdafx.h>

//Effect by ShortCircuit908

static void OnTick()
{
	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (curTick > lastTick + 500)
	{
		lastTick = curTick;

		Vector3 spawnPos;
		spawnPos.x = playerPos.x + g_random.GetRandomInt(-100, 100);
		spawnPos.y = playerPos.y + g_random.GetRandomInt(-100, 100);
		spawnPos.z = playerPos.z + g_random.GetRandomInt(25, 50);


		static std::vector<Hash> vehModels = { GET_HASH_KEY("FREIGHT")/*, GET_HASH_KEY("FREIGHTCAR"), GET_HASH_KEY("FREIGHTCONT1"), GET_HASH_KEY("FREIGHTCONT2"), GET_HASH_KEY("FREIGHTGRAIN"), GET_HASH_KEY("TANKERCAR"), GET_HASH_KEY("METROTRAIN")*/ };

		Vehicle veh = CreateTempVehicle(vehModels[g_random.GetRandomInt(0, vehModels.size() - 1)], spawnPos.x, spawnPos.y, spawnPos.z, g_random.GetRandomInt(0, 359));

		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(veh, 0, g_random.GetRandomInt(-5, 5), -5, g_random.GetRandomInt(-5, 5), true, false, true, true);
	}
}

static RegisterEffect registerEffect(EFFECT_MISC_TRAIN_RAIN, nullptr, nullptr, OnTick, EffectInfo
	{
		.Name = "Train Rain",
		.Id = "misc_train_rain",
		.IsTimed = true,
		.IsShortDuration = true
	}
);