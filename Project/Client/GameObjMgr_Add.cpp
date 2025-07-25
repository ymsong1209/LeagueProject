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
#include <Engine\CRenderMgr.h>


#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CEventMgr.h>

#include <Engine\CPathFindMgr.h>
#include <Engine\CAnim3D.h>
#include <Engine\CResMgr.h>

#include <Script\CUnitScript.h>
#include <Script\CChampionScript.h>
#include <Script\CJinxScript.h>
#include <Script/CMalphiteScript.h>
#include <Script\CJinxWScript.h>
#include <Script/CMalphiteEScript.h>
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
#include <Script\CMinionHPRatioScript.h>
#include <Script\CMinionHPBarPosScript.h>
#include <Script/CGrompScript.h>

#include <Script/CTurretScript.h>
#include <Script/CInhibitorScript.h>
#include <Script/CNexusScript.h>

#include <Script/CMurkWolfScript.h>
#include <Script/CMurkWolfMiniScript.h>
#include <Script/CRazorBeakScript.h>
#include <Script/CRazorBeakMiniScript.h>
#include <Script/CKrugScript.h>
#include <Script/CKrugMiniScript.h>
#include <Script/CBlueScript.h>
#include <Script/CRedScript.h>
#include <Script/CDragonScript.h>
#include <Script\CDragonHPUIScript.h>
#include <Script\CMinionHPRatioScript.h>
#include <Script\CJungleMINIHPScript.h>
#include <Script/CJungleMobHPScript.h>
#include <Script\CTurretHPUIScript.h>
#include <Script/CInGameCameraScript.h>

static bool MinionSpawn = true;
// ===============================================
//   Add
// ===============================================
void GameObjMgr::AddPlayer(PlayerInfo _info, bool myPlayer)
{
	// 추후 프리팹으로 변경되길 희망
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
			pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));

		}break;
		case ChampionType::MALPHITE:
		{
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Malphite.fbx");
			pObj = pMeshData->Instantiate();

			if (myPlayer)
				pObj->AddComponent(new CMalphiteScript);
			else
				pObj->AddComponent(new CUnitScript);


			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Malphite");
			pObj->Animator3D()->PlayRepeat(L"Malphite\\Idle1", true, true, 0.1f);
			pObj->Transform()->SetRelativeScale(Vec3(0.36f, 0.36f, 0.36f));

		}break;
		case ChampionType::AMUMU:
		{
		}break;
		}

		// 챔피언 타입과 관계없이 공통
		if (myPlayer)
		{
			CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
			MainCam->GetOwner()->GetScript<CInGameCameraScript>()->SetTargetObject(pObj);

			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CFsm);
			MyPlayerScript = pObj->GetScript<CUnitScript>();
			MyPlayerScript->SetServerID(_info.id);
			MyPlayerScript->SetNickname(_info.nickname);
			MyPlayerScript->SetHost(_info.host);
			MyPlayerScript->SetFaction(_info.faction);

			// Script 프로젝트에 본인 플레이어 게임 오브젝트 보관 (UI용)
			CSendServerEventMgr::GetInst()->SetMyPlayer(pObj);

			pObj->SetName(L"MyPlayer");

			// 사거리 자식 오브젝트 추가
			CGameObject* AttackRange = new CGameObject;
			AttackRange->AddComponent(new CTransform);
			AttackRange->AddComponent(new CCollider2D);
			AttackRange->Collider2D()->SetAbsolute(true);
			AttackRange->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			
			switch (_info.champion)
			{
			case ChampionType::JINX:
			{
				AttackRange->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));
			}break;
			case ChampionType::MALPHITE:
			{
				AttackRange->Collider2D()->SetOffsetScale(Vec2(60.f, 60.f));

			}break;
			}
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
			pObj->Transform()->SetUseMouseOutline(false);
		}
		else
		{
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetIsShootingRay(false);
		}

		pObj->AddComponent(new CCollider3D);
		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObj->Collider3D()->SetAbsolute(true);
		pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		pObj->Collider3D()->SetDrawCollision(false);

		pObj->AddComponent(new CCollider2D);
		pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
		pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0, 0));
		
		pObj->Transform()->SetRelativeRot(Vec3(_info.posInfo.moveDir.x, _info.posInfo.moveDir.y, _info.posInfo.moveDir.z));
		Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
		SpawnGameObject(pObj, spawnPos, L"Player");

		int mtrlcount = pObj->MeshRender()->GetMtrlCount();
		for (int i = 0; i < mtrlcount; ++i) {
			pObj->MeshRender()->GetDynamicMaterial(i);
		}
		
		pObj->GetRenderComponent()->SetDynamicShadow(true);
		
		_players.insert(std::make_pair(_info.id, pObj));
		_allObjects.insert(std::make_pair(_info.id, pObj));
	}
}

void GameObjMgr::AddObject(uint64 _objectId, ObjectInfo _objectInfo)
{
	// 추후 프리팹으로 변경되길 희망
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj;

		switch (_objectInfo.unitType)
		{

		case UnitType::MELEE_MINION:
		{
			if (MinionSpawn == false) break;
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

			// 방장은 진짜 계산 오브젝트 생성,  방장이 아닐 경우 허상을 생성
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

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::MELEE_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.16f, 0.16f, 0.16f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));


			if (_objectInfo.faction == MyPlayer.faction)
			{
				pObj->Transform()->SetIsShootingRay(true);
				pObj->Transform()->SetRayRange(150.f);
				pObj->Transform()->SetUseMouseOutline(false);
			}
			else
			{
				pObj->Transform()->SetUseMouseOutline(true);
				pObj->Transform()->SetIsShootingRay(false);
				pObj->Transform()->SetOutlineThickness(0.072f);
			}
			
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);

			CGameObject* MinionHP = new CGameObject;
			MinionHP->SetName(L"MinionHP");
			MinionHP->AddComponent(new CTransform);
			MinionHP->AddComponent(new CMeshRender);
			MinionHP->AddComponent(new CMinionHPRatioScript);
			pObj->AddChild(MinionHP);

			SpawnGameObject(pObj,Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId, pObj));

		}
		break;
		case UnitType::RANGED_MINION:
		{
			if (MinionSpawn == false) break;
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

			// 방장은 진짜 계산 오브젝트 생성,  방장이 아닐 경우 허상을 생성
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
			

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::RANGED_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.16f, 0.16f, 0.16f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));

			if (_objectInfo.faction == MyPlayer.faction)
			{
				pObj->Transform()->SetIsShootingRay(true);
				pObj->Transform()->SetRayRange(150.f);
				pObj->Transform()->SetUseMouseOutline(false);
			}
			else
			{
				pObj->Transform()->SetUseMouseOutline(true);
				pObj->Transform()->SetIsShootingRay(false);
				pObj->Transform()->SetOutlineThickness(0.072f);
			}

			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);

			CGameObject* HPBar = new CGameObject;
			HPBar->SetName(L"HPBar");
			HPBar->AddComponent(new CTransform);
			HPBar->AddComponent(new CMeshRender);
			HPBar->AddComponent(new CMinionHPRatioScript);
			pObj->AddChild(HPBar);

			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SIEGE_MINION:
		{
			if (MinionSpawn == false) break;
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SiegeMinion.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_minion_siege");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Minion_Siege_Red.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_minion_siege");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Minion_Siege_Blue.mtrl"), 0);
			}

			// 방장은 진짜 계산 오브젝트 생성,  방장이 아닐 경우 허상을 생성
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

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::SIEGE_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.16f, 0.16f, 0.16f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));

			if (_objectInfo.faction == MyPlayer.faction)
			{
				pObj->Transform()->SetIsShootingRay(true);
				pObj->Transform()->SetRayRange(150.f);
				pObj->Transform()->SetUseMouseOutline(false);
			}
			else
			{
				pObj->Transform()->SetUseMouseOutline(true);
				pObj->Transform()->SetIsShootingRay(false);
				pObj->Transform()->SetOutlineThickness(0.072f);
			}

			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
		

			CGameObject* HPBar = new CGameObject;
			HPBar->SetName(L"HPBar");
			HPBar->AddComponent(new CTransform);
			HPBar->AddComponent(new CMeshRender);
			HPBar->AddComponent(new CMinionHPRatioScript);
			pObj->AddChild(HPBar);

			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SUPER_MINION:
		{
			if (MinionSpawn == false) break;
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

			// 방장은 진짜 계산 오브젝트 생성,  방장이 아닐 경우 허상을 생성
				// 방장은 진짜 계산 오브젝트 생성,  방장이 아닐 경우 허상을 생성
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

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::SUPER_MINION);

			pObj->Transform()->SetRelativeScale(Vec3(0.25f, 0.25f, 0.25f));
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			

			if (_objectInfo.faction == MyPlayer.faction)
			{
				pObj->Transform()->SetIsShootingRay(true);
				pObj->Transform()->SetRayRange(150.f);
				pObj->Transform()->SetUseMouseOutline(false);
			}
			else
			{
				pObj->Transform()->SetUseMouseOutline(true);
				pObj->Transform()->SetIsShootingRay(false);
				pObj->Transform()->SetOutlineThickness(0.072f);
			}

			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);

			CGameObject* HPBar = new CGameObject;
			HPBar->SetName(L"HPBar");
			HPBar->AddComponent(new CTransform);
			HPBar->AddComponent(new CMeshRender);
			HPBar->AddComponent(new CMinionHPRatioScript);
			pObj->AddChild(HPBar);

			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		// =======================================================================================================================
		//  
		// Jungle Mob(Objects) 
		// 
		// =======================================================================================================================
		case UnitType::SOUTH_GROMP://블루팀 두꺼비
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\gromp.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\gromp");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CGrompScript);

				CGrompScript* Script = pObj->GetScript<CGrompScript>();

				Script->SetAggroPos(Vec3(323.f, 0.f, 1242.f));
				SpawnGameObject(pObj, Vec3(323.f, 10.f, 1242.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(323.f, 10.f, 1242.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_GROMP);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			CGameObject* GrompHP = new CGameObject;
			GrompHP->SetName(L"SOUTH_GrompHP");
			GrompHP->AddComponent(new CTransform);
			GrompHP->AddComponent(new CMeshRender);
			GrompHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(GrompHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_MURKWOLF://블루팀 늑대
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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

			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CMurkWolfScript);

				CMurkWolfScript* script = pObj->GetScript <CMurkWolfScript>();

				script->SetAggroPos(Vec3(564.f, 0.f, 959.f));
				SpawnGameObject(pObj, Vec3(550.f, 15.f, 944.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(550.f, 15.f, 944.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* MurkWolfHP = new CGameObject;
			MurkWolfHP->SetName(L"SOUTH_MurkWolfHP");
			MurkWolfHP->AddComponent(new CTransform);
			MurkWolfHP->AddComponent(new CMeshRender);
			MurkWolfHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(MurkWolfHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_MURKWOLF_MINI_L://블루팀 늑대 째깐이(좌)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(564.f, 0.f, 964.f));
				SpawnGameObject(pObj, Vec3(552.f, 15.f, 964.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(552.f, 15.f, 964.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF_MINI_L);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"SOUTH_MurkWolf_Mini_L_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(MurkWolf_MiniHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_MURKWOLF_MINI_R://블루팀 늑대 째깐이(우)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(564.f, 0.f, 964.f));
				SpawnGameObject(pObj, Vec3(580.f, 15.f, 944.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(580.f, 15.f, 944.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF_MINI_R);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"SOUTH_MurkWolf_Mini_R_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(MurkWolf_MiniHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_KRUG://블루팀 돌거북
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CKrugScript);

				CKrugScript* Script = pObj->GetScript<CKrugScript>();

				Script->SetAggroPos(Vec3(1238.f, 0.f, 389.f));
				SpawnGameObject(pObj, Vec3(1221.f, 15.f, 379.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1221.f, 15.f, 379.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_KRUG);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* KrugHP = new CGameObject;
			KrugHP->SetName(L"SOUTH_KrugHP");
			KrugHP->AddComponent(new CTransform);
			KrugHP->AddComponent(new CMeshRender);
			KrugHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(KrugHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_KRUG_MINI://블루팀 돌거북 째깐이
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CKrugMiniScript);

				CKrugMiniScript* Script = pObj->GetScript<CKrugMiniScript>();

				Script->SetAggroPos(Vec3(1238.f, 0.f, 389.f));
				SpawnGameObject(pObj, Vec3(1252.f, 15.f, 379.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1252.f, 15.f, 379.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_KRUG_MINI);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* Krug_MiniHP = new CGameObject;
			Krug_MiniHP->SetName(L"SOUTH_Krug_Mini_HP");
			Krug_MiniHP->AddComponent(new CTransform);
			Krug_MiniHP->AddComponent(new CMeshRender);
			Krug_MiniHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(Krug_MiniHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RAZORBEAK://블루팀 칼날부리
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakScript);

				CRazorBeakScript* Script = pObj->GetScript<CRazorBeakScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1008.f, 15.f, 800.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1008.f, 15.f, 800.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeakHP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_1://블루팀 칼날부리 째깐이 1
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1026.f, 15.f, 810.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1026.f, 15.f, 810.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_1);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini1_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_2://블루팀 칼날부리 째깐이 2
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 799.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 799.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_2);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini2_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_3://블루팀 칼날부리 째깐이 3
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1031.f, 15.f, 782.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1031.f, 15.f, 782.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_3);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini3_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_4://블루팀 칼날부리 째깐이 4
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1012.f, 15.f, 765.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1012.f, 15.f, 765.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_4);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini4_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RAZORBEAK_MINI_5://블루팀 칼날부리 째깐이 5
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 767.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1048.f, 15.f, 767.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_5);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini5_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_BLUE://블루팀 블루
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_blue");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CBlueScript);

				CBlueScript* Script = pObj->GetScript<CBlueScript>();

				Script->SetAggroPos(Vec3(559.f, 0.f, 1164.f));
				SpawnGameObject(pObj, Vec3(559.f, 15.f, 1164.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(559.f, 15.f, 1164.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_BLUE);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* BlueHP = new CGameObject;
			BlueHP->SetName(L"SOUTH_BlueHP");
			BlueHP->AddComponent(new CTransform);
			BlueHP->AddComponent(new CMeshRender);
			BlueHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(BlueHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::SOUTH_RED://블루팀 레드
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_red.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_red");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRedScript);

				CRedScript* Script = pObj->GetScript<CRedScript>();

				Script->SetAggroPos(Vec3(1123, 15.f, 559.f));
				SpawnGameObject(pObj, Vec3(1136, 15.f, 580.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1136, 15.f, 580.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::SOUTH_RED);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RedHP = new CGameObject;
			RedHP->SetName(L"SOUTH_RedHP");
			RedHP->AddComponent(new CTransform);
			RedHP->AddComponent(new CMeshRender);
			RedHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(RedHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_GROMP://레드팀 두꺼비
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\gromp.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\gromp");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CGrompScript);

				CGrompScript* Script = pObj->GetScript<CGrompScript>();

				Script->SetAggroPos(Vec3(1823.f, 0.f, 943.f));
				SpawnGameObject(pObj, Vec3(1859.f, 10.f, 956.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1859.f, 10.f, 956.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_GROMP);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* GrompHP = new CGameObject;
			GrompHP->SetName(L"NORTH_GrompHP");
			GrompHP->AddComponent(new CTransform);
			GrompHP->AddComponent(new CMeshRender);
			GrompHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(GrompHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_MURKWOLF://레드팀 늑대
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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

			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CMurkWolfScript);

				CMurkWolfScript* script = pObj->GetScript <CMurkWolfScript>();

				script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
				SpawnGameObject(pObj, Vec3(1619.f, 15.f, 1236.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1619.f, 15.f, 1236.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_MURKWOLF);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* MurkWolfHP = new CGameObject;
			MurkWolfHP->SetName(L"NORTH_MurkWolfHP");
			MurkWolfHP->AddComponent(new CTransform);
			MurkWolfHP->AddComponent(new CMeshRender);
			MurkWolfHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(MurkWolfHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_MURKWOLF_MINI_L://레드팀 늑대 째깐이(좌)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
				SpawnGameObject(pObj, Vec3(1592.f, 19.f, 1232.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1592.f, 19.f, 1232.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_MURKWOLF_MINI_L);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"NORTH_MurkWolf_Mini_L_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(MurkWolf_MiniHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_MURKWOLF_MINI_R://레드팀 늑대 째깐이(우)
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CMurkWolfMiniScript);

				CMurkWolfMiniScript* Script = pObj->GetScript<CMurkWolfMiniScript>();

				Script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
				SpawnGameObject(pObj, Vec3(1634, 15.f, 1208.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1634, 15.f, 1208.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_MURKWOLF_MINI_R);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"NORTH_MurkWolf_Mini_R_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(MurkWolf_MiniHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_KRUG://레드팀 돌거북
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CKrugScript);

				CKrugScript* Script = pObj->GetScript<CKrugScript>();

				Script->SetAggroPos(Vec3(942.f, 0.f, 1845.f));
				SpawnGameObject(pObj, Vec3(966, 15.f, 1800.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(966, 15.f, 1800.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_KRUG);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* KrugHP = new CGameObject;
			KrugHP->SetName(L"NORTH_KrugHP");
			KrugHP->AddComponent(new CTransform);
			KrugHP->AddComponent(new CMeshRender);
			KrugHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(KrugHP);


			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_KRUG_MINI://레드팀 돌거북 째깐이
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CKrugMiniScript);

				CKrugMiniScript* Script = pObj->GetScript<CKrugMiniScript>();

				Script->SetAggroPos(Vec3(942.f, 0.f, 1845.f));
				SpawnGameObject(pObj, Vec3(935.f, 15.f, 1791.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(935.f, 15.f, 1791.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_KRUG_MINI);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* Krug_MiniHP = new CGameObject;
			Krug_MiniHP->SetName(L"NORTH_Krug_Mini_HP");
			Krug_MiniHP->AddComponent(new CTransform);
			Krug_MiniHP->AddComponent(new CMeshRender);
			Krug_MiniHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(Krug_MiniHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RAZORBEAK://레드팀 칼날부리
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakScript);

				CRazorBeakScript* Script = pObj->GetScript<CRazorBeakScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1170.f, 15.f, 1390.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1170.f, 15.f, 1390.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeakHP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_1://레드팀 칼날부리 째깐이 1
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1139, 15.f, 1419.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1139, 15.f, 1419.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_1);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini1_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_2://레드팀 칼날부리 째깐이 2
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1176, 15.f, 1419.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1176, 15.f, 1419.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_2);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini2_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_3://레드팀 칼날부리 째깐이 3
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1159, 15.f, 1405.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1031.f, 15.f, 782.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_3);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini3_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_4://레드팀 칼날부리 째깐이 4
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1140, 15.f, 1385.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1140, 15.f, 1385.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_4);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini4_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RAZORBEAK_MINI_5://블루팀 칼날부리 째깐이 5
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRazorBeakMiniScript);

				CRazorBeakMiniScript* Script = pObj->GetScript<CRazorBeakMiniScript>();

				Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
				SpawnGameObject(pObj, Vec3(1157, 15.f, 1370.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1157, 15.f, 1370.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_5);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini5_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			pObj->AddChild(RazorBeakHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_BLUE://레드팀 블루
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_blue");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CBlueScript);

				CBlueScript* Script = pObj->GetScript<CBlueScript>();

				Script->SetAggroPos(Vec3(1653.f, 0.f, 1011.f));
				SpawnGameObject(pObj, Vec3(1636, 10.f, 1009.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1636, 10.f, 1009.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_BLUE);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* BlueHP = new CGameObject;
			BlueHP->SetName(L"NORTH_BlueHP");
			BlueHP->AddComponent(new CTransform);
			BlueHP->AddComponent(new CMeshRender);
			BlueHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(BlueHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::NORTH_RED://레드팀 레드
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_red.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_red");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
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
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CRedScript);

				CRedScript* Script = pObj->GetScript<CRedScript>();

				Script->SetAggroPos(Vec3(1055, 15.f, 1637.f));
				SpawnGameObject(pObj, Vec3(1049, 15.f, 1607.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1049, 15.f, 1607.f), L"Mob");
			}

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::NORTH_RED);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);

			CGameObject* RedHP = new CGameObject;
			RedHP->SetName(L"NORTH_RedHP");
			RedHP->AddComponent(new CTransform);
			RedHP->AddComponent(new CMeshRender);
			RedHP->AddComponent(new CJungleMobHPScript);
			pObj->AddChild(RedHP);

			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;
		case UnitType::DRAGON:// 드래곤
		{
			pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Elder_Dragon.fbx");
			pObj = nullptr;
			pObj = pMeshData->Instantiate();
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Elder_Dragon");  // 내부말고 여기서 해줘야 빈껍데기 두꺼비도 애니메이션을 안다.
			pObj->SetName(L"Elder_Dragon");
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetRelativeScale(0.33f, 0.33f, 0.33f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-44.f), XMConvertToRadians(-180.f)));
			pObj->AddComponent(new CCollider3D);
			pObj->Collider3D()->SetAbsolute(true);
			pObj->Collider3D()->SetDrawCollision(false);
			pObj->Collider3D()->SetOffsetScale(Vec3(125.f, 125.f, 125.f));
			
			pObj->AddComponent(new CCollider2D);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			pObj->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));
			if (MyPlayer.host) // 방장인 경우에 진짜
			{
				pObj->AddComponent(new CDragonScript);
			
				CDragonScript* Script = pObj->GetScript<CDragonScript>();
			
				Script->SetAggroPos(Vec3(1451, 15.f, 649.f));
				Script->SetAlertPos(Vec3(1483, 15.f, 689.f));
				SpawnGameObject(pObj, Vec3(1451, 0.f, 656.f), L"Mob");
			}
			else // 방장이 아닌 경우에는 가짜를 생성(빈껍데기)
			{
				pObj->AddComponent(new CUnitScript);
				CUnitScript* Script = pObj->GetScript<CUnitScript>();
				SpawnGameObject(pObj, Vec3(1451, 0.f, 656.f), L"Mob");
			}
			
			CGameObject* DragonBar = new CGameObject;
			DragonBar->SetName(L"DragonBar");
			DragonBar->AddComponent(new CTransform);
			DragonBar->AddComponent(new CMeshRender);
			DragonBar->AddComponent(new CDragonHPUIScript);
			pObj->AddChild(DragonBar);

			// UnitScript 에 진짜도, 가짜도 공통적으로 들어가야 하는 값들.
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);  // 서버 id
			Script->SetUnitType(UnitType::DRAGON);  // UnitType
			Script->SetFaction(_objectInfo.faction);    // 진영 정글몹은: NONE
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetRaySightCulling(true);
			pObj->Transform()->SetIsShootingRay(false);
			pObj->Transform()->SetRayRange(0.f);
			_objects.insert(std::make_pair(_objectId, pObj));   // 서버가 관리하도록 꼭 넣어야함!! make_pair(서버id, GameObject*)
			_allObjects.insert(std::make_pair(_objectId, pObj));
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
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			CGameObject* TurretBase = pObj->FindChildObjByName(L"TurretBase");
			TurretBase->Transform()->SetUseMouseOutline(true);
			TurretBase->Transform()->SetOutlineThickness(0.072f);
			TurretBase->Collider3D()->SetOffsetScale(Vec3(320.f, 320.f, 320.f));
			CGameObject* TurretBreak1 = pObj->FindChildObjByName(L"TurretBreak_1");
			CGameObject* TurretBreak2 = pObj->FindChildObjByName(L"TurretBreak_2");
			
			TurretBreak1->Animator3D()->GetCurAnim()->Pause();
			TurretBreak2->Animator3D()->GetCurAnim()->Pause();

			// 포탑 자식 오브젝트에 UnitScript 붙여주기
			TurretBase->AddComponent(new CUnitScript);
			TurretBreak1->AddComponent(new CUnitScript);
			TurretBreak2->AddComponent(new CUnitScript);

			// 잔해, Break1, Break2에 Alpha Mtrl 설정
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_alpha.mtrl"), 0);
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_alpha.mtrl"), 1);

			TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_alpha.mtrl"), 0);
			TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_alpha.mtrl"), 1);
			TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_alpha.mtrl"), 0);
			TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_alpha.mtrl"), 1);

			// 포탑에게 
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));


			if (_objectInfo.faction == Faction::RED)
			{				
				pObj->SetName(L"red_turret");
				
				// TurretBase(본체)
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth1_red.mtrl"), 0);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth2_red.mtrl"), 1);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_SRUAP_ChaosTurret1_Mat_red.mtrl"), 2);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage1_red.mtrl"), 3);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage2_red.mtrl"), 4);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Rubble_red.mtrl"), 5);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_turret");

				// TurretBase(본체)
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth1_blue.mtrl"), 0);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Cloth2_blue.mtrl"), 1);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_SRUAP_ChaosTurret1_Mat_blue.mtrl"), 2);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage1_blue.mtrl"), 3);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Stage2_blue.mtrl"), 4);
				TurretBase->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_idlebreak_Rubble_blue.mtrl"), 5);
				
			}

			if (MyPlayer.host)
			{
				// 방장일 경우 TurretScript
				pObj->AddComponent(new CTurretScript);
			}
			else
			{
				// 클라이언트는 UnitScript
				pObj->AddComponent(new CUnitScript);
			}

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);

			// 자식에게 서버 아이디 부여
			TurretBase->GetScript<CUnitScript>()->SetServerID(_objectId+1);
			TurretBreak1->GetScript<CUnitScript>()->SetServerID(_objectId+2);
			TurretBreak2->GetScript<CUnitScript>()->SetServerID(_objectId+3);

			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::TURRET);
			Script->SetLane(_objectInfo.lane);
			SpawnGameObject(pObj
				, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z)
				, L"Structure");
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			if (Script->GetFaction() == MyPlayer.faction) {
				pObj->Transform()->SetIsShootingRay(true);
				pObj->Transform()->SetRayRange(200.f);
			}
			else {
				pObj->Transform()->SetIsShootingRay(false);
			}

			CGameObject* HPBar = new CGameObject;
			HPBar->SetName(L"TurretBar");
			HPBar->AddComponent(new CTransform);
			HPBar->AddComponent(new CMeshRender);
			HPBar->AddComponent(new CTurretHPUIScript);
			pObj->AddChild(HPBar);

			_placedObjects.insert(std::make_pair(_objectId, pObj));
			_turretChild.insert(std::make_pair(_objectId + 1, TurretBase));
			_turretChild.insert(std::make_pair(_objectId + 2, TurretBreak1));
			_turretChild.insert(std::make_pair(_objectId + 3, TurretBreak2));

			_allObjects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId + 1, TurretBase));
			_allObjects.insert(std::make_pair(_objectId + 2, TurretBreak1));
			_allObjects.insert(std::make_pair(_objectId + 3, TurretBreak2));
		}
		break;

		case UnitType::INHIBITOR:
		{
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Inhibitor.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);
			pObj->Collider2D()->SetAbsolute(true);
			pObj->Collider3D()->SetAbsolute(true);

			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_Inhibitor");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_Inhibitor");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Mtrl.mtrl"), 0);

			}
			//억제기 평상시 애니메이션은 idle1 애니메이션임!! 
			//억제기의 평상시에는 1번에 alphaTex 이미지를 넣어주고, 0번 머터리얼에 억제기 기본 머터리얼을 넣어주면됨(기본적으로 되어있어서 따로 세팅해줄 필요는 없음)
			//억제기가 폭발할때는 0번머터리얼에 alphaTex 이미지를 넣어주고, 1번머터리얼에 억제기 전용 destroy텍스쳐를 입혀주면됨 (따로 세팅해줘야함)
			if (MyPlayer.host)
			{
				pObj->AddComponent(new CInhibitorScript);  // 추후 주석처리
				// 공격범위 시야 자식오브젝트도 추가해야할듯.
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
			}

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetUnitDead(true);
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetLane(_objectInfo.lane);
			Script->SetUnitType(UnitType::INHIBITOR);
			SpawnGameObject(pObj
				, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z)
				, L"Structure");
	

			CGameObject* HPBar = new CGameObject;
			HPBar->SetName(L"TurretBar");
			HPBar->AddComponent(new CTransform);
			HPBar->AddComponent(new CMeshRender);
			HPBar->AddComponent(new CTurretHPUIScript);
			pObj->AddChild(HPBar);

			_placedObjects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId, pObj));
		}
		break;

		case UnitType::NEXUS:
		{
			//-------------------------------넥서스-----------------------------------------
			//넥서스는 0번머터리얼을 쓰면 1번 머터리얼에는 알파텍스쳐를 장착하고, 1번머터리얼을 쓰면 0번머터리얼에 알파 텍스쳐를 장착해줘야한다.
			//-----터지는 넥서스쪽 보기 ------ 
			//pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\nexus_destroyed_red_clear.png"));
			//pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));

			//-----빙빙 도는 넥서스쪽 보기------ 
			/*pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
			pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\nexus_red_clear.png"));*/
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Nexus.prefab");
			CPrefab* pPrefab = (CPrefab*)Prefab.Get();
			pObj = pPrefab->Instantiate();
			pObj->Transform()->SetUseMouseOutline(true);
			pObj->Transform()->SetOutlineThickness(0.072f);
			pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(_objectInfo.objectMove.moveDir.x), XMConvertToRadians(_objectInfo.objectMove.moveDir.y), XMConvertToRadians(_objectInfo.objectMove.moveDir.z)));
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_alpha.mtrl"), 0);
			
			if (_objectInfo.faction == Faction::RED)
			{
				pObj->SetName(L"red_nexus");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Mat_Red.mtrl"), 1);
			}
			else if (_objectInfo.faction == Faction::BLUE)
			{
				pObj->SetName(L"blue_nexus");
				pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Mat_Blue.mtrl"), 1);
			}


			if (MyPlayer.host)
			{
				pObj->AddComponent(new CNexusScript);
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
			}

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);
			Script->SetUnitType(UnitType::NEXUS);
			SpawnGameObject(pObj, Vec3(_objectInfo.objectMove.pos.x, _objectInfo.objectMove.pos.y, _objectInfo.objectMove.pos.z), L"Structure");
			pObj->GetRenderComponent()->SetFrustumCheck(true);

			CGameObject* HPBar = new CGameObject;
			HPBar->SetName(L"TurretBar");
			HPBar->AddComponent(new CTransform);
			HPBar->AddComponent(new CMeshRender);
			HPBar->AddComponent(new CTurretHPUIScript);
			pObj->AddChild(HPBar);

			_placedObjects.insert(std::make_pair(_objectId, pObj));
			_allObjects.insert(std::make_pair(_objectId, pObj));
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

		// 방장만 진짜를 생성한다. 나머지는 가짜를 생성한다.
		if (MyPlayer.host)
		{
			// _SkillInfo를 까서, 어떤 Skill인지 가지고 옴		
			CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);
			CGameObject* TargetObj = FindAllObject(_skillInfo.TargetId);

			// Skill Projectile 오브젝트 가지고 와서, 해당 투사체 스크립트와 서버 아이디 붙여줌
			vector<CGameObject*> vecProj = skill->GetProjectile();

			CUnitScript* MinionScript = UserObj->GetScript<CUnitScript>();

			// 원거리 평타 쓰는 미니언만 예외처리
			if (MinionScript->GetUnitType() == UnitType::RANGED_MINION || MinionScript->GetUnitType() == UnitType::SIEGE_MINION)
			{
				if (MinionScript->GetFaction() == Faction::RED)
				{
					//vecProj[0]->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionBlueAttack.dds"));
					CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RedMinionBasicAttack.prefab")->Instantiate();
					RangedMinionBasicAttackObj->SetName(L"RedRangedMinionBasicAttackProjectile");
					RangedMinionBasicAttackObj->AddComponent(new CCollider2D);
					RangedMinionBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
					RangedMinionBasicAttackObj->Collider2D()->SetAbsolute(true);
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
					RangedMinionBasicAttackObj->Collider2D()->SetDrawCollision(true);
					RangedMinionBasicAttackObj->Transform()->SetBillBoard(true);
					RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionBlueAttack.dds"));
					vecProj[0] = RangedMinionBasicAttackObj;
				}
				else if (MinionScript->GetFaction() == Faction::BLUE)
				{
					//vecProj[0]->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionRedAttack.dds"));
					CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BlueMinionBasicAttack.prefab")->Instantiate();
					RangedMinionBasicAttackObj->SetName(L"BlueRangedMinionBasicAttackProjectile");
					RangedMinionBasicAttackObj->AddComponent(new CCollider2D);
					RangedMinionBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
					RangedMinionBasicAttackObj->Collider2D()->SetAbsolute(true);
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
					RangedMinionBasicAttackObj->Collider2D()->SetDrawCollision(true);
					RangedMinionBasicAttackObj->Transform()->SetBillBoard(true);
					RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionRedAttack.dds"));

					vecProj[0] = RangedMinionBasicAttackObj;
				}
			}

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

				vecProj[i]->GetScript<CUnitScript>()->SetUnitType(UnitType::PROJECTILE);
				_objects.insert(std::make_pair(_projectileId + i, vecProj[i]));
				_allObjects.insert(std::make_pair(_projectileId + i, vecProj[i]));
			}
		}
		else
		{
			// _SkillInfo를 까서, 어떤 Skill인지 가지고 옴
			CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);

			// Skill Projectile 오브젝트 가지고 와서, 빈 UnitScript 스크립트와 서버 아이디 붙여줌
			vector<CGameObject*> vecProj = skill->GetProjectile();

			CUnitScript* MinionScript = UserObj->GetScript<CUnitScript>();

			// 원거리 평타 쓰는 미니언만 예외처리
			if (MinionScript->GetUnitType() == UnitType::RANGED_MINION || MinionScript->GetUnitType() == UnitType::SIEGE_MINION)
			{
				if (MinionScript->GetFaction() == Faction::RED)
				{
					//vecProj[0]->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionBlueAttack.dds"));
					CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RedMinionBasicAttack.prefab")->Instantiate();
					RangedMinionBasicAttackObj->SetName(L"RedRangedMinionBasicAttackProjectile");
					RangedMinionBasicAttackObj->AddComponent(new CCollider2D);
					RangedMinionBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
					RangedMinionBasicAttackObj->Collider2D()->SetAbsolute(true);
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
					RangedMinionBasicAttackObj->Collider2D()->SetDrawCollision(true);
					RangedMinionBasicAttackObj->Transform()->SetBillBoard(true);
					RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionBlueAttack.dds"));
					vecProj[0] = RangedMinionBasicAttackObj;
				}
				else if(MinionScript->GetFaction() == Faction::BLUE)
				{
					//vecProj[0]->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionRedAttack.dds"));
					CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BlueMinionBasicAttack.prefab")->Instantiate();
					RangedMinionBasicAttackObj->SetName(L"BlueRangedMinionBasicAttackProjectile");
					RangedMinionBasicAttackObj->AddComponent(new CCollider2D);
					RangedMinionBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
					RangedMinionBasicAttackObj->Collider2D()->SetAbsolute(true);
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
					RangedMinionBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
					RangedMinionBasicAttackObj->Collider2D()->SetDrawCollision(true);
					RangedMinionBasicAttackObj->Transform()->SetBillBoard(true);
					RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionRedAttack.dds"));

					vecProj[0] = RangedMinionBasicAttackObj;
				}
			}

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

				vecProj[i]->GetScript<CUnitScript>()->SetUnitType(UnitType::PROJECTILE);
				_objects.insert(std::make_pair(_projectileId+i, vecProj[i]));
				_allObjects.insert(std::make_pair(_projectileId + i, vecProj[i]));
			}
		}

		// 스킬 이펙트 생성
		CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);

		// 스킬 쓰는 애
		CGameObject* skillTargetObj = UserObj;

		// _SkillInfo를 까서, 어떤 Skill인지 가지고 옴
		CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);

		// 시전자 몸 주변에 이펙트를 생성
		if (skill->GetSkillEffect() != nullptr)
		{
			SpawnGameObject(skill->GetSkillEffect(), UserObj->Transform()->GetRelativePos(), L"Effect");

			CUnitScript* MinionScript = UserObj->GetScript<CUnitScript>();

			// 미니언만 예외처리
			if ((UINT)UnitType::MELEE_MINION <= (UINT)MinionScript->GetUnitType() &&
				(UINT)MinionScript->GetUnitType() <= (UINT)UnitType::SUPER_MINION)
			{
				if (MinionScript->GetFaction() == Faction::BLUE)
				{
					skill->GetSkillEffect()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionHitEffect.mtrl"), 0);
				}
			}

			skill->GetSkillHitEffect()->SetLifeSpan(0.3f);
		}
	}
}



