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

void GameObjMgr::AddPlayer(PlayerInfo _info, bool myPlayer)
{

	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		
		if (myPlayer)
		{
			// playerScript에 _info 변수들 추가하기.

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

		// 방장만 진짜를 생성한다. 나머지는 가짜를 생성한다.
		if (MyPlayer.host)
		{
			// 미니언 스크립트
			// 디버깅용으로 구체 띄워야지
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
			// 기본 OtherObjectScript
			// 미니언 스크립트
			// 디버깅용으로 구체 띄워야지
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
	// 본인 플레이어의 움직임을 서버에 보낸다. 
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

		if (PrevPos == CurPos) // 이전 좌표와 변화가 없다면 move packet을 보내지 않는다. return
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

		// 서버에게 패킷 전송
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
	// 오브젝트의 움직임을 서버에 보낸다. 
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
		if (it != _objectsPrevPos.end()) // PrevPos가 있다. 	
		{
			// 이전 좌표와 똑같다면 move packet을 보내지 않는다. return
			if (_objectsPrevPos.at(_id) == CurPos) 
				return;

			_objectsPrevPos.at(_id) = CurPos; // 현재 좌표를 이전좌표로 저장

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

void GameObjMgr::SendObjectAnim(uint64 _id, ClientServiceRef _service)
{
	// _id 오브젝트의 애니메이션을 보낸다.
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

		// 서버에게 패킷 전송
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

		if (_playerId == MyPlayer.id) // 내 플레이어가 움직인건 반영하지 않아도 된다. 
			return;

		CGameObject* obj = FindPlayer(_playerId);

		tServerEvent evn = {};

		evn.Type = SERVER_EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// ObjectMove 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
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

		if (_objectId == MyPlayer.id) // 내 플레이어가 움직인건 반영하지 않아도 된다. 
			return;

		CGameObject* obj = FindObject(_objectId);

		tServerEvent evn = {};

		evn.Type = SERVER_EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// ObjectMove 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
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

		if (_objectId == MyPlayer.id) // 내 플레이어 애니메이션은 반영하지 않아도 된다. 
			return;

		CGameObject* obj = FindAllObject(_objectId);

		tServerEvent evn = {};

		evn.Type = SERVER_EVENT_TYPE::ANIM_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// AnimInfo 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
		AnimInfo* animInfo = new AnimInfo(_animInfo);
		evn.lParam = (DWORD_PTR)animInfo;

		ServerEventMgr::GetInst()->AddEvent(evn);
	}
}

