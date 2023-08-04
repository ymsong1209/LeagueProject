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
			pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

		}break;

		case ChampionType::AMUMU:
		{
		}break;
		}

		if (myPlayer)
		{
			pObj->AddComponent(new CPathFinder);
			pObj->AddComponent(new CFsm);
			MyPlayerScript = pObj->GetScript<CChampionScript>();
			MyPlayerScript->SetServerID(_info.id);
			MyPlayerScript->SetNickname(_info.nickname);
			MyPlayerScript->SetHost(_info.host);
			MyPlayerScript->SetFaction(_info.faction);

			pObj->SetName(L"MyPlayer");


			// 사거리 자식 오브젝트 추가
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

		Vec3 spawnPos = Vec3(_info.posInfo.pos.x, _info.posInfo.pos.y, _info.posInfo.pos.z);
		SpawnGameObject(pObj, spawnPos, L"Player");


		_players.insert(std::make_pair(_info.id, pObj));
	}
}

void GameObjMgr::AddObject(uint64 _objectId, ObjectInfo _objectInfo)
{
	// 추후 프리팹으로 변경되길 희망
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = new CGameObject;

		switch (_objectInfo.unitType)
		{
		case UnitType::MELEE_MINION:
		{
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\minion_melee.fbx");
			pObj = pMeshData->Instantiate();

			pObj->SetName(L"minion_melee");
			pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\minion_melee");
			pObj->Animator3D()->PlayRepeat(L"minion_melee\\Idle1", true, true, 0.1f);

			//pObj->AddComponent(new CCollider3D);
			//pObj->AddComponent(new CCollider2D);

			//pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			//pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
			//pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

			//pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			//pObj->Collider3D()->SetAbsolute(true);
			//pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
			//pObj->Collider3D()->SetDrawCollision(false);

			// 방장은 진짜 계산 오브젝트 생성,  방장이 아닐 경우 허상을 생성
			if (MyPlayer.host)
			{
				pObj->AddComponent(new CMinionScript);

				if (_objectInfo.lane == Lane::TOP)
				{
					// 미니언 라인 Top 설정
				}
				else if (_objectInfo.lane == Lane::MID)
				{
					// 미니언 라인 Mid 설정
				}
				else if (_objectInfo.lane == Lane::BOTTOM)
				{
					// 미니언 라인 Bottom 설정
				}
			}
			else
			{
				pObj->AddComponent(new CUnitScript);
			}

			// 공통
			CUnitScript* Script = pObj->GetScript<CUnitScript>();
			Script->SetServerID(_objectId);
			Script->SetFaction(_objectInfo.faction);


			// 추후 추가 unitscript에 변수 채우기 + spawnPos도 서버가 줄 예정
			//Script->SetCurHP
			//script->SetCurMP
			pObj->Transform()->SetRelativeScale(Vec3(0.1f, 0.1f, 0.1f));
			Vec3 spawnPos = Vec3(100.f + (50 * _objects.size()), 30.f, 100.f);
			SpawnGameObject(pObj, spawnPos, L"Mob");

			_objects.insert(std::make_pair(_objectId, pObj));

		}
		break;

		case UnitType::RANGED_MINION:
		{
		}
		break;


		}


	}
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
			// _SkillInfo를 까서, 어떤 Skill인지 가지고 옴
			CSkill* skill = CSkillMgr::GetInst()->FindSkill(_skillInfo.skillType);

			CGameObject* UserObj = FindAllObject(_skillInfo.OwnerId);

			// Skill Projectile 오브젝트 가지고 와서, 빈 UnitScript 스크립트와 서버 아이디 붙여줌
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

