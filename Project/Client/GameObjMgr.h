#pragma once

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CCameraMoveScript.h>
#include <Engine\CPathFindMgr.h>


class GameObjMgr
{
	CPlayerScript* MyPlayerScript;

	map<uint16, CGameObject> _players;
	// map<uint16, CGameObject> _monsters;

public:
	void AddPlayer(PlayerInfo info, bool myPlayer = false);


public:

static GameObjMgr* GetInst()
{
	 static GameObjMgr inst;
	 return &inst;
}
private:
	 GameObjMgr();
	~GameObjMgr();

};

