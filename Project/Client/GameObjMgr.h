#pragma once

class CPlayerScript;
class CGameObject;

class GameObjMgr
{
	CPlayerScript* MyPlayerScript;

	map<uint16, CGameObject*> _players;
	// map<uint16, CGameObject> _monsters;

public:
	CGameObject* Find(uint16 _id);
	void AddPlayer(PlayerInfo info, bool myPlayer = false);
	void MovePlayer(uint16 _playerId, PlayerMove _playerMove);


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

