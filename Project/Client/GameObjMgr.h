#pragma once

class CPlayerScript;
class CGameObject;

class GameObjMgr
{
	// 전체
	map<uint64, CGameObject*> _players;
	map<uint64, CGameObject*> _objects; // 미니언, 정글몹, 투사체 (시간마다)
	map<uint64, CGameObject*> _towers; // 포탑, 억제기, 넥서스 (맵 처음)


	// My Player 관련
	CPlayerScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript의 prevPos는 매 틱마다 이전좌표고, 이건 1/10초전 좌표.


public:
	CGameObject* FindPlayer(uint64 _id);
	void AddPlayer(PlayerInfo info, bool myPlayer = false);

	void AddObject(uint64 _objectId, ObjectType _objectType, FactionType _factionType);
	// void AddTower();

	void E_MovePlayer(uint64 _playerId, ObjectMove _playerMove);


public: // 서버에 보낸다.
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

