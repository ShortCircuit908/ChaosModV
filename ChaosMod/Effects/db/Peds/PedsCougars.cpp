#include <stdafx.h>


static void OnTick()
{
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

			if (++done > 10) {
				break;
			}
		}
	}
}

static RegisterEffect registerEffect(EFFECT_PEDS_COUGARS, nullptr, nullptr, OnTick, EffectInfo
	{
		.Name = "Single Cougars In Your Area",
		.Id = "peds_cougars",
		.IsTimed = true,
		.IncompatibleWith = {EFFECT_PEDS_COPS}
	}
);