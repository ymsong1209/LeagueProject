#pragma once

class CPlayerScript;
class CGameObject;
class CChampionScript;
class CUnitScript;

class GameObjMgr
{
private:
	map<uint64, CGameObject*> _allObjects; // �ʿ� �ִ� ��� ������Ʈ
	map<uint64, CGameObject*> _players;
	map<uint64, CGameObject*> _objects; // �̴Ͼ�, ���۸�, ��ų ����ü, ����Ʈ
	map<uint64, CGameObject*> _placedObjects; // ��ž, ������, �ؼ��� (��ġ�� ��������, �� ó��)

	// My Player Prev
	CUnitScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript�� prevPos�� �� ƽ���� ������ǥ��, �̰� 1/10���� ��ǥ.
	float PrevHP = 0;
	float PrevMP = 0;
	CC    PrevCC = CC::CLEAR;

	// Objects, PlacedObjects Prev
	map<uint64, Vec3> _objectsPrevPos;
	map<uint64, float> _placedObjectsPrevHP;

public:
	map<uint64, CGameObject*> GetPlayers() { return _players; }
	map<uint64, CGameObject*> GetObjects() { return _objects; }
	map<uint64, CGameObject*> GetPlacedObjects() { return _placedObjects; }

public: 
	// �������� ��Ŷ�� ������.
	void SendMyPlayerMove(ClientServiceRef _service);
	void SendObjectAnim(AnimInfo* _animInfo, ClientServiceRef _service);
	void SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendPlacedObjectUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendSkillSpawn(SkillInfo* _skillInfo, ClientServiceRef _service);
	void SendSkillHit(HitInfo* _hitInfo, ClientServiceRef _service);
	void SendDespawn(UINT64 _despawnId, float lifespan, ClientServiceRef _service);
	void SendKDACS(KDACSInfo* _kdacsInfo, ClientServiceRef _service);
public:
	CGameObject* FindPlayer(uint64 _targetId);
	CGameObject* FindObject(uint64 _targetId);
	CGameObject* FindPlacedObject(uint64 _targetId);
	CGameObject* FindAllObject(uint64 _targetId);
	
	CGameObject* DeleteObjectInMap(uint64 _id);

	void AddPlayer(PlayerInfo info, bool myPlayer = false);
	void AddObject(uint64 _objectId, ObjectInfo _objectInfo);
	// void AddTower();
	void AddSkillProjectile(uint64 _projectileId, SkillInfo _skillInfo);

	CUnitScript* GetMyPlayerScript() { return MyPlayerScript; }

static GameObjMgr* GetInst()
{
	 static GameObjMgr inst;
	 return &inst;
}
private:
	 GameObjMgr();
	~GameObjMgr();

};

