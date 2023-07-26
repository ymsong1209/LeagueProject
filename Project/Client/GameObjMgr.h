#pragma once

class CPlayerScript;
class CGameObject;

class GameObjMgr
{
	map<uint16, CGameObject*> _players;
	// map<uint16, CGameObject> _monsters;

	CPlayerScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript�� prevPos�� �� ƽ���� ������ǥ��, �̰� 1/10���� ��ǥ.


public:
	CGameObject* Find(uint16 _id);
	void AddPlayer(PlayerInfo info, bool myPlayer = false);
	void E_MovePlayer(uint16 _playerId, PlayerMove _playerMove);


public:
	// ������ ������.
	void SendMyPlayerMove(ClientServiceRef _service);

static GameObjMgr* GetInst()
{
	 static GameObjMgr inst;
	 return &inst;
}
private:
	 GameObjMgr();
	~GameObjMgr();

};

