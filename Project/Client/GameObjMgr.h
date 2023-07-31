#pragma once

class CPlayerScript;
class CGameObject;
class CChampionScript;

class GameObjMgr
{
private:
	map<uint64, CGameObject*> _allObjects; // 맵에 있는 모든 오브젝트
	map<uint64, CGameObject*> _players;
	map<uint64, CGameObject*> _objects; // 미니언, 정글몹, 스킬 투사체, 이펙트
	map<uint64, CGameObject*> _towers; // 포탑, 억제기, 넥서스 (배치형 오브젝터, 맵 처음)

	// My Player 관련
	CChampionScript* MyPlayerScript;
	Vec3 PrevPos = Vec3(0, 0, 0);  // playerScript의 prevPos는 매 틱마다 이전좌표고, 이건 1/10초전 좌표.

	// Objects pos 관련
	map<uint64, Vec3> _objectsPrevPos;

public:
	map<uint64, CGameObject*> GetObjects() { return _objects; }
	map<uint64, CGameObject*> GetTowers() { return _towers; }

public: 
	// 서버에게 패킷을 보낸다.
	void SendMyPlayerMove(ClientServiceRef _service);
	void SendObjectAnim(AnimInfo* _animInfo, ClientServiceRef _service);
	void SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendTowerUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service);
	void SendSkillSpawn(SkillInfo* _skillInfo, ClientServiceRef _service);
	void SendSkillHit(HitInfo* _hitInfo, ClientServiceRef _service);

	// 서버에게 받은 패킷 이벤트 등록 처리
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

