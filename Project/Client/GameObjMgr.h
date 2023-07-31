#pragma once

class CPlayerScript;
class CGameObject;

class GameObjMgr
{
private:
	map<uint64, CGameObject*> _allObjects; // �ʿ� �ִ� ��� ������Ʈ
	map<uint64, CGameObject*> _players;
	map<uint64, CGameObject*> _objects; // �̴Ͼ�, ���۸�, ��ų ����ü
	map<uint64, CGameObject*> _towers; // ��ž, ������, �ؼ��� (�� ó��)

	// My Player ����
	CPlayerScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript�� prevPos�� �� ƽ���� ������ǥ��, �̰� 1/10���� ��ǥ.

	// Objects pos ����
	map<uint64, Vec3> _objectsPrevPos;

public:
	map<uint64, CGameObject*> GetObjects() { return _objects; }


public: 
	// �������� ��Ŷ�� ������.
	void SendMyPlayerMove(ClientServiceRef _service);
	void SendObjectAnim(uint64 _id, ClientServiceRef _service);
	void SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendTowerUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service);

	// �������� ���� ��Ŷ ó��
	void E_MovePlayer(uint64 _playerId, ObjectMove _playerMove);
	void E_MoveObject(uint64 _objectId, ObjectMove _objectMove);
	void E_ObjectAnim(uint64 _objectId, AnimInfo _animInfo);

public:
	CGameObject* FindPlayer(uint64 _targetId);
	CGameObject* FindObject(uint64 _targetId);
	CGameObject* FindAllObject(uint64 _targetId);

	void AddPlayer(PlayerInfo info, bool myPlayer = false);
	void AddObject(uint64 _objectId, ObjectInfo _objectInfo);
	// void AddTower();

	CPlayerScript* GetMyPlayerScript() { return MyPlayerScript; }

static GameObjMgr* GetInst()
{
	 static GameObjMgr inst;
	 return &inst;
}
private:
	 GameObjMgr();
	~GameObjMgr();

};

