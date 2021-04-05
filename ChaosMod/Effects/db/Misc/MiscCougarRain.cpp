#include <stdafx.h>

static void OnTick()
{
	static constexpr int MAX_COUGARS = 20;
	static const Hash COUGAR_MODEL = GET_HASH_KEY("a_c_mtlion");

	static Ped cougars[MAX_COUGARS] = {};
	static int cougarAmount = 0;

	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (cougarAmount <= MAX_COUGARS && curTick > lastTick + 200)
	{
		lastTick = curTick;

		Vector3 spawnPos = Vector3::Init(
			playerPos.x + g_random.GetRandomInt(-100, 100),
			playerPos.y + g_random.GetRandomInt(-100, 100),
			playerPos.z + g_random.GetRandomInt(5, 10)
		);

		LoadModel(COUGAR_MODEL);

		Ped cougar = CREATE_PED(28, COUGAR_MODEL, spawnPos.x, spawnPos.y, spawnPos.z, g_random.GetRandomInt(0, 359), true, false);

		cougarAmount++;
		for (int i = 0; i < MAX_COUGARS; i++)
		{
			Ped& ped = cougars[i];
			if (!ped)
			{
				ped = cougar;
				break;
			}
		}

		SET_ENTITY_INVINCIBLE(cougar, true);

		SET_MODEL_AS_NO_LONGER_NEEDED(COUGAR_MODEL);
	}

	static DWORD64 lastTick2 = 0;
	for (int i = 0; i < MAX_COUGARS; i++)
	{
		Ped& cougar = cougars[i];
		if (cougar)
		{
			if (DOES_ENTITY_EXIST(cougar) && !IS_PED_DEAD_OR_DYING(cougar, true))
			{
				if (HAS_ENTITY_COLLIDED_WITH_ANYTHING(cougar)) {
					SET_ENTITY_INVINCIBLE(cougar, false);
				}
				continue;
			}
			else {
				cougars[i] = 0;
			}

			cougarAmount--;

			SET_OBJECT_AS_NO_LONGER_NEEDED(&cougar);

			cougar = 0;
		}
	}

	if (lastTick2 < curTick - 1000)
	{
		lastTick2 = curTick;
	}
}

static RegisterEffect registerEffect(EFFECT_COUGAR_RAIN, nullptr, nullptr, OnTick, EffectInfo
	{
		.Name = "Cougar Rain",
		.Id = "world_cougarrain",
		.IsTimed = true
	}
);