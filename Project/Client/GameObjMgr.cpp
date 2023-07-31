#include "pch.h"
#include "GameObjMgr.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CEventMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\COtherPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CCameraMoveScript.h>
#include <Engine\CPathFindMgr.h>

CGameObject* GameObjMgr::Find(uint16 _targetId)
{
	// find 함수를 사용하여 원하는 키 값을 가진 원소를 찾습니다.
	std::map<uint16, CGameObject*>::iterator iter = _players.find(_targetId);

	// 원하는 원소가 존재하는지 확인합니다.
	if (iter != _players.end()) {

		// 원하는 원소를 찾았을 때의 처리
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// 원하는 원소가 없을 때의 처리
		cout << "Id is not objectId in Server";
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

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Jinx");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->AddComponent(new CPlayerScript);
		pObj->AddComponent(new CPathFinder);
		pObj->AddComponent(new CCollider3D);

		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pObj->Collider3D()->SetAbsolute(true);
		pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		pObj->Collider3D()->SetDrawCollision(false);
		pObj->Animator3D()->SetRepeat(true);
		pObj->Animator3D()->Play(L"Jinx\\Idle1_Base", true, 0.1f);
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

		pObj->Transform()->SetUseMouseOutline(true);

		SpawnGameObject(pObj, Vec3(0, 0, 0), 0);

		if (myPlayer)
		{
			// playerScript에 _info 변수들 추가하기.

			Ptr<CMeshData> pMeshData = nullptr;
			CGameObject* pObj = new CGameObject;

			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
			pObj = pMeshData->Instantiate();

			pObj->AddComponent(new CPlayerScript);
			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CCollider3D);

			MyPlayerScript = pObj->GetScript<CPlayerScript>();
			MyPlayerScript->SetPlayerID(MyPlayer.id);
			MyPlayerScript->SetNickName(MyPlayer.nickname);
			MyPlayerScript->SetFaction(MyPlayer.faction);

			pObj->SetName(MyPlayer.nickname);
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
			pObj->Animator3D()->Play(L"Jinx\\Idle1_Base", true, 0.1f);
			pObj->Animator3D()->SetRepeat(true);

			pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			pObj->Collider3D()->SetDrawCollision(false);

			pObj->GetRenderComponent()->SetFrustumCheck(false);

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
			pObj->Animator3D()->Play(L"Jinx\\Idle1_Base", true, 0.15f);

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

void GameObjMgr::MovePlayer(uint16 _playerId, PlayerMove _playerMove)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		if (_playerId == MyPlayer.id) // 내 플레이어가 움직인건 반영하지 않아도 된다. 
			return;

		CGameObject* obj = Find(_playerId);

		tEvent evn = {};

		evn.Type = EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// PlayerMove 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
		PlayerMove* temp = new PlayerMove(_playerMove);
		evn.lParam = (DWORD_PTR)temp;

		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void GameObjMgr::tick(ClientServiceRef _service)
{
	// 모든 플레이어의 움직임을 서버에 보낸다. 
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = Find(MyPlayer.id);

		if (obj == nullptr)
			return;

		if (obj->GetLayerIndex() == -1)
			return;

		PlayerMove move = {};
		Vec3 tempPos = obj->Transform()->GetRelativePos();
		Vec3 temoRot = obj->Transform()->GetRelativeRot();
		move.moveDir.x = temoRot.x;
		move.moveDir.y = temoRot.y;
		move.moveDir.z = temoRot.z;
		move.pos.x = tempPos.x;
		move.pos.y = tempPos.y;
		move.pos.z = tempPos.z;
		move.state = PlayerMove::PlayerState::IDLE;
		Send_CMove(_service, move);
	}
}

GameObjMgr::GameObjMgr()
{
}

GameObjMgr::~GameObjMgr()
{
	_players.clear();
}

