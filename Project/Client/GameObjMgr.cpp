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
	//std::mutex m;
	//m.lock();

	if (myPlayer)
	{
		// playerScript에 _info 변수들 추가하기.

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = new CGameObject;

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();

		pObj->AddComponent(new CPlayerScript);
		pObj->AddComponent(new CPathFinder);

		MyPlayerScript = pObj->GetScript<CPlayerScript>();
		MyPlayerScript->SetPlayerID(MyPlayer.id);
		MyPlayerScript->SetNickName(MyPlayer.nickname);
		MyPlayerScript->SetFaction(MyPlayer.faction);

		pObj->SetName(MyPlayer.nickname);
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);

		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		
		Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
		SpawnGameObject(pObj, spawnPos, 0);

		_players.insert(std::make_pair(MyPlayer.id,pObj));
	}
	else
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = new CGameObject;

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();

		pObj->AddComponent(new COtherPlayerScript);
		pObj->AddComponent(new CPathFinder);

		COtherPlayerScript* player = pObj->GetScript<COtherPlayerScript>();
		player->SetPlayerID(_info.id);
		player->SetNickName(_info.nickname);
		player->SetFaction(_info.faction);

		//pObj->SetName(_info.nickname);
		pObj->SetName(L"OtherPlayer");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);
		pObj->Animator3D()->Play(L"Jinx\\Run_Base", true, 0.15f);
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
		SpawnGameObject(pObj, spawnPos, 0);

		_players.insert(std::make_pair(_info.id, pObj));
	}

	//m.unlock();
}

void GameObjMgr::MovePlayer(uint16 _playerId, PlayerMove _playerMove)
{
	std::mutex m;
	std::lock_guard<std::mutex> lock(m);
	//m.lock();

	if (_playerId == MyPlayer.id) // 내 플레이어가 움직인건 반영하지 않아도 된다. 
		return;
	
	CGameObject* obj = Find(_playerId);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::MOVE_PACKET;
	evn.wParam = (DWORD_PTR)obj;
	
	// PlayerMove 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
	PlayerMove* temp = new PlayerMove(_playerMove);
	//shared_ptr<PlayerMove> temp = make_shared<PlayerMove>(_playerMove);
	evn.lParam = (DWORD_PTR)temp;

	CEventMgr::GetInst()->AddEvent(evn);

	//delete temp;
	//m.unlock();
}

void GameObjMgr::tick(ClientServiceRef _service)
{
	// 모든 플레이어의 움직임을 서버에 보낸다. 
	std::mutex m;
	//m.lock();
	{
		std::lock_guard<std::mutex> lock(m);

		CGameObject* obj = Find(MyPlayer.id);

		if (obj == nullptr)
			return;

		if (obj->GetLayerIndex() == -1)
			return;

		PlayerMove move = {};
		Vec3 tempPos = obj->Transform()->GetRelativePos();
		move.moveDir.x = 1.f;
		move.moveDir.y = 2.f;
		move.moveDir.z = 3.f;
		move.pos.x = tempPos.x;
		move.pos.y = tempPos.y;
		move.pos.z = tempPos.z;
		move.state = PlayerMove::PlayerState::IDLE;
		Send_CMove(_service, move);

		//for (int i = 0; i < _players.size(); ++i)
		//{
		//	CGameObject* obj = _players[i];
		//
		//	if (obj == nullptr)
		//		continue;
		//	
		//	if (obj->GetLayerIndex() == -1)
		//		continue;
		//
		//	PlayerMove move = {};
		//	Vec3 tempPos = _players[i]->Transform()->GetRelativePos();
		//	move.moveDir.x = 1.f;
		//	move.moveDir.y = 2.f;
		//	move.moveDir.z = 3.f;
		//	move.pos.x = tempPos.x;
		//	move.pos.y = tempPos.y;
		//	move.pos.z = tempPos.z;
		//	move.state = PlayerMove::PlayerState::IDLE;
		//	Send_CMove(_service, move);
		//}

		//m.unlock();
	}
}

GameObjMgr::GameObjMgr()
{
}

GameObjMgr::~GameObjMgr()
{
	_players.clear();
}

