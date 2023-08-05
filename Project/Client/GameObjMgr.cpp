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
	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _players.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _players.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		m.unlock();
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
		cout << "Id is not playerId in Server (FindPlayers)" << endl;
		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindObject(uint64 _targetId)
{
	std::mutex m;
	m.lock();
	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _objects.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _objects.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		m.unlock();
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
		cout << "Id is not objectId in Server (FindObject)" << endl;
		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindPlacedObject(uint64 _targetId)
{
	std::mutex m;
	m.lock();

	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _placedObjects.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _placedObjects.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		m.unlock();
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
		cout << "Id is not placedObjectId in Server (FindPlacedObject)" << endl;
		m.unlock();
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindAllObject(uint64 _targetId)
{
	std::mutex m;
	m.lock();

	_allObjects.insert(_players.begin(), _players.end());
	_allObjects.insert(_objects.begin(), _objects.end());
	_allObjects.insert(_placedObjects.begin(), _placedObjects.end());

	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _allObjects.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _allObjects.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;

		m.unlock();
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
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
	// ���� �÷��̾��� �������� ������ ������. 
	CGameObject* obj = FindPlayer(MyPlayer.id);

	if (obj == nullptr || obj->GetLayerIndex() == -1)
		return;

	Vec3  CurPos = obj->Transform()->GetRelativePos();

	if (PrevPos == CurPos) // ���� ��ǥ�� ��ȭ�� ���ٸ� move packet�� ������ �ʴ´�. return
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

	// �������� ��Ŷ ����
	std::cout << "C_PLAYER_MOVE Pakcet. id : "<< MyPlayer.id << endl;
	PKT_C_PLAYER_MOVE_WRITE pktWriter(move);
	SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
	_service->Broadcast(sendBuffer);
	std::cout << "===============================" << endl;

}

void GameObjMgr::SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
	// ������Ʈ�� �������� ������ ������.
	CGameObject* obj = FindObject(_id);
	
	if (obj == nullptr || obj->GetLayerIndex() == -1)
		return;
	
	Vec3  CurPos = obj->Transform()->GetRelativePos();
	
	auto it = _objectsPrevPos.find(_id);
	if (it != _objectsPrevPos.end()) // PrevPos�� �ִ�. 	
	{
		// ���� ��ǥ�� �Ȱ��ٸ� move packet�� ������ �ʴ´�. return
		if (_objectsPrevPos.at(_id) == CurPos) 
			return;
	
		_objectsPrevPos.at(_id) = CurPos; // ���� ��ǥ�� ������ǥ�� ����
		
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
	
		// �������� ��Ŷ ����
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

void GameObjMgr::SendPlacedObjectUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
	// ��ġ�� ������Ʈ�� ������Ʈ�� ������ ������. (HP ����ÿ���)
	CGameObject* obj = FindObject(_id);

	if (obj == nullptr || obj->GetLayerIndex() == -1)
		return;

	float CurHP = obj->GetScript<CUnitScript>()->GetCurHP();
	auto it = _placedObjectsPrevHP.find(_id);
	if (it != _placedObjectsPrevHP.end()) // PrevHP�� �ִ�. 	
	{
		// ���� HP�� �Ȱ��ٸ� move packet�� ������ �ʴ´�. return
		if (_placedObjectsPrevHP.at(_id) == CurHP)
			return;

		_placedObjectsPrevHP.at(_id) = CurHP; // ���� HP�� ���� HP�� ����

		ObjectMove updatePlacedObject = {};

		float CurMP = obj->GetScript<CUnitScript>()->GetCurMP();
		float CurAttackPower = obj->GetScript<CUnitScript>()->GetAttackPower();
		float CurDefencePower = obj->GetScript<CUnitScript>()->GetDefencePower();

		updatePlacedObject.HP = CurHP;
		updatePlacedObject.MP = CurMP;
		updatePlacedObject.AttackPower = CurAttackPower;
		updatePlacedObject.DefencePower = CurDefencePower;

		Vec3 CurPos = obj->Transform()->GetRelativePos();
		Vec3 CurRot = obj->Transform()->GetRelativeRot();

		updatePlacedObject.pos.x = CurPos.x;
		updatePlacedObject.pos.y = CurPos.y;
		updatePlacedObject.pos.z = CurPos.z;
		updatePlacedObject.moveDir.x = CurRot.x;
		updatePlacedObject.moveDir.y = CurRot.y;
		updatePlacedObject.moveDir.z = CurRot.z;

		// �������� ��Ŷ ����
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

void GameObjMgr::SendSkillSpawn(SkillInfo* _skillInfo, ClientServiceRef _service)
{
	// ��ų�� �� ������Ʈ�� ���� ��Ŷ�� ������. 
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

		// �������� ��Ŷ ����
		std::cout << "Send C_SKILL_PROJECTILE Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendSkillHit(HitInfo* _hitInfo, ClientServiceRef _service)
{
	// hitOjbId�� ��ų�� �¾Ҵٰ� ������. 
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

		// �������� ��Ŷ ����
		std::cout << "Send C_SKILL_HIT Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendDespawn(UINT64 _despawnId, float lifespan, ClientServiceRef _service)
{
	// �� ������Ʈ�� lifespan�ð��ڿ� �����϶�� �������� ������.
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		PKT_C_DESPAWN_OBJECT_WRITE  pktWriter(_despawnId, lifespan);

		// �������� ��Ŷ ����
		std::cout << "Send C_DESPAWN_OBJECT Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}

void GameObjMgr::SendObjectAnim(AnimInfo* _animInfo, ClientServiceRef _service)
{
	// _id ������Ʈ�� �ִϸ��̼��� ������.
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		AnimInfoPacket animInfoPacket = {};
		animInfoPacket.targetId = _animInfo->targetId;
		animInfoPacket.bRepeat = _animInfo->bRepeat;
		animInfoPacket.blend = _animInfo->blend;
		animInfoPacket.blendTime = _animInfo->blendTime;


		wstring _animName = _animInfo->animName;

		// ������ ����� Id	  : MyPlayer.id
		// ����� ������Ʈ�� Id : animInfo.targetId
		PKT_C_OBJECT_ANIM_WRITE  pktWriter(MyPlayer.id, animInfoPacket);
		PKT_C_OBJECT_ANIM_WRITE::AnimNameList animNamePacket = pktWriter.ReserveAnimNameList(_animName.size());
		for (int i = 0; i < _animName.size(); i++)
		{
			animNamePacket[i] = { _animName[i] };
		}

		// �������� ��Ŷ ����
		std::cout << "Send C_OBJECT_ANIM Pakcet " << endl;
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}


