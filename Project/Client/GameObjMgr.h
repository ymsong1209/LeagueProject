#pragma once

class CPlayerScript;
class CGameObject;

class GameObjMgr
{
	// ��ü
	map<uint64, CGameObject*> _players;
	map<uint64, CGameObject*> _objects; // �̴Ͼ�, ���۸� (�ð�����)
	map<uint64, CGameObject*> _towers; // ��ž, ������, �ؼ��� (�� ó��)


	// My Player ����
	CPlayerScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript�� prevPos�� �� ƽ���� ������ǥ��, �̰� 1/10���� ��ǥ.
	PlayerMove::PlayerState PrevState = PlayerMove::PlayerState::IDLE;


public:
	PlayerMove::PlayerState GetPrevState() { return PrevState; }
	CGameObject* FindPlayer(uint64 _id);
	void AddPlayer(PlayerInfo info, bool myPlayer = false);

	void AddObject(uint64 _objectId, ObjectType _objectType, FactionType _factionType);
	// void AddTower();

	void E_MovePlayer(uint64 _playerId, PlayerMove _playerMove);


public: // ������ ������.
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

