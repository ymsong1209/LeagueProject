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
#include <Script/CBasicAttackScript.h>
#include <Script/CProjectileScript.h>
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
	std::mutex m;
	m.lock();
	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _players.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _players.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		m.unlock();
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not playerId in Server (FindPlayers)" << endl;
		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindObject(uint64 _targetId)
{
	std::mutex m;
	m.lock();
	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _objects.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _objects.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		m.unlock();
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not objectId in Server (FindObject)" << endl;
		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindPlacedObject(uint64 _targetId)
{
	std::mutex m;
	m.lock();

	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _placedObjects.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _placedObjects.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		m.unlock();
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not placedObjectId in Server (FindPlacedObject)" << endl;
		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindAllObject(uint64 _targetId)
{
	std::mutex m;
	m.lock();

	if (_targetId == UINT64_MAX)
	{
		cout << "Maybe NonTarget Id" << endl;
		m.unlock();
		return nullptr;
	}

	_allObjects.insert(_players.begin(), _players.end());
	_allObjects.insert(_objects.begin(), _objects.end());
	_allObjects.insert(_placedObjects.begin(), _placedObjects.end());

	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint64, CGameObject*>::iterator iter = _allObjects.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _allObjects.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;

		m.unlock();
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not in Server (FindAllObject)" << endl;

		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::DeleteObjectInMap(uint64 _id)
{
	std::mutex m;
	m.lock();

	CGameObject* obj = FindAllObject(_id);

	_allObjects.erase(_id);
	_players.erase(_id);
	_objects.erase(_id);
	_placedObjects.erase(_id);

	m.unlock();
	return obj;
}



// ===============================================
//   Send 
// ===============================================
void GameObjMgr::SendMyPlayerMove(ClientServiceRef _service)
{
	// 본인 플레이어의 움직임을 서버에 보낸다. 
	CGameObject* obj = FindPlayer(MyPlayer.id);

	if (obj == nullptr || obj->GetLayerIndex() == -1)
		return;

	Vec3  CurPos = obj->Transform()->GetRelativePos();
	Vec3  CurRot = obj->Transform()->GetRelativeRot();
	int   CurLV = obj->GetScript<CUnitScript>()->GetLevel();
	float CurHP = obj->GetScript<CUnitScript>()->GetCurHP();
	float CurMP = obj->GetScript<CUnitScript>()->GetCurMP();
	float MaxHP = obj->GetScript<CUnitScript>()->GetMaxHP();
	float MaxMP = obj->GetScript<CUnitScript>()->GetMaxMP();
	CC CurCC = obj->GetScript<CUnitScript>()->GetCC();


	// 이전과 변화가 없다면 move packet을 보내지 않는다. return
	if (PrevPos == CurPos
		&& PrevHP == CurHP
		&& PrevMP == CurMP
		&& PrevCC == CurCC) 
		return;

	PrevPos = CurPos;
	PrevHP = CurHP;
	PrevMP = CurMP;
	PrevCC = CurCC;

	float CurAttackPower = obj->GetScript<CUnitScript>()->GetAttackPower();
	float CurDefencePower = obj->GetScript<CUnitScript>()->GetDefencePower();
	bool bUnitDead = obj->GetScript<CUnitScript>()->IsUnitDead();


	ObjectMove move = {};
	move.LV = CurLV;
	move.HP = CurHP;
	move.MP = CurMP;
	move.MaxHP = MaxHP;
	move.MaxMP = MaxMP;
	move.AttackPower = CurAttackPower;
	move.DefencePower = CurDefencePower;
	move.pos.x = CurPos.x;
	move.pos.y = CurPos.y;
	move.pos.z = CurPos.z;
	move.moveDir.x = CurRot.x;
	move.moveDir.y = CurRot.y;
	move.moveDir.z = CurRot.z;
	move.CC = CurCC;

	move.bUnitDead = bUnitDead;
	// 서버에게 패킷 전송
	std::cout << "C_PLAYER_MOVE Pakcet. id : "<< MyPlayer.id << endl;
	PKT_C_PLAYER_MOVE_WRITE pktWriter(move);
	SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
	_service->Broadcast(sendBuffer);
	std::cout << "===============================" << endl;

}

void GameObjMgr::SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
	// 오브젝트의 움직임을 서버에 보낸다.

	if (_obj == nullptr || _obj->GetLayerIndex() == -1)
		return;
	
	Vec3  CurPos = _obj->Transform()->GetRelativePos();
	Vec3  CurRot = _obj->Transform()->GetRelativeRot();
	float CurHP = _obj->GetScript<CUnitScript>()->GetCurHP();
	float CurMP = _obj->GetScript<CUnitScript>()->GetCurMP();
	int   CurLevel = _obj->GetScript<CUnitScript>()->GetLevel();

	auto it = _objectsPrevPos.find(_id);
	if (it != _objectsPrevPos.end()) // PrevPos가 있다. 	
	{
		auto it1 = _objectsPrevHP.find(_id);
		if (it1 != _objectsPrevHP.end())  // PrevHP 가 있다.
		{
			
			// 이전 좌표, 이전 HP 똑같다면 move packet을 보내지 않는다. return
			if (_objectsPrevPos.at(_id) == CurPos
				&& _objectsPrevHP.at(_id) == CurHP)
				return;

			_objectsPrevPos.at(_id) = CurPos; // 현재 좌표를 이전좌표로 저장
			_objectsPrevHP.at(_id) = CurHP;   // 현재 HP를 이전 HP 로 저장


			ObjectMove move = {};
			if (_obj->GetScript<CUnitScript>() != nullptr)
			{
				float CurAttackPower = _obj->GetScript<CUnitScript>()->GetAttackPower();
				float CurDefencePower = _obj->GetScript<CUnitScript>()->GetDefencePower();
				float MaxHP = _obj->GetScript<CUnitScript>()->GetMaxHP();
				float MaxMP = _obj->GetScript<CUnitScript>()->GetMaxMP();
				CC CurCC = _obj->GetScript<CUnitScript>()->GetCC();
				bool bUnitDead = _obj->GetScript<CUnitScript>()->IsUnitDead();

				move.HP = CurHP;
				move.MP = CurMP;
				move.MaxHP = MaxHP;
				move.MaxMP = MaxMP;

				move.AttackPower = CurAttackPower;
				move.DefencePower = CurDefencePower;
				move.CC = CurCC;
				move.bUnitDead = bUnitDead;
				move.LV = CurLevel;
			}

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
			_objectsPrevHP.insert(pair(_id, CurHP));
		}
	}
	else
	{
		_objectsPrevPos.insert(pair(_id, CurPos));
	}
}

void GameObjMgr::SendPlacedObjectUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
	// 배치형 오브젝트의 업데이트를 서버에 보낸다. (HP 변경시에만)
	CGameObject* obj = FindPlacedObject(_id);

	if (_obj != obj || obj == nullptr || obj->GetLayerIndex() == -1)
		return;

	if (_obj->GetScript<CUnitScript>() != nullptr)
	{
		float CurHP = obj->GetScript<CUnitScript>()->GetCurHP();
		auto it = _placedObjectsPrevHP.find(_id);
		if (it != _placedObjectsPrevHP.end()) // PrevHP가 있다. 	
		{
			// 이전 HP와 똑같다면 move packet을 보내지 않는다. return
			if (_placedObjectsPrevHP.at(_id) == CurHP)
				return;

			_placedObjectsPrevHP.at(_id) = CurHP; // 현재 HP를 이전 HP로 저장

			ObjectMove updatePlacedObject = {};

			float CurMP = obj->GetScript<CUnitScript>()->GetCurMP();
			float CurAttackPower = obj->GetScript<CUnitScript>()->GetAttackPower();
			float CurDefencePower = obj->GetScript<CUnitScript>()->GetDefencePower();
			float MaxHP = obj->GetScript<CUnitScript>()->GetMaxHP();
			float MaxMP = obj->GetScript<CUnitScript>()->GetMaxMP();
			bool bUnitDead = obj->GetScript<CUnitScript>()->IsUnitDead();
			updatePlacedObject.HP = CurHP;
			updatePlacedObject.MP = CurMP;
			updatePlacedObject.MaxHP = MaxHP;
			updatePlacedObject.MaxMP = MaxMP;
			updatePlacedObject.AttackPower = CurAttackPower;
			updatePlacedObject.DefencePower = CurDefencePower;

			updatePlacedObject.bUnitDead = bUnitDead;

			Vec3 CurPos = obj->Transform()->GetRelativePos();
			Vec3 CurRot = obj->Transform()->GetRelativeRot();

			updatePlacedObject.pos.x = CurPos.x;
			updatePlacedObject.pos.y = CurPos.y;
			updatePlacedObject.pos.z = CurPos.z;
			updatePlacedObject.moveDir.x = CurRot.x;
			updatePlacedObject.moveDir.y = CurRot.y;
			updatePlacedObject.moveDir.z = CurRot.z;

			// 서버에게 패킷 전송
			std::cout << "C_OBJECT_MOVE Pakcet.(placedObject) id : " << _id << endl;

			PKT_C_OBJECT_MOVE_WRITE pktWriter(_id, updatePlacedObject);
			SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
			_service->Broadcast(sendBuffer);
			std::cout << "===============================" << endl;
		}
		else
		{
			_placedObjectsPrevHP.insert(pair(_id, CurHP));
		}
	}
}

void GameObjMgr::SendSkillSpawn(SkillInfo* _skillInfo, ClientServiceRef _service)
{
	// 스킬을 쏜 오브젝트가 생성 패킷을 보낸다. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		SkillInfo skillInfoPacket = {};
		// this time, we don't know skill projectile Id
		//skillInfoPacket.SkillId = _skillInfo->SkillId;
		skillInfoPacket.OwnerId = _skillInfo->OwnerId;
		skillInfoPacket.TargetId = _skillInfo->TargetId;
		skillInfoPacket.SkillLevel = _skillInfo->SkillLevel;
		skillInfoPacket.skillType = _skillInfo->skillType;
		skillInfoPacket.offsetPos = _skillInfo->offsetPos;
		skillInfoPacket.projectileCount = _skillInfo->projectileCount;
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

void GameObjMgr::SendKDACS(KDACSInfo* _kdacsInfo, ClientServiceRef _service)
{
	// 누군가 죽였고, 누군가 죽었다고 알린다. (죽은 id는 UnitType에 따라 확인유무 갈림)
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		KDACSInfo kdacsInfo = {};
		kdacsInfo.killerId = _kdacsInfo->killerId;
		kdacsInfo.victimId = _kdacsInfo->victimId;
		kdacsInfo.deadObjUnitType = _kdacsInfo->deadObjUnitType;

		PKT_C_KDA_CS_WRITE  pktWriter(kdacsInfo);

		// 서버에게 패킷 전송
		std::cout << "Send C_KDA_CS Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendObjectMtrl(MtrlInfo* _mtrlInfo, ClientServiceRef _service)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		MtrlInfoPacket mtrlInfoPacket = {};
		mtrlInfoPacket.targetId = _mtrlInfo->targetId;
		mtrlInfoPacket.iMtrlIndex = _mtrlInfo->iMtrlIndex;
		mtrlInfoPacket.tex_param = _mtrlInfo->tex_param;

		wstring _texName = _mtrlInfo->wTexName;

		PKT_C_OBJECT_MTRL_WRITE  pktWriter(mtrlInfoPacket);
		PKT_C_OBJECT_MTRL_WRITE::TexNameList texNamePacket = pktWriter.ReserveTexNameList(_texName.size());
		for (int i = 0; i < _texName.size(); i++)
		{
			texNamePacket[i] = { _texName[i] };
		}

		// 서버에게 패킷 전송
		std::cout << "Send C_OBJECT_MTRL Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}

}

void GameObjMgr::SendObjectAnim(AnimInfo* _animInfo, ClientServiceRef _service)
{
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


