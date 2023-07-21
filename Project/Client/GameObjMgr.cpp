#include "pch.h"
#include "GameObjMgr.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

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
		//_info.posInfo.x
		//Vec3 spawnPos = 
		SpawnGameObject(pObj, Vec3(0, 0, 0), 0);

		_players.insert(std::make_pair(MyPlayer.id,pObj));
	}
	else
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = new CGameObject;

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"OtherJinx");

		pObj->AddComponent(new COtherPlayerScript);
		pObj->AddComponent(new CPathFinder);

		COtherPlayerScript* player = pObj->GetScript<COtherPlayerScript>();
		player->SetPlayerID(_info.id);
		player->SetNickName(_info.nickname);
		player->SetFaction(_info.faction);

		pObj->SetName(_info.nickname);
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);

		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		SpawnGameObject(pObj, Vec3(100, 0, 200), 0);

		_players.insert(std::make_pair(_info.id, pObj));
	}
}

void GameObjMgr::MovePlayer(uint16 _playerId, PlayerMove _playerMove)
{
	CGameObject* obj = Find(_playerId);

	CTransform* trans = (CTransform*)obj->GetComponent(COMPONENT_TYPE::TRANSFORM);
	trans->SetRelativePos(Vec3(_playerMove.pos.x, _playerMove.pos.y, _playerMove.pos.z));
}

GameObjMgr::GameObjMgr()
{
}

GameObjMgr::~GameObjMgr()
{
	_players.clear();
}

