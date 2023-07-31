#pragma once

class CPlayerScript;
class CGameObject;
class CChampionScript;

class GameObjMgr
{
private:
	map<uint64, CGameObject*> _allObjects; // �ʿ� �ִ� ��� ������Ʈ
	map<uint64, CGameObject*> _players;
	map<uint64, CGameObject*> _objects; // �̴Ͼ�, ���۸�, ��ų ����ü, ����Ʈ
	map<uint64, CGameObject*> _towers; // ��ž, ������, �ؼ��� (��ġ�� ��������, �� ó��)

	// My Player ����
	CChampionScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript�� prevPos�� �� ƽ���� ������ǥ��, �̰� 1/10���� ��ǥ.

	// Objects pos ����
	map<uint64, Vec3> _objectsPrevPos;

public:
	map<uint64, CGameObject*> GetObjects() { return _objects; }
	map<uint64, CGameObject*> GetTowers() { return _towers; }

public: 
	// �������� ��Ŷ�� ������.
	void SendMyPlayerMove(ClientServiceRef _service);
	void SendObjectAnim(AnimInfo* _animInfo, ClientServiceRef _service);
	void SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendTowerUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendSkillSpawn(SkillInfo* _skillInfo, ClientServiceRef _service);
	void SendSkillHit(HitInfo* _hitInfo, ClientServiceRef _service);

	// �������� ���� ��Ŷ �̺�Ʈ ��� ó��
	void E_MovePlayer(uint64 _playerId, ObjectMove _playerMove);
	void E_MoveObject(uint64 _objectId, ObjectMove _objectMove);
	void E_ObjectAnim(AnimInfo _animInfo);
	void E_HitObject(uint64 _hit, SkillInfo _skillInfo);
public:
	CGameObject* FindPlayer(uint64 _targetId);
	CGameObject* FindObject(uint64 _targetId);
	CGameObject* FindAllObject(uint64 _targetId);

	void AddPlayer(PlayerInfo info, bool myPlayer = false);
	void AddObject(uint64 _objectId, ObjectInfo _objectInfo);
	// void AddTower();
	void AddSkillProjectile(uint64 _projectileId, SkillInfo _skillInfo);

	CChampionScript* GetMyPlayerScript() { return MyPlayerScript; }

static GameObjMgr* GetInst()
{
	 static GameObjMgr inst;
	 return &inst;
}
private:
	 GameObjMgr();
	~GameObjMgr();

};

