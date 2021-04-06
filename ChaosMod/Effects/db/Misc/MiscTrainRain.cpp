#include <stdafx.h>

//Effect by ShortCircuit908

static void OnTick()
{
	static constexpr int MAX_TRAINS = 20;
	
	static Vehicle trains[MAX_TRAINS];
	static int trainDespawnTime[MAX_TRAINS];
	static int trainAmount = 0;

	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (trainAmount < MAX_TRAINS && curTick > lastTick + 500)
	{
		lastTick = curTick;

		Vector3 spawnPos;
		spawnPos.x = playerPos.x + g_random.GetRandomInt(-100, 100);
		spawnPos.y = playerPos.y + g_random.GetRandomInt(-100, 100);
		spawnPos.z = playerPos.z + g_random.GetRandomInt(25, 50);


		static std::vector<Hash> vehModels = { GET_HASH_KEY("FREIGHT")/*, GET_HASH_KEY("FREIGHTCAR"), GET_HASH_KEY("FREIGHTCONT1"), GET_HASH_KEY("FREIGHTCONT2"), GET_HASH_KEY("FREIGHTGRAIN"), GET_HASH_KEY("TANKERCAR"), GET_HASH_KEY("METROTRAIN")*/ };

		Vehicle veh = CreateTempVehicle(vehModels[g_random.GetRandomInt(0, vehModels.size() - 1)], spawnPos.x, spawnPos.y, spawnPos.z, g_random.GetRandomInt(0, 359));

		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(veh, 0, g_random.GetRandomInt(-5, 5), -5, g_random.GetRandomInt(-5, 5), true, false, true, true);

		trainAmount++;

		for (int i = 0; i < MAX_TRAINS; i++)
		{
			Vehicle& train = trains[i];
			if (!train)
			{
				train = veh;
				trainDespawnTime[i] = 5;
				break;
			}
		}
	}

	static DWORD64 lastTick2 = 0;
	for (int i = 0; i < MAX_TRAINS; i++)
	{
		Vehicle& train = trains[i];
		if (train)
		{
			if (DOES_ENTITY_EXIST(train) && trainDespawnTime[i] > 0)
			{
				Vector3 propPos = GET_ENTITY_COORDS(train, false);
				if (GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, propPos.x, propPos.y, propPos.z, true) < 400.f)
				{
					if (HAS_ENTITY_COLLIDED_WITH_ANYTHING(train))
					{
						if (lastTick2 < curTick - 1000)
						{
							trainDespawnTime[i]--;
						}
					}
					continue;
				}
			}

			trainAmount--;

			SET_ENTITY_AS_MISSION_ENTITY(train, true, true);
			DELETE_VEHICLE(&train);

			train = 0;
		}
	}

	if (lastTick2 < curTick - 1000)
	{
		lastTick2 = curTick;
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