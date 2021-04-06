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

	static const Hash COUGAR_MODEL = GET_HASH_KEY("a_c_mtlion");
	int done = 0;
	for (Ped ped : GetAllPeds())
	{
		Ped pedType = GET_PED_TYPE(ped);

		if (!IS_PED_A_PLAYER(ped) && pedType != 0 && pedType != 1 && pedType != 2 && pedType != 28 && !IS_ENTITY_A_MISSION_ENTITY(ped))
		{
			Vector3 pos = GET_ENTITY_COORDS(ped, false);
			Ped cougar = CreatePoolPed(28, COUGAR_MODEL, pos.x, pos.y, pos.z, GET_ENTITY_HEADING(ped));
			SET_ENTITY_COORDS(ped, 0, 0, 0, 1, 0, 0, 1);
			SET_PED_AS_NO_LONGER_NEEDED(&ped);
			DELETE_PED(&ped);

			if (IS_PED_IN_GROUP(cougar) && GET_PED_GROUP_INDEX(cougar) == playerGroup)
			{
				REMOVE_PED_FROM_GROUP(cougar);
			}

			SET_PED_RELATIONSHIP_GROUP_HASH(cougar, enemyGroupHash);

			SET_PED_COMBAT_ATTRIBUTES(cougar, 5, true);
			SET_PED_COMBAT_ATTRIBUTES(cougar, 46, true);

			SET_PED_FLEE_ATTRIBUTES(cougar, 2, true);

			TASK_COMBAT_PED(cougar, playerPed, 0, 16);

			if (++done > 10) {
				break;
			}
		}
	}
}

static RegisterEffect registerEffect(EFFECT_PEDS_COUGARS, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Single Cougars In Your Area",
		.Id = "peds_cougars",
		.IsTimed = true,
		.IncompatibleWith = {EFFECT_PEDS_COPS}
	}
);