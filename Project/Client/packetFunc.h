
#pragma once


enum class CHAMPION_TYPE
{
	JINX,
	BLITZCRANL,
	AMUMI,

	END
};


void SpawnMyChampion(CHAMPION_TYPE _type, Vec3 _vWorldPos);

void SpawnOtherChampion(CHAMPION_TYPE _type, Vec3 _vWorldPos);
