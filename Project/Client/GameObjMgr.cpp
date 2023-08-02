#include "pch.h"
#include "GameObjMgr.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CEventMgr.h>

#include <Engine\CPathFindMgr.h>
#include <Engine\CAnim3D.h>

#include <Script\CUnitScript.h>
#include <Script\CChampionScript.h>
#include <Script\CJinxScript.h>
#include <Script\CJinxWScript.h>
#include <Script\CCameraMoveScript.h>

#include <Script\CSkill.h>
#include <Script\CBaseAttack.h>
#include <Script/CBasicAttackScript.h>
#include <Script/CAttackRangeScript.h>
#include <Script/CMinionScript.h>
#include <Script/CSkillMgr.h>

#include <Script\COtherPlayerScript.h>
#include "ServerEventMgr.h"

GameObjMgr::GameObjMgr()
{
}
GameObjMgr::~GameObjMgr()
{
}


CGameObject* GameObjMgr::FindPlayer(uint64 _targetId)
{
	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _players.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _players.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not playerId in Server" << endl;
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindObject(uint64 _targetId)
{	
	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _objects.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _objects.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not objectId in Server" << endl;
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindAllObject(uint64 _targetId)
{
	_allObjects.insert(_players.begin(), _players.end());
	_allObjects.insert(_objects.begin(), _objects.end());
	_allObjects.insert(_towers.begin(), _towers.end());

	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _allObjects.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _allObjects.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not in Server" << endl;
		return nullptr;
	}
}

CGameObject* GameObjMgr::DeleteObjectInMap(uint64 _id)
{
	CGameObject* obj = FindAllObject(_id);

	_allObjects.erase(_id);
	_players.erase(_id);
	_objects.erase(_id);
	_towers.erase(_id);

	return obj;
}



// ===============================================
//   Add
// ===============================================
void GameObjMgr::AddPlayer(PlayerInfo _info, bool myPlayer)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		switch (_info.champion)
		{
			case ChampionType::JINX:
			{
				pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
				pObj = pMeshData->Instantiate();

				if (myPlayer)
					pObj->AddComponent(new CJinxScript);
				else
					pObj->AddComponent(new CUnitScript);


				pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
				pObj->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true, true, 0.1f);
				pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

			}break;

			case ChampionType::AMUMU:
			{
			}break;
		}

		if (myPlayer)
		{
			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CFsm);
			MyPlayerScript = pObj->GetScript<CChampionScript>();
			MyPlayerScript->SetServerID(_info.id);
			MyPlayerScript->SetNickname(_info.nickname);
			MyPlayerScript->SetHost(_info.host);
			MyPlayerScript->SetFaction(_info.faction);

			pObj->SetName(L"MyPlayer");


			// 사거리 자식 오브젝트 추가
			CGameObject* AttackRange = new CGameObject;
			AttackRange->AddComponent(new CTransform);
			AttackRange->AddComponent(new CCollider2D);
			AttackRange->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			AttackRange->Collider2D()->SetOffsetScale(Vec2(2000.f, 2000.f));
			AttackRange->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			AttackRange->AddComponent(new CAttackRangeScript);
			AttackRange->SetName(L"AttackRange");
			pObj->AddChild(AttackRange);


		}
		else
		{
			CUnitScript* unit = pObj->GetScript<CUnitScript>();
			unit->SetServerID(_info.id);
			unit->SetNickname(_info.nickname);
			unit->SetHost(_info.host);
			unit->SetFaction(_info.faction);

			pObj->SetName(L"OtherPlayer");
		}

		//pObj->SetName(_info.nickname);


		pObj->AddComponent(new CCollider3D);
		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pObj->Collider3D()->SetAbsolute(true);
		pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		pObj->Collider3D()->SetDrawCollision(false);

		pObj->AddComponent(new CCollider2D);
		pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
		pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0, 0));


		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		pObj->Transform()->SetUseMouseOutline(true);

		Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
		SpawnGameObject(pObj, spawnPos, 0);


		_players.insert(std::make_pair(_info.id, pObj));
	}
}

void GameObjMgr::AddObject(uint64 _objectId, ObjectInfo _objectInfo)
{

	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		// 방장만 진짜를 생성한다. 나머지는 가짜를 생성한다.
		if (MyPlayer.host)
		{
			// 미니언 스크립트
			// 디버깅용으로 구체 띄워야지

			CGameObject* pObj = new CGameObject;

			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\minion_melee.fbx");
			pObj = pMeshData->Instantiate();

			pObj->AddComponent(new CMinionScript);
			pObj->AddComponent(new CCollider3D);
			//pObj->AddComponent(new CCollider2D);

			//pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			//pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
			//pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			pObj->SetName(L"Minion");

			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\minion_melee");
			pObj->Animator3D()->PlayRepeat(L"minion_melee\\Attack1", true, true, 0.1f);

			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			

			pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			pObj->Collider3D()->SetDrawCollision(false);

			pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));


			Vec3 spawnPos = Vec3(100.f + (50 * _objects.size()), 30.f, 100.f);
			SpawnGameObject(pObj, spawnPos, 0);

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		else
		{
			// 기본 OtherObjectScript
			// 미니언 스크립트
			// 디버깅용으로 구체 띄워야지
			CGameObject* pObj = new CGameObject;

			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\minion_melee.fbx");
			pObj = pMeshData->Instantiate();
			
			pObj->AddComponent(new CUnitScript);
			pObj->AddComponent(new CCollider3D);
			//pObj->AddComponent(new CCollider2D);

			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\minion_melee");
			pObj->Animator3D()->PlayRepeat(L"minion_melee\\Attack1", true, true, 0.1f);


			//pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			//pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
			//pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);

			pObj->SetName(L"OtherMinion");

			pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			pObj->Collider3D()->SetDrawCollision(false);

			pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));


			Vec3 spawnPos = Vec3(100.f + (50 * _objects.size()), 30.f, 100.f);
			SpawnGameObject(pObj, spawnPos, 0);

			_objects.insert(std::make_pair(_objectId, pObj));
		}


		
	}
}

void GameObjMgr::AddSkillProjectile(uint64 _projectileId, SkillInfo _skillInfo)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		// 방장만 진짜를 생성한다. 나머지는 가짜를 생성한다.
		if (MyPlayer.host)
		{
			CGameObject* pObj = new CGameObject;
			
			// 원래라면 skillinfo에서 스킬타입에 따라 switch case로 해당 스킬을 AddComponent해준다.
			//pObj->AddComponent(new CMeshRender);
			//pObj->AddComponent(new CTransform);
			//pObj->AddComponent(new CBasicAttackScript);
			//pObj->AddComponent(new CCollider2D);
			//pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			//pObj->Collider2D()->SetOffsetScale(Vec2(15, 15));
			//pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0, 0));
			//
			//// skillinfo 에 따라 세팅해줌 
			//CBasicAttackScript* Script = pObj->GetScript<CBasicAttackScript>();
			//Script->SetServeID(_projectileId);
			//Script->SetServerUserID(_skillInfo.OwnerId);
			//Script->SetServerTargetID(_skillInfo.TargetId);
			//
			//Script->SetUserObj(FindAllObject(_skillInfo.OwnerId));
			//Script->SetTargetObj(FindAllObject(_skillInfo.TargetId));

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);
			CGameObject* TargetObj = FindAllObject(_skillInfo.TargetId);
	
			// _SkillInfo를 까서, 어떤 Skill인지 가지고 옴
			//CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);
			//
			//// skill에 사용자 / 피격자 정보 등록
			//skill->SetUserObj(UserObj);
			//skill->SetTargetObj(TargetObj);

			// skill에 정보 등록
			//skill->Use();

			// skill 꺼내옴
			//CGameObject* projectile = skill->GetProjectile();

			CGameObject* Projectile = new CGameObject;
			Projectile->AddComponent(new CTransform);
			Projectile->AddComponent(new CCollider2D);

			Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
			Projectile->Collider2D()->SetOffsetScale(Vec2(20.f, 5.f));
			Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			Projectile->SetName(L"JinxW");

			Projectile->AddComponent(new CJinxWScript);
			Vec3 OwnerPos = UserObj->Transform()->GetRelativePos();
			float OwnerFace = UserObj->GetScript<CUnitScript>()->GetFaceRot();

			Projectile->GetScript<CJinxWScript>()->SetUserObj(UserObj);
			Projectile->GetScript<CJinxWScript>()->SetSpawnPos(OwnerPos);
			//Projectile->GetScript<CJinxWScript>()->SetTargetPos(TargetObj->Transform()->GetRelativePos());
			Projectile->GetScript<CJinxWScript>()->SetDir(Vec3(_skillInfo.MouseDir.x, _skillInfo.MouseDir.y, _skillInfo.MouseDir.z));
			Projectile->GetScript<CJinxWScript>()->SetServeID(_projectileId);
			Projectile->GetScript<CJinxWScript>()->SetServerUserID(_skillInfo.OwnerId);
			//Projectile->GetScript<CJinxWScript>()->SetServerTargetID(_skillInfo.TargetId);

			// 스킬쏜 주인 중점에서 투사체 생김
			SpawnGameObject(Projectile, OwnerPos, 0);

			_objects.insert(std::make_pair(_projectileId, Projectile));
		}
		else
		{
			//CGameObject* pObj = new CGameObject;
			//
			//pObj->AddComponent(new CMeshRender);
			//pObj->AddComponent(new CTransform);
			//pObj->AddComponent(new CUnitScript);
			//pObj->AddComponent(new CCollider2D);
			//pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			//pObj->Collider2D()->SetOffsetScale(Vec2(15, 15));
			//pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0, 0));
			//CUnitScript* Script = pObj->GetScript<CUnitScript>();
			//Script->SetServerID(_projectileId);
			//Script->SetFaction(Faction::NONE);
			//pObj->SetName(L"Projectile");
			//
			//// 스킬쏜 주인 중점에서 투사체 생김
			//CGameObject* ownerObj = FindAllObject(_skillInfo.OwnerId);

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);
			CGameObject* TargetObj = FindAllObject(_skillInfo.TargetId);

			// _SkillInfo를 까서, 어떤 Skill인지 가지고 옴
			//CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);
			//
			//// skill에 사용자 / 피격자 정보 등록
			//skill->SetUserObj(UserObj);
			//skill->SetTargetObj(TargetObj);
			//
			//// skill에 정보 등록
			//skill->Use();
			//
			//// skill 꺼내옴
			//CGameObject* projectile = skill->GetProjectile();
			//
			//// 스킬쏜 주인 중점에서 투사체 생김
			//CGameObject* ownerObj = FindAllObject(_skillInfo.OwnerId);
			//SpawnGameObject(projectile, ownerObj->Transform()->GetRelativePos(), 0);

			CGameObject* Projectile = new CGameObject;
			Projectile->AddComponent(new CTransform);
			Projectile->AddComponent(new CCollider2D);

			Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
			Projectile->Collider2D()->SetOffsetScale(Vec2(20.f, 5.f));
			Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			Projectile->SetName(L"JinxW");

			Vec3 OwnerPos = UserObj->Transform()->GetRelativePos();

			Projectile->AddComponent(new CUnitScript);
			Projectile->GetScript<CUnitScript>()->SetServerID(_projectileId);


			// 스킬쏜 주인 중점에서 투사체 생김
			SpawnGameObject(Projectile, OwnerPos, 0);

			_objects.insert(std::make_pair(_projectileId, Projectile));
		}



	}
}




// ===============================================
//   Send 
// ===============================================
void GameObjMgr::SendMyPlayerMove(ClientServiceRef _service)
{
	// 본인 플레이어의 움직임을 서버에 보낸다. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = FindPlayer(MyPlayer.id);

		if (obj == nullptr)
			return;

		if (obj->GetLayerIndex() == -1)
			return;

		Vec3  CurPos = obj->Transform()->GetRelativePos();

		if (PrevPos == CurPos) // 이전 좌표와 변화가 없다면 move packet을 보내지 않는다. return
			return;

		PrevPos = CurPos;

		Vec3  CurRot = obj->Transform()->GetRelativeRot();
		//float CurLV = obj->GetScript<CUnitScript>()->GetLV();
		float CurHP = obj->GetScript<CUnitScript>()->GetCurHP();
		float CurMP = obj->GetScript<CUnitScript>()->GetCurMP();
		float CurAttackPower = obj->GetScript<CUnitScript>()->GetAttackPower();
		float CurDefencePower = obj->GetScript<CUnitScript>()->GetDefencePower();

		ObjectMove move = {};
		//move.LV = CurLV;
		move.HP = CurHP;
		move.MP = CurMP;
		move.AttackPower = CurAttackPower;
		move.DefencePower = CurDefencePower;
		move.pos.x = CurPos.x;
		move.pos.y = CurPos.y;
		move.pos.z = CurPos.z;
		move.moveDir.x = CurRot.x;
		move.moveDir.y = CurRot.y;
		move.moveDir.z = CurRot.z;

		// 서버에게 패킷 전송
		std::cout << "C_PLAYER_MOVE Pakcet. id : "<< MyPlayer.id << endl;
		PKT_C_PLAYER_MOVE_WRITE pktWriter(move);
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);
		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
	// 오브젝트의 움직임을 서버에 보낸다. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = FindObject(_id);

		if (obj == nullptr || obj->GetLayerIndex() == -1)
			return;

		Vec3  CurPos = obj->Transform()->GetRelativePos();

		auto it = _objectsPrevPos.find(_id);
		if (it != _objectsPrevPos.end()) // PrevPos가 있다. 	
		{
			// 이전 좌표와 똑같다면 move packet을 보내지 않는다. return
			if (_objectsPrevPos.at(_id) == CurPos) 
				return;

			_objectsPrevPos.at(_id) = CurPos; // 현재 좌표를 이전좌표로 저장
			
			Vec3  CurRot = obj->Transform()->GetRelativeRot();
			//float CurLV = obj->GetScript<CUnitScript>()->GetLV();

			ObjectMove move = {};
			if (FindObject(_id)->GetScript<CUnitScript>() != nullptr)
			{
				float CurHP = obj->GetScript<CUnitScript>()->GetCurHP();
				float CurMP = obj->GetScript<CUnitScript>()->GetCurMP();
				float CurAttackPower = obj->GetScript<CUnitScript>()->GetAttackPower();
				float CurDefencePower = obj->GetScript<CUnitScript>()->GetDefencePower();

				move.HP = CurHP;
				move.MP = CurMP;
				move.AttackPower = CurAttackPower;
				move.DefencePower = CurDefencePower;
			}

			//move.LV = CurLV;
			move.pos.x = CurPos.x;
			move.pos.y = CurPos.y;
			move.pos.z = CurPos.z;
			move.moveDir.x = CurRot.x;
			move.moveDir.y = CurRot.y;
			move.moveDir.z = CurRot.z;

			// 서버에게 패킷 전송
			std::cout << "C_OBJECT_MOVE Pakcet. id : " << _id << endl;

			PKT_C_OBJECT_MOVE_WRITE pktWriter(_id, move);
			SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
			_service->Broadcast(sendBuffer);
			std::cout << "===============================" << endl;
		}
		else
		{
			_objectsPrevPos.insert(pair(_id, CurPos));
		}

		
	}
}

void GameObjMgr::SendTowerUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
}

void GameObjMgr::SendSkillSpawn(SkillInfo* _skillInfo, ClientServiceRef _service)
{
	// 스킬을 쏜 오브젝트가 생성 패킷을 보낸다. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		SkillInfo skillInfoPacket = {};
		skillInfoPacket.SkillId = _skillInfo->SkillId;
		skillInfoPacket.OwnerId = _skillInfo->OwnerId;
		skillInfoPacket.TargetId = _skillInfo->TargetId;
		skillInfoPacket.SkillLevel = _skillInfo->SkillLevel;
		skillInfoPacket.skillType = _skillInfo->skillType;
		skillInfoPacket.offsetPos = _skillInfo->offsetPos;
		skillInfoPacket.UseMousePos = _skillInfo->UseMousePos;
		skillInfoPacket.MousePos = _skillInfo->MousePos;
		skillInfoPacket.UseMouseDir = _skillInfo->UseMouseDir;
		skillInfoPacket.MouseDir = _skillInfo->MouseDir;

		PKT_C_SKILL_PROJECTILE_WRITE  pktWriter(skillInfoPacket);

		// 서버에게 패킷 전송
		std::cout << "Send C_SKILL_PROJECTILE Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendSkillHit(HitInfo* _hitInfo, ClientServiceRef _service)
{
	// hitOjbId가 스킬을 맞았다고 보낸다. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		SkillInfo skillInfoPacket = {};
		skillInfoPacket.SkillId =		_hitInfo->skillObjId;
		skillInfoPacket.OwnerId =		_hitInfo->useObjId;
		skillInfoPacket.TargetId =		_hitInfo->hitObjId;
		skillInfoPacket.SkillLevel =	_hitInfo->SkillLevel;
		skillInfoPacket.skillType =		_hitInfo->skillType;

		PKT_C_SKILL_HIT_WRITE  pktWriter(_hitInfo->hitObjId,skillInfoPacket);

		// 서버에게 패킷 전송
		std::cout << "Send C_SKILL_HIT Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendDespawn(UINT64 _despawnId, float lifespan, ClientServiceRef _service)
{
	// 이 오브젝트를 lifespan시간뒤에 삭제하라고 서버에게 보낸다.
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		PKT_C_DESPAWN_OBJECT_WRITE  pktWriter(_despawnId, lifespan);

		// 서버에게 패킷 전송
		std::cout << "Send C_DESPAWN_OBJECT Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendObjectAnim(AnimInfo* _animInfo, ClientServiceRef _service)
{
	// _id 오브젝트의 애니메이션을 보낸다.
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		AnimInfoPacket animInfoPacket = {};
		animInfoPacket.targetId = _animInfo->targetId;
		animInfoPacket.bRepeat = _animInfo->bRepeat;
		animInfoPacket.blend = _animInfo->blend;
		animInfoPacket.blendTime = _animInfo->blendTime;


		wstring _animName = _animInfo->animName;

		// 보내는 사람의 Id	  : MyPlayer.id
		// 변경될 오브젝트의 Id : animInfo.targetId
		PKT_C_OBJECT_ANIM_WRITE  pktWriter(MyPlayer.id, animInfoPacket);
		PKT_C_OBJECT_ANIM_WRITE::AnimNameList animNamePacket = pktWriter.ReserveAnimNameList(_animName.size());
		for (int i = 0; i < _animName.size(); i++)
		{
			animNamePacket[i] = { _animName[i] };
		}

		// 서버에게 패킷 전송
		std::cout << "Send C_OBJECT_ANIM Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}


