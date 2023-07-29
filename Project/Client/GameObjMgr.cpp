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

#include <Script\CPlayerScript.h>
#include <Script\CJinxScript.h>
#include <Script\CChampionScript.h>
#include <Script\COtherPlayerScript.h>
#include <Script\CCameraMoveScript.h>
#include "ServerEventMgr.h"

GameObjMgr::GameObjMgr()
{

}
GameObjMgr::~GameObjMgr()
{

}


CGameObject* GameObjMgr::FindPlayer(uint64 _targetId)
{
	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _players.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _players.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
		cout << "Id is not playerId in Server" << endl;
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindObject(uint64 _targetId)
{	
	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _objects.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _objects.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
		cout << "Id is not objectId in Server" << endl;
		return nullptr;
	}
}

CGameObject* GameObjMgr::FindAllObject(uint64 _targetId)
{
	_allObjects.insert(_players.begin(), _players.end());
	_allObjects.insert(_objects.begin(), _objects.end());
	_allObjects.insert(_towers.begin(), _towers.end());

	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint64, CGameObject*>::iterator iter = _allObjects.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _allObjects.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
		cout << "Id is not in Server" << endl;
		return nullptr;
	}
}

void GameObjMgr::AddPlayer(PlayerInfo _info, bool myPlayer)
{

	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		
		if (myPlayer)
		{
			// playerScript�� _info ������ �߰��ϱ�.

			Ptr<CMeshData> pMeshData = nullptr;
			CGameObject* pObj = new CGameObject;

			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
			pObj = pMeshData->Instantiate();

			pObj->AddComponent(new CJinxScript);
			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CFsm);
			//pObj->AddComponent(new CCollider3D);

			MyPlayerScript = pObj->GetScript<CChampionScript>();
			MyPlayerScript->SetServerID(MyPlayer.id);
			//MyPlayerScript->SetNickName(MyPlayer.nickname);
			//MyPlayerScript->SetFaction(MyPlayer.faction);

			//pObj->SetName(MyPlayer.nickname);
			pObj->SetName(L"My Player");
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
			pObj->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true, 0.1f);
			//pObj->Animator3D()->SetRepeat(true);

			//pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			//pObj->Collider3D()->SetAbsolute(true);
			//pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			//pObj->Collider3D()->SetDrawCollision(false);

			//pObj->GetRenderComponent()->SetFrustumCheck(false);

			pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));


			Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
			SpawnGameObject(pObj, spawnPos, 0);

			_players.insert(std::make_pair(MyPlayer.id, pObj));
		}
		else
		{
			Ptr<CMeshData> pMeshData = nullptr;
			CGameObject* pObj = new CGameObject;

			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
			pObj = pMeshData->Instantiate();

			pObj->AddComponent(new COtherPlayerScript);
			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CCollider3D);

			COtherPlayerScript* player = pObj->GetScript<COtherPlayerScript>();
			player->SetPlayerID(_info.id);
			player->SetNickName(_info.nickname);
			player->SetFaction(_info.faction);

			//pObj->SetName(_info.nickname);
			pObj->SetName(L"OtherPlayer");
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
			pObj->Animator3D()->SetRepeat(true);
			pObj->Animator3D()->PlayRepeat (L"Jinx\\Idle1_Base", true, 0.15f);

			pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			pObj->Collider3D()->SetDrawCollision(false);

			pObj->GetRenderComponent()->SetFrustumCheck(false);
			pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

			Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
			SpawnGameObject(pObj, spawnPos, 0);

			_players.insert(std::make_pair(_info.id, pObj));
		}
	}
}

void GameObjMgr::AddObject(uint64 _objectId, ObjectInfo _objectInfo)
{

	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		// ���常 ��¥�� �����Ѵ�. �������� ��¥�� �����Ѵ�.
		if (MyPlayer.host)
		{
			// �̴Ͼ� ��ũ��Ʈ
			// ���������� ��ü �������
			CGameObject* pObj = new CGameObject;

			//pObj->AddComponent(new CMeshRender);
			pObj->AddComponent(new CTransform);
			pObj->AddComponent(new COtherPlayerScript);
			pObj->AddComponent(new CCollider3D);

			COtherPlayerScript* Script = pObj->GetScript<COtherPlayerScript>();
			Script->SetPlayerID(_objectId);
			Script->SetFaction(_objectInfo.factionType);
			
			pObj->SetName(L"Minion");

			pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			pObj->Collider3D()->SetDrawCollision(true);

			pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));


			Vec3 spawnPos = Vec3(100.f + (50 * _objects.size()), 30.f, 100.f);
			SpawnGameObject(pObj, spawnPos, 0);

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		else
		{
			// �⺻ OtherObjectScript
			// �̴Ͼ� ��ũ��Ʈ
			// ���������� ��ü �������
			CGameObject* pObj = new CGameObject;

			//pObj->AddComponent(new CMeshRender);
			pObj->AddComponent(new CTransform);
			pObj->AddComponent(new COtherPlayerScript);
			pObj->AddComponent(new CCollider3D);

			COtherPlayerScript* Script = pObj->GetScript<COtherPlayerScript>();
			Script->SetPlayerID(_objectId);
			Script->SetFaction(_objectInfo.factionType);

			pObj->SetName(L"Minion");

			pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			pObj->Collider3D()->SetDrawCollision(true);

			pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));


			Vec3 spawnPos = Vec3(100.f + (50 * _objects.size()), 30.f, 100.f);
			SpawnGameObject(pObj, spawnPos, 0);

			_objects.insert(std::make_pair(_objectId, pObj));
		}


		
	}
}


void GameObjMgr::SendMyPlayerMove(ClientServiceRef _service)
{
	// ���� �÷��̾��� �������� ������ ������. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = FindPlayer(MyPlayer.id);

		if (obj == nullptr)
			return;

		if (obj->GetLayerIndex() == -1)
			return;

		Vec3 CurPos = obj->Transform()->GetRelativePos();
		Vec3 CurRot = obj->Transform()->GetRelativeRot();
		// float CurLV = obj->?()->GetLV();
		// float CurHP = obj->?()->GetHP();
		// float CurMP = obj->?()->GetMP();
		// float CurAD = obj->?()->GetAD();
		// float CurDefence = obj->?()->GetDefence();

		if (PrevPos == CurPos) // ���� ��ǥ�� ��ȭ�� ���ٸ� move packet�� ������ �ʴ´�. return
			return;

		PrevPos = CurPos;

		ObjectMove move = {};
		//move.LV = CurLV;
		//move.HP = CurHP;
		//move.MP = CurMP;
		//move.AD = CurAD;
		//move.Defence = CurDefence;
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
		//sendBuffer->Close(sendBuffer->WriteSize());
	}
}

void GameObjMgr::SendObjectMove(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
	// ������Ʈ�� �������� ������ ������. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = FindObject(_id);

		if (obj == nullptr)
			return;

		if (obj->GetLayerIndex() == -1)
			return;

		Vec3 CurPos = obj->Transform()->GetRelativePos();
		Vec3 CurRot = obj->Transform()->GetRelativeRot();

		auto it = _objectsPrevPos.find(_id);
		if (it != _objectsPrevPos.end()) // PrevPos�� �ִ�. 	
		{
			// ���� ��ǥ�� �Ȱ��ٸ� move packet�� ������ �ʴ´�. return
			if (_objectsPrevPos.at(_id) == CurPos) 
				return;

			_objectsPrevPos.at(_id) = CurPos; // ���� ��ǥ�� ������ǥ�� ����

			// float CurLV = obj->?()->GetLV();
			// float CurHP = obj->?()->GetHP();
			// float CurMP = obj->?()->GetMP();
			// float CurAD = obj->?()->GetAD();
			// float CurDefence = obj->?()->GetDefence();

			ObjectMove move = {};
			//move.LV = CurLV;
			//move.HP = CurHP;
			//move.MP = CurMP;
			//move.AD = CurAD;
			//move.Defence = CurDefence;
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
}

void GameObjMgr::SendTowerUpdate(uint64 _id, CGameObject* _obj, ClientServiceRef _service)
{
}

void GameObjMgr::SendObjectAnim(uint64 _id, ClientServiceRef _service)
{
	// _id ������Ʈ�� �ִϸ��̼��� ������.
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = FindAllObject(_id);

		if (obj == nullptr)
			return;

		if (obj->GetLayerIndex() == -1)
			return;

		AnimInfoPacket animInfoPacket = {};
		animInfoPacket.blend = obj->Animator3D()->IsBlend();
		animInfoPacket.blendTime = obj->Animator3D()->GetBlendTime();

		wstring _animName = obj->Animator3D()->GetCurAnim()->GetName();

		// �������� ��Ŷ ����
		std::cout << "C_OBJECT_ANIM Pakcet. id : " << _id << endl;
		PKT_C_OBJECT_ANIM_WRITE  pktWriter(_id, animInfoPacket);
		PKT_C_OBJECT_ANIM_WRITE::AnimNameList animNamePacket = pktWriter.ReserveAnimNameList(_animName.size());
		for (int i = 0; i < _animName.size(); i++)
		{
			animNamePacket[i] = { _animName[i] };
		}

		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		_service->Broadcast(sendBuffer);

		std::cout << "===============================" << endl;
	}
}


void GameObjMgr::E_MovePlayer(uint64 _playerId, ObjectMove _playerMove)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		if (_playerId == MyPlayer.id) // �� �÷��̾ �����ΰ� �ݿ����� �ʾƵ� �ȴ�. 
			return;

		CGameObject* obj = FindPlayer(_playerId);

		tServerEvent evn = {};

		evn.Type = SERVER_EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// ObjectMove ����ü�� �����͸� DWORD_PTR�� ĳ�����Ͽ� lParam�� ����
		ObjectMove* objMove = new ObjectMove(_playerMove);
		evn.lParam = (DWORD_PTR)objMove;

		ServerEventMgr::GetInst()->AddEvent(evn);
	}
}

void GameObjMgr::E_MoveObject(uint64 _objectId, ObjectMove _objectMove)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		if (_objectId == MyPlayer.id) // �� �÷��̾ �����ΰ� �ݿ����� �ʾƵ� �ȴ�. 
			return;

		CGameObject* obj = FindObject(_objectId);

		tServerEvent evn = {};

		evn.Type = SERVER_EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// ObjectMove ����ü�� �����͸� DWORD_PTR�� ĳ�����Ͽ� lParam�� ����
		ObjectMove* objMove = new ObjectMove();
		objMove->AD = _objectMove.AD;
		objMove->Defence = _objectMove.Defence;
		objMove->HP = _objectMove.HP;
		objMove->LV = _objectMove.LV;
		objMove->moveDir = _objectMove.moveDir;
		objMove->MP = _objectMove.MP;
		objMove->pos = _objectMove.pos;
		evn.lParam = (DWORD_PTR)objMove;

		ServerEventMgr::GetInst()->AddEvent(evn);
	}
}

void GameObjMgr::E_ObjectAnim(uint64 _objectId, AnimInfo _animInfo)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		if (_objectId == MyPlayer.id) // �� �÷��̾� �ִϸ��̼��� �ݿ����� �ʾƵ� �ȴ�. 
			return;

		CGameObject* obj = FindAllObject(_objectId);

		tServerEvent evn = {};

		evn.Type = SERVER_EVENT_TYPE::ANIM_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// AnimInfo ����ü�� �����͸� DWORD_PTR�� ĳ�����Ͽ� lParam�� ����
		AnimInfo* animInfo = new AnimInfo(_animInfo);
		evn.lParam = (DWORD_PTR)animInfo;

		ServerEventMgr::GetInst()->AddEvent(evn);
	}
}

