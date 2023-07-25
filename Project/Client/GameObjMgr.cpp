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
	// find �Լ��� ����Ͽ� ���ϴ� Ű ���� ���� ���Ҹ� ã���ϴ�.
	std::map<uint16, CGameObject*>::iterator iter = _players.find(_targetId);

	// ���ϴ� ���Ұ� �����ϴ��� Ȯ���մϴ�.
	if (iter != _players.end()) {

		// ���ϴ� ���Ҹ� ã���� ���� ó��
		CGameObject* pObj = iter->second;
		return iter->second;
	}
	else {
		// ���ϴ� ���Ұ� ���� ���� ó��
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
		// playerScript�� _info ������ �߰��ϱ�.

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
	{
		std::lock_guard<std::mutex> lock(m);

		if (_playerId == MyPlayer.id) // �� �÷��̾ �����ΰ� �ݿ����� �ʾƵ� �ȴ�. 
			return;

		CGameObject* obj = Find(_playerId);

		tEvent evn = {};

		evn.Type = EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// PlayerMove ����ü�� �����͸� DWORD_PTR�� ĳ�����Ͽ� lParam�� ����
		PlayerMove* temp = new PlayerMove(_playerMove);
		evn.lParam = (DWORD_PTR)temp;

		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void GameObjMgr::tick(ClientServiceRef _service)
{
	// ��� �÷��̾��� �������� ������ ������. 
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

