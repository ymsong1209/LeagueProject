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
#include <Script/CSendServerEventMgr.h>
#include <Script/CGrompScript.h>

#include <Script/CTurretScript.h>

#include <Script/CMurkWolfScript.h>
#include <Script/CMurkWolfMiniScript.h>
#include <Script/CRazorBeakScript.h>
#include <Script/CRazorBeakMiniScript.h>
#include <Script/CKrugScript.h>
#include <Script/CKrugMiniScript.h>
#include <Script/CBlueScript.h>
#include <Script/CRedScript.h>
#include <Script/CDragonScript.h>



// ===============================================
//   Add
// ===============================================
void GameObjMgr::AddPlayer(PlayerInfo _info, bool myPlayer)
{
	// ���� ���������� ����Ǳ� ���
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

		// è�Ǿ� Ÿ�԰� ������� ����
		if (myPlayer)
		{
			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CFsm);
			MyPlayerScript = pObj->GetScript<CUnitScript>();
			MyPlayerScript->SetServerID(_info.id);
			MyPlayerScript->SetNickname(_info.nickname);
			MyPlayerScript->SetHost(_info.host);
			MyPlayerScript->SetFaction(_info.faction);

			// Script ������Ʈ�� ���� �÷��̾� ���� ������Ʈ ���� (UI��)
			CSendServerEventMgr::GetInst()->SetMyPlayer(pObj);

			pObj->SetName(L"MyPlayer");


			// ��Ÿ� �ڽ� ������Ʈ �߰�
			CGameObject* AttackRange = new CGameObject;
			AttackRange->AddComponent(new CTransform);
			AttackRange->AddComponent(new CCollider2D);
			AttackRange->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			AttackRange->Collider2D()->SetOffsetScale(Vec2(2000.f, 2000.f));
			AttackRange->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			AttackRange->AddComponent(new CAttackRangeScript);
			AttackRange->SetName(L"AttackRange");
			pObj->AddChild(AttackRange);
			AttackRange->ChangeLayer(6);
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
		CUnitScript* pUnit = pObj->GetScript<CUnitScript>();
		pUnit->SetChampType(_info.champion);

		pObj->GetRenderComponent()->SetRaySightCulling(true);

		if (_info.faction == MyPlayer.faction)
		{
			pObj->Transform()->SetIsShootingRay(true);
			pObj->Transform()->SetRayRange(200.f);
		}
		else
		{
			pObj->Transform()->SetIsShootingRay(false);
		}


		pObj->AddComponent(new CCollider3D);
		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pObj->Collider3D()->SetAbsolute(true);
		pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		pObj->Collider3D()->SetDrawCollision(false);

		pObj->AddComponent(new CCollider2D);
		pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
		pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0, 0));


		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		pObj->Transform()->SetUseMouseOutline(true);
		pObj->Transform()->SetRelativeRot(Vec3(_info.posInfo.moveDir.x, _info.posInfo.moveDir.y, _info.posInfo.moveDir.z));
		Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
		SpawnGameObject(pObj, spawnPos, L"Player");

		_players.insert(std::make_pair(_info.id, pObj));
	}
}

void GameObjMgr::AddObject(uint64 _objectId, ObjectInfo _objectInfo)
{
	// ���� ���������� ����Ǳ� ���
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj;

		switch (_objectInfo.unitType)
		{

		case UnitType::MELEE_MINION:
		{
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MeleeMinion.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_minion_melee");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\minion_melee_Red.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_minion_melee");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\minion_melee_Blue.mtrl"), 0);
			}

			// ������ ��¥ ��� ������Ʈ ����,  ������ �ƴ� ��� ����� ����
			if (MyPlayer.host)
			{
				pObj->AddComponent(new CMinionScript);
				pObj->GetScript<CMinionScript>()->SetLane(_objectInfo.lane);
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
				pObj->GetScript<CUnitScript>()->SetLane(_objectInfo.lane);
			}

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::MELEE_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			SpawnGameObject(pObj,Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		break;
		case UnitType::RANGED_MINION:
		{
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RangedMinion.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_minion_ranged");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\minion_caster_red.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_minion_ranged");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\minion_caster_blue.mtrl"), 0);
			}

			// ������ ��¥ ��� ������Ʈ ����,  ������ �ƴ� ��� ����� ����
			if (MyPlayer.host)
			{
				pObj->AddComponent(new CMinionScript);
				pObj->GetScript<CMinionScript>()->SetLane(_objectInfo.lane);
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
				pObj->GetScript<CUnitScript>()->SetLane(_objectInfo.lane);
			}
			

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::RANGED_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		break;
		case UnitType::SIEGE_MINION:
		{
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SiegeMinion.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_minion_siege");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Minion_siege_Red.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_minion_siege");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Minion_siege_Blue.mtrl"), 0);
			}

			// ������ ��¥ ��� ������Ʈ ����,  ������ �ƴ� ��� ����� ����
			if (MyPlayer.host)
			{
				pObj->AddComponent(new CMinionScript);
				pObj->GetScript<CMinionScript>()->SetLane(_objectInfo.lane);
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
				pObj->GetScript<CUnitScript>()->SetLane(_objectInfo.lane);
			}

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::SIEGE_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		break;
		case UnitType::SUPER_MINION:
		{
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SuperMinion.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_minion_super");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\minion_super_Red.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_minion_super");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\minion_super_Blue.mtrl"), 0);
			}

			// ������ ��¥ ��� ������Ʈ ����,  ������ �ƴ� ��� ����� ����
				// ������ ��¥ ��� ������Ʈ ����,  ������ �ƴ� ��� ����� ����
			if (MyPlayer.host)
			{
				pObj->AddComponent(new CMinionScript);
				pObj->GetScript<CMinionScript>()->SetLane(_objectInfo.lane);
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
				pObj->GetScript<CUnitScript>()->SetLane(_objectInfo.lane);
			}

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::SUPER_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		break;
		// =======================================================================================================================
		//  
		// Jungle Mob(Objects) 
		// 
		// =======================================================================================================================
		case UnitType::SOUTH_GROMP://����� �β���
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\gromp.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\gromp");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_Gromp");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(270.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));
			
			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CGrompScript);

				CGrompScript* Script = pObj->GetScript<CGrompScript>();

				Script->SetAggroPos(Vec3(323.f, 0.f, 1242.f));
				SpawnGameObject(pObj, Vec3(323.f, 10.f, 1242.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(323.f, 10.f, 1242.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_GROMP);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_MURKWOLF://����� ����
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_MurkWolf");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(24.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));

			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CMurkWolfScript);

				CMurkWolfScript* script = pObj->GetScript <CMurkWolfScript>();

				script->SetAggroPos(Vec3(564.f, 0.f, 959.f));
				SpawnGameObject(pObj, Vec3(550.f, 15.f, 944.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(550.f, 15.f, 944.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_MURKWOLF_MINI_L://����� ���� °����(��)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_MurkWolf_Mini_L");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(36.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(564.f, 0.f, 964.f));
				SpawnGameObject(pObj, Vec3(552.f, 15.f, 964.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(552.f, 15.f, 964.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF_MINI_L);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_MURKWOLF_MINI_R://����� ���� °����(��)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_MurkWolf_Mini_R");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(36.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(564.f, 0.f, 964.f));
				SpawnGameObject(pObj, Vec3(580.f, 15.f, 944.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(580.f, 15.f, 944.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF_MINI_R);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_KRUG://����� ���ź�
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_Krug");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(40.f, 40.f, 40.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CKrugScript);

				CKrugScript* Script = pObj->GetScript<CKrugScript>();

				Script->SetAggroPos(Vec3(1238.f, 0.f, 389.f));
				SpawnGameObject(pObj, Vec3(1221.f, 15.f, 379.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1221.f, 15.f, 379.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_KRUG);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_KRUG_MINI://����� ���ź� °����
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_Krug_Mini");
			pObj->Transform()->SetRelativeScale(0.12f, 0.12f, 0.12f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CKrugMiniScript);

				CKrugMiniScript* Script = pObj->GetScript<CKrugMiniScript>();

				Script->SetAggroPos(Vec3(1238.f, 0.f, 389.f));
				SpawnGameObject(pObj, Vec3(1252.f, 15.f, 379.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1252.f, 15.f, 379.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_KRUG_MINI);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RAZORBEAK://����� Į���θ�
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_RazorBeak");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-66.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(25.f, 25.f, 25.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakScript);

				CRazorBeakScript* Script = pObj->GetScript<CRazorBeakScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1008.f, 15.f, 800.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1008.f, 15.f, 800.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_1://����� Į���θ� °���� 1
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_RazorBeak_Mini1");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1026.f, 15.f, 810.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1026.f, 15.f, 810.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_1);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_2://����� Į���θ� °���� 2
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_RazorBeak_Mini2");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(73.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 799.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 799.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_2);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_3://����� Į���θ� °���� 3
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_RazorBeak_Mini3");
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-90.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1031.f, 15.f, 782.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1031.f, 15.f, 782.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_3);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_4://����� Į���θ� °���� 4
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_RazorBeak_Mini4");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1012.f, 15.f, 765.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1012.f, 15.f, 765.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_4);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_5://����� Į���θ� °���� 5
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_RazorBeak_Mini5");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(69.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 767.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 767.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_5);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_BLUE://����� ���
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_blue");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"South_Blue");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.2f, 0.2f, 0.2f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(60.f, 60.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CBlueScript);

				CBlueScript* Script = pObj->GetScript<CBlueScript>();

				Script->SetAggroPos(Vec3(563.f, 0.f, 1164.f));
				SpawnGameObject(pObj, Vec3(563.f, 15.f, 1164.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(563.f, 15.f, 1164.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_BLUE);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::SOUTH_RED://����� ����
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_red.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_red");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"South_Red");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.35f, 0.35f, 0.35f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(46.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(80.f, 80.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRedScript);

				CRedScript* Script = pObj->GetScript<CRedScript>();

				Script->SetAggroPos(Vec3(1123, 15.f, 559.f));
				SpawnGameObject(pObj, Vec3(1136, 15.f, 580.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1136, 15.f, 580.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::SOUTH_RED);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_GROMP://������ �β���
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\gromp.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\gromp");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_Gromp");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(58.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CGrompScript);

				CGrompScript* Script = pObj->GetScript<CGrompScript>();

				Script->SetAggroPos(Vec3(1823.f, 0.f, 943.f));
				SpawnGameObject(pObj, Vec3(1859.f, 10.f, 956.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1859.f, 10.f, 956.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_GROMP);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_MURKWOLF://������ ����
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_MurkWolf");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(24.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));

			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CMurkWolfScript);

				CMurkWolfScript* script = pObj->GetScript <CMurkWolfScript>();

				script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
				SpawnGameObject(pObj, Vec3(1619.f, 15.f, 1236.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1619.f, 15.f, 1236.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_MURKWOLF);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_MURKWOLF_MINI_L://������ ���� °����(��)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_MurkWolf_Mini_L");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(-30.f), XMConvertToRadians(-180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
				SpawnGameObject(pObj, Vec3(1592.f, 19.f, 1232.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1592.f, 19.f, 1232.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_MURKWOLF_MINI_L);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_MURKWOLF_MINI_R://������ ���� °����(��)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_MurkWolf_Mini_R");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(-30.f), XMConvertToRadians(-180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
				SpawnGameObject(pObj, Vec3(1634, 15.f, 1208.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1634, 15.f, 1208.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_MURKWOLF_MINI_R);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_KRUG://������ ���ź�
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_Krug");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.25f, 0.25f, 0.25f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-4.f), XMConvertToRadians(-180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(40.f, 40.f, 40.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CKrugScript);

				CKrugScript* Script = pObj->GetScript<CKrugScript>();

				Script->SetAggroPos(Vec3(942.f, 0.f, 1845.f));
				SpawnGameObject(pObj, Vec3(966, 15.f, 1800.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(966, 15.f, 1800.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_KRUG);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_KRUG_MINI://������ ���ź� °����
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"SOUTH_Krug_Mini");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(4.f), XMConvertToRadians(180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CKrugMiniScript);

				CKrugMiniScript* Script = pObj->GetScript<CKrugMiniScript>();

				Script->SetAggroPos(Vec3(942.f, 0.f, 1845.f));
				SpawnGameObject(pObj, Vec3(935.f, 15.f, 1791.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(935.f, 15.f, 1791.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_KRUG_MINI);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RAZORBEAK://������ Į���θ�
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_RazorBeak");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(90.f), XMConvertToRadians(180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(25.f, 25.f, 25.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakScript);

				CRazorBeakScript* Script = pObj->GetScript<CRazorBeakScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1170.f, 15.f, 1390.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1170.f, 15.f, 1390.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_1://������ Į���θ� °���� 1
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_RazorBeak_Mini1");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-46.f), XMConvertToRadians(180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1139, 15.f, 1419.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1139, 15.f, 1419.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_1);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_2://������ Į���θ� °���� 2
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_RazorBeak_Mini2");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(52.f), XMConvertToRadians(180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1176, 15.f, 1419.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1176, 15.f, 1419.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_2);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_3://������ Į���θ� °���� 3
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_RazorBeak_Mini3");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1159, 15.f, 1405.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1031.f, 15.f, 782.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_3);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_4://������ Į���θ� °���� 4
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_RazorBeak_Mini4");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(0.f), XMConvertToRadians(-90.f), XMConvertToRadians(0.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1140, 15.f, 1385.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1140, 15.f, 1385.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_4);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_5://����� Į���θ� °���� 5
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_RazorBeak_Mini5");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(0.f), XMConvertToRadians(180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1157, 15.f, 1370.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1157, 15.f, 1370.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_5);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_BLUE://������ ���
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_blue");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"NORTH_Blue");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.22f, 0.22f, 0.22f);
			pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-57.f), 0.f));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(60.f, 60.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CBlueScript);

				CBlueScript* Script = pObj->GetScript<CBlueScript>();

				Script->SetAggroPos(Vec3(1653.f, 0.f, 1011.f));
				SpawnGameObject(pObj, Vec3(1636, 10.f, 1009.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1636, 10.f, 1009.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_BLUE);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::NORTH_RED://������ ����
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_red.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_red");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			pObj->SetName(L"North_Red");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.35f, 0.35f, 0.35f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-54.f), XMConvertToRadians(-180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));

			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(80.f, 80.f));
			if (MyPlayer.host) // ������ ��쿡 ��¥
			{
				pObj->AddComponent(new CRedScript);

				CRedScript* Script = pObj->GetScript<CRedScript>();

				Script->SetAggroPos(Vec3(1055, 15.f, 1637.f));
				SpawnGameObject(pObj, Vec3(1049, 15.f, 1607.f), L"Mob");
			}
			else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1049, 15.f, 1607.f), L"Mob");
			}

			// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // ���� id
			Script->SetUnitType(UnitType::NORTH_RED);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		case UnitType::DRAGON:// �巡��
		{
			//pMeshData = nullptr;
			//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Elder_Dragon.fbx");
			//pObj = nullptr;
			//pObj = pMeshData->Instantiate();
			//pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Elder_Dragon");  // ���θ��� ���⼭ ����� �󲮵��� �β��� �ִϸ��̼��� �ȴ�.
			//pObj->SetName(L"Elder_Dragon");
			//pObj->Transform()->SetUseMouseOutline(true);
			//pObj->Transform()->SetRelativeScale(0.33f, 0.33f, 0.33f);
			//pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-44.f), XMConvertToRadians(-180.f)));
			//pObj->AddComponent(new CCollider3D);
			//pObj->Collider3D()->SetAbsolute(true);
			//pObj->Collider3D()->SetDrawCollision(false);
			//pObj->Collider3D()->SetOffsetScale(Vec3(125.f, 125.f, 125.f));
			//
			//pObj->AddComponent(new CCollider2D);
			//pObj->Collider2D()->SetAbsolute(true);
			//pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			//pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			//pObj->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));
			//if (MyPlayer.host) // ������ ��쿡 ��¥
			//{
			//	pObj->AddComponent(new CDragonScript);
			//
			//	CDragonScript* Script = pObj->GetScript<CDragonScript>();
			//
			//	Script->SetAggroPos(Vec3(1451, 15.f, 649.f));
			//	Script->SetAlertPos(Vec3(1483, 15.f, 689.f));
			//	SpawnGameObject(pObj, Vec3(1451, 0.f, 656.f), L"Mob");
			//}
			//else // ������ �ƴ� ��쿡�� ��¥�� ����(�󲮵���)
			//{
			//	pObj->AddComponent(new CUnitScript);
			//	CUnitScript* Script = pObj->GetScript<CUnitScript>();
			//	SpawnGameObject(pObj, Vec3(1451, 0.f, 656.f), L"Mob");
			//}
			//
			//// UnitScript �� ��¥��, ��¥�� ���������� ���� �ϴ� ����.
			//CUnitScript* Script = pObj->GetScript<CUnitScript>();
			//Script->SetServerID(_objectId);  // ���� id
			//Script->SetUnitType(UnitType::DRAGON);  // UnitType
			//Script->SetFaction(_objectInfo.faction);    // ���� ���۸���: NONE
			//pObj->GetRenderComponent()->SetFrustumCheck(true);
			//pObj->GetRenderComponent()->SetRaySightCulling(true);
			//pObj->Transform()->SetIsShootingRay(false);
			//pObj->Transform()->SetRayRange(0.f);
			//_objects.insert(std::make_pair(_objectId, pObj));   // ������ �����ϵ��� �� �־����!! make_pair(����id, GameObject*)
		}
		break;
		// =======================================================================================================================
		//  
		// Structures(PlacedObjects) TURRET, INHIBITOR, NEXUS
		// 
		// =======================================================================================================================
		case UnitType::TURRET:
		{
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\TurretRubble.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();
			CGameObject* TurretBase = pObj->FindChildObjByName(L"TurretBase");
			CGameObject* TurretBreak1 = pObj->FindChildObjByName(L"TurretBreak_1");
			CGameObject* TurretBreak2 = pObj->FindChildObjByName(L"TurretBreak_2");
			Vec3 Scale = pObj->Transform()->GetRelativeScale();
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));


			if (_objectInfo.faction == Faction::RED)
			{				
				pObj->SetName(L"red_turret");
				// Rubble(����, �θ�)
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_red.mtrl"), 0);
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_red.mtrl"), 1);
				// TurretBase(��ü)
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth1_red.mtrl"), 0);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth2_red.mtrl"), 1);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_SRUAP_ChaosTurret1_Mat_red.mtrl"), 2);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage1_red.mtrl"), 3);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage2_red.mtrl"), 4);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Rubble_red.mtrl"), 5);
				// TurretBreak1(�ر� �ִϸ��̼�1)
				TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_red.mtrl"), 0);
				TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_red.mtrl"), 1);
				// TurretBreak2(�ر� �ִϸ��̼�2)
				TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_red.mtrl"), 0);
				TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_red.mtrl"), 1);

				
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_turret");
				// Rubble(����, �θ�)
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_blue.mtrl"), 0);
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_blue.mtrl"), 1);
				// TurretBase(��ü)
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth1_blue.mtrl"), 0);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth2_blue.mtrl"), 1);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_SRUAP_ChaosTurret1_Mat_blue.mtrl"), 2);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage1_blue.mtrl"), 3);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage2_blue.mtrl"), 4);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Rubble_blue.mtrl"), 5);
				// TurretBreak1(�ر� �ִϸ��̼�1)
				TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_blue.mtrl"), 0);
				TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_blue.mtrl"), 1);
				// TurretBreak2(�ر� �ִϸ��̼�2)
				TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_blue.mtrl"), 0);
				TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_blue.mtrl"), 1);
			}

			if (MyPlayer.host)
			{
				//pObj->AddComponent(new CTurretScript);
				// script->setLane, _objectInfo.lane
				pObj->AddComponent(new CUnitScript);  // ���� �ּ�ó��
				// ���ݹ��� �þ� �ڽĿ�����Ʈ�� �߰��ؾ��ҵ�.

			}
			else
			{
				pObj->AddComponent(new CUnitScript);
			}

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::TURRET);
			SpawnGameObject(pObj
				, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z)
				, L"Structure");
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			_placedObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;

		case UnitType::INHIBITOR:
		{
			pMeshData = nullptr;
			pObj = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
			pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_Inhibitor");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_Inhibitor");

			}
			//������ ���� �ִϸ��̼��� idle1 �ִϸ��̼���!! 
			//�������� ���ÿ��� 1���� alphaTex �̹����� �־��ְ�, 0�� ���͸��� ������ �⺻ ���͸����� �־��ָ��(�⺻������ �Ǿ��־ ���� �������� �ʿ�� ����)
			//�����Ⱑ �����Ҷ��� 0�����͸��� alphaTex �̹����� �־��ְ�, 1�����͸��� ������ ���� destroy�ؽ��ĸ� �����ָ�� (���� �����������)
			if (MyPlayer.host)
			{
				//pObj->AddComponent(new CInhibitorScript);
				// script->setLane, _objectInfo.lane
				pObj->AddComponent(new CUnitScript);  // ���� �ּ�ó��
				// ���ݹ��� �þ� �ڽĿ�����Ʈ�� �߰��ؾ��ҵ�.

			}
			else
			{
				pObj->AddComponent(new CUnitScript);
			}

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::INHIBITOR);
			SpawnGameObject(pObj
				, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z)
				, L"Structure");
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			_placedObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;

		case UnitType::NEXUS:
		{
			//-------------------------------�ؼ���-----------------------------------------
			//�ؼ����� 0�����͸����� ���� 1�� ���͸��󿡴� �����ؽ��ĸ� �����ϰ�, 1�����͸����� ���� 0�����͸��� ���� �ؽ��ĸ� ����������Ѵ�.
			//-----������ �ؼ����� ���� ------ 
			//pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\nexus_destroyed_red_clear.png"));
			//pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));

			//-----���� ���� �ؼ����� ����------ 
			/*pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
			pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\nexus_red_clear.png"));*/
			pMeshData = nullptr;
			pObj = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\nexus.fbx");
			pObj = pMeshData->Instantiate();
			pObj->SetName(L"blue_nexus");
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\nexus");
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->Animator3D()->PlayRepeat(L"nexus\\sruap_order_idle.anm_skinned_mesh.001", true, true, 0.1f);
			pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
			pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_ordernexus_tx_cm_clear.png"));
			pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
			
			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_nexus");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Mat_Red.mtrl"), 1);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_nexus");

			}


			if (MyPlayer.host)
			{
				//pObj->AddComponent(new CInhibitorScript);
				// script->setLane, _objectInfo.lane
				pObj->AddComponent(new CUnitScript);  // ���� �ּ�ó��
				// ���ݹ��� �þ� �ڽĿ�����Ʈ�� �߰��ؾ��ҵ�.

			}
			else
			{
				pObj->AddComponent(new CUnitScript);
			}

			// ����
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::NEXUS);
			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Structure");
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			_placedObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;


		} // End Swich case


	} // End mutex lock
}

void GameObjMgr::AddSkillProjectile(uint64 _projectileId, SkillInfo _skillInfo)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		// ���常 ��¥�� �����Ѵ�. �������� ��¥�� �����Ѵ�.
		if (MyPlayer.host)
		{
			// _SkillInfo�� �, � Skill���� ������ ��
			CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);
			CGameObject* TargetObj = FindAllObject(_skillInfo.TargetId);

			// Skill Projectile ������Ʈ ������ �ͼ�, �ش� ����ü ��ũ��Ʈ�� ���� ���̵� �ٿ���
			vector<CGameObject*> vecProj = skill->GetProjectile();

			for (int i = 0; i < vecProj.size(); i++)
			{
				vecProj[i]->AddComponent(CSkillMgr::GetInst()->FindProjectileScript(_skillInfo.skillType));
				vecProj[i]->GetScript<CProjectileScript>()->SetServerID(_projectileId + i);
				vecProj[i]->GetScript<CProjectileScript>()->SetServerUserID(_skillInfo.OwnerId);
				vecProj[i]->GetScript<CProjectileScript>()->SetServerTargetID(_skillInfo.TargetId);
				vecProj[i]->GetScript<CProjectileScript>()->SetUserObj(UserObj);
				vecProj[i]->GetScript<CProjectileScript>()->SetTargetObj(TargetObj);
				vecProj[i]->GetScript<CProjectileScript>()->SetDir(Vec3(_skillInfo.MouseDir.x, _skillInfo.MouseDir.y, _skillInfo.MouseDir.z));

				if (_skillInfo.UseMousePos)
					SpawnGameObject(vecProj[i], Vec3(_skillInfo.MousePos.x, _skillInfo.MousePos.y, _skillInfo.MousePos.z), L"SkillProjectile");
				else
				{
					Vec3 OwnerPos = UserObj->Transform()->GetRelativePos();
					SpawnGameObject(vecProj[i]
						, OwnerPos + Vec3(_skillInfo.offsetPos.x, _skillInfo.offsetPos.y, _skillInfo.offsetPos.z)
						, L"SkillProjectile");
				}
				_objects.insert(std::make_pair(_projectileId, vecProj[i]));
			}
		}
		else
		{
			// _SkillInfo�� �, � Skill���� ������ ��
			CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);

			// Skill Projectile ������Ʈ ������ �ͼ�, �� UnitScript ��ũ��Ʈ�� ���� ���̵� �ٿ���
			vector<CGameObject*> vecProj = skill->GetProjectile();

			for (int i = 0; i < vecProj.size(); i++)
			{
				vecProj[i]->AddComponent(new CUnitScript);
				vecProj[i]->GetScript<CUnitScript>()->SetServerID(_projectileId + i);

				if (_skillInfo.UseMousePos)
					SpawnGameObject(vecProj[i], Vec3(_skillInfo.MousePos.x, _skillInfo.MousePos.y, _skillInfo.MousePos.z), L"SkillProjectile");
				else
				{
					Vec3 OwnerPos = UserObj->Transform()->GetRelativePos();
					SpawnGameObject(vecProj[i]
						, OwnerPos + Vec3(_skillInfo.offsetPos.x, _skillInfo.offsetPos.y, _skillInfo.offsetPos.z)
						, L"SkillProjectile");
				}
				_objects.insert(std::make_pair(_projectileId, vecProj[i]));
			}
		}
	}
}

