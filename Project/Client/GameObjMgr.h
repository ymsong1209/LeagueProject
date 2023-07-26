#pragma once

class CPlayerScript;
class CGameObject;

class GameObjMgr
{
	map<uint16, CGameObject*> _players;
	// map<uint16, CGameObject> _monsters;

	CPlayerScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript의 prevPos는 매 틱마다 이전좌표고, 이건 1/10초전 좌표.


public:
	CGameObject* Find(uint16 _id);
	void AddPlayer(PlayerInfo info, bool myPlayer = false);
	void E_MovePlayer(uint16 _playerId, PlayerMove _playerMove);


public:
	// 서버에 보낸다.
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

