#include <stdafx.h>

static void OnStart()
{
	Hash groupHash;
	ADD_RELATIONSHIP_GROUP("_COUGAR_ATTACK_PLAYER", &groupHash);
}

static void OnTick()
{
	static const Hash enemyGroupHash = GET_HASH_KEY("_ATTACK_PLAYER");
	static const Hash playerGroupHash = GET_HASH_KEY("PLAYER");

	SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroupHash, playerGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, enemyGroupHash);

	Player player = PLAYER_ID();
	Ped playerPed = PLAYER_PED_ID();
	int playerGroup = GET_PLAYER_GROUP(player);

	static constexpr int MAX_COUGARS = 20;
	static const Hash COUGAR_MODEL = GET_HASH_KEY("a_c_mtlion");

	static Ped cougars[MAX_COUGARS] = {};
	static int cougarDespawnTime[MAX_COUGARS];
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
			playerPos.z + g_random.GetRandomInt(25, 50)
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
				cougarDespawnTime[i] = 5;
				break;
			}
		}

		SET_ENTITY_INVINCIBLE(cougar, true);

		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(cougar, 0, 35.f, 0, -5000.f, true, false, true, true);

		SET_MODEL_AS_NO_LONGER_NEEDED(COUGAR_MODEL);
	}

	static DWORD64 lastTick2 = 0;
	for (int i = 0; i < MAX_COUGARS; i++)
	{
		Ped& cougar = cougars[i];
		if (cougar)
		{
			if (DOES_ENTITY_EXIST(cougar) && cougarDespawnTime[i] > 0)
			{
				if (IS_PED_IN_GROUP(cougar) && GET_PED_GROUP_INDEX(cougar) == playerGroup)
				{
					REMOVE_PED_FROM_GROUP(cougar);
				}

				SET_PED_RELATIONSHIP_GROUP_HASH(cougar, enemyGroupHash);

				SET_PED_COMBAT_ATTRIBUTES(cougar, 5, true);
				SET_PED_COMBAT_ATTRIBUTES(cougar, 46, true);

				SET_PED_FLEE_ATTRIBUTES(cougar, 2, true);

				TASK_COMBAT_PED(cougar, playerPed, 0, 16);

				Vector3 propPos = GET_ENTITY_COORDS(cougar, false);
				if (GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, propPos.x, propPos.y, propPos.z, true) < 400.f)
				{
					if (HAS_ENTITY_COLLIDED_WITH_ANYTHING(cougar))
					{
						SET_ENTITY_INVINCIBLE(cougar, false);

						if (lastTick2 < curTick - 1000)
						{
							cougarDespawnTime[i]--;
						}
					}
					continue;
				}
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

static RegisterEffect registerEffect(EFFECT_COUGAR_RAIN, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Cougar Rain",
		.Id = "world_cougarrain",
		.IsTimed = true
	}
);