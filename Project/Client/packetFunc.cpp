#include "pch.h"
#include "packetFunc.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CResMgr.h>

#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Script\CPlayerScript.h>
#include <Script\COtherPlayerScript.h>


void SpawnMyChampion(CHAMPION_TYPE _type, Vec3 _vWorldPos)
{
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;

	switch (_type)
	{
	case CHAMPION_TYPE::JINX:
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Jinx");

		pObj->AddComponent(new CPlayerScript);
		pObj->AddComponent(new CPathFinder);

		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);

		pObj->GetRenderComponent()->SetFrustumCheck(false);

		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		SpawnGameObject(pObj, _vWorldPos, 0);
	}
	break;
	case CHAMPION_TYPE::BLITZCRANL:
	{

	}
	break;


	default:
		break;
	}
}

void SpawnOtherChampion(CHAMPION_TYPE _type, Vec3 _vWorldPos)
{
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;

	switch (_type)
	{
	case CHAMPION_TYPE::JINX:
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Jinx");

		pObj->AddComponent(new COtherPlayerScript);
		pObj->AddComponent(new CPathFinder);

		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);

		pObj->GetRenderComponent()->SetFrustumCheck(false);

		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		SpawnGameObject(pObj, _vWorldPos, 0);
	}
	break;
	case CHAMPION_TYPE::BLITZCRANL:
	{

	}
	break;


	default:
		break;
	}
}