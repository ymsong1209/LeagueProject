#include "pch.h"
#include "ServerEventMgr.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "GameObjMgr.h"
#include <Engine/components.h>
#include <Script/CSendServerEventMgr.h>
#include <Script/CChampionScript.h>
#include <Script/CGameEvent.h>
#include <Script/CGameEventMgr.h>


ServerEventMgr::ServerEventMgr()
{
	last_tick_time = std::chrono::steady_clock::now();
}

ServerEventMgr::~ServerEventMgr()
{
}

void ServerEventMgr::sendtick(ClientServiceRef _service)
{
    auto now = std::chrono::steady_clock::now();
    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick_time);

	// ========================================
	// 규칙 패킷(MovePacket) : 100ms = 1/10 sec
    // ========================================
	if (time_diff.count() >= 100)  // 나중에 40으로 고침
    {     
        // 1. 본인 플레이어 move 패킷을 서버에 보낸다. (ObjectMove)
		// 문제점 : 안움직일땐 move 패킷을 안보내서 LV,HP,MP 등 업데이트가 안됌...그냥 움직여라는 뜻.(나중에는 안움직일때도 패킷보내게 변경예정)
        GameObjMgr::GetInst()->SendMyPlayerMove(_service);

		// 2. 방장일시, 모든 오브젝트 move패킷을 서버에 보낸다.(포탑,억제기,넥서스 제외)
		if (MyPlayer.host)
		{
			// 일단은 pos다르면 업데이트 안하지만 추후 pos가 같아도 계속 보내도록 변경하기
			map<uint64, CGameObject*> _objects = GameObjMgr::GetInst()->GetObjects();
			for (auto& pair : _objects) {
				uint64 id = pair.first; 
				CGameObject* obj = pair.second; 
				if (obj->IsDead()) // 이미 죽어서 사라진 오브젝트
					continue;
				GameObjMgr::GetInst()->SendObjectMove(id, obj, _service);
			}

			// 배치형 오브젝트는 움직임이 없으니 HP에 변동이 생길때만 보낸다.(SendTowersUpdate)
			map<uint64, CGameObject*> _placedObject = GameObjMgr::GetInst()->GetPlacedObjects();
			for (auto& pair : _placedObject) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
				if (obj->IsDead()) // 이미 죽어서 사라진 오브젝트
					continue;
				GameObjMgr::GetInst()->SendPlacedObjectUpdate(id, obj, _service);
			}
		}
		
        // 시간 업데이트
        last_tick_time = now;
    }

	// ==================================================================
	// 불규칙 패킷 (애니메이션 변경, 스킬사용, 스킬Hit, 데미지맞음, 상태이상)
	// ==================================================================
	vector<tServerEvent> _vecScriptEvent = CSendServerEventMgr::GetInst()->GetVecEvent();
	for (int i = 0; i < _vecScriptEvent.size(); ++i)
	{
		switch (_vecScriptEvent[i].Type)
		{

		case SERVER_EVENT_TYPE::SEND_ANIM_PACKET:
		{
			AnimInfo* animInfo = (AnimInfo*)(_vecScriptEvent[i].lParam);
			uint64 _objectId = animInfo->targetId; // 애니메이션 변경할 id

			// 본인 플레이어 애니메이션 전송
			if (MyPlayer.id == _objectId) 
				GameObjMgr::GetInst()->SendObjectAnim(animInfo, _service);
			
			// 방장인 경우, 플레이어 아닌 오브젝트 애니메이션 전송
			else if(MyPlayer.host)		
				GameObjMgr::GetInst()->SendObjectAnim(animInfo, _service);

			delete animInfo; // 메모리 사용 해제
			animInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET:
		{
			SkillInfo* skillInfo = (SkillInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendSkillSpawn(skillInfo, _service);
			delete skillInfo; // 메모리 사용 해제
			skillInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SEND_HIT_PACKET:
		{
			HitInfo* hitInfo = (HitInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendSkillHit(hitInfo, _service);
			delete hitInfo; // 메모리 사용 해제
			hitInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SEND_DESPAWN_PACKET:
		{
			UINT64 objId = (UINT64)(_vecScriptEvent[i].wParam);
			float lifeSpan = (float)(_vecScriptEvent[i].lParam);
			GameObjMgr::GetInst()->SendDespawn(objId, lifeSpan, _service);
			break;
		}

		case SERVER_EVENT_TYPE::SEND_KDA_CS_PACKET:
		{
			KDACSInfo* _kdacsInfo = (KDACSInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendKDACS(_kdacsInfo, _service);
			delete _kdacsInfo; // 메모리 사용 해제
			_kdacsInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SEND_MTRL_PACKET:
		{
			MtrlInfo* _mtrlInfo = (MtrlInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendObjectMtrl(_mtrlInfo, _service);
			delete _mtrlInfo; // 메모리 사용 해제
			_mtrlInfo = nullptr;
			break;
		}


		}

	}
	CSendServerEventMgr::GetInst()->ClearServerSendEvent();
}

void ServerEventMgr::clienttick()
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		for (size_t i = 0; i < m_vecEvent.size(); ++i)
		{
			switch (m_vecEvent[i].Type)
			{

			case SERVER_EVENT_TYPE::MOVE_PACKET:
			{
				CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
				ObjectMove* objectMove = (ObjectMove*)(m_vecEvent[i].lParam);

				if (NewObject == nullptr) continue;

				if (NewObject->GetScript<CUnitScript>() != nullptr)
				{
					NewObject->GetScript<CUnitScript>()->SetLevel(objectMove->LV);
					NewObject->GetScript<CUnitScript>()->SetCurHP(objectMove->HP);
					NewObject->GetScript<CUnitScript>()->SetCurMP(objectMove->MP);
					NewObject->GetScript<CUnitScript>()->SetMaxHP(objectMove->MaxHP);
					NewObject->GetScript<CUnitScript>()->SetMaxMP(objectMove->MaxMP);

					NewObject->GetScript<CUnitScript>()->SetAttackPower(objectMove->AttackPower);
					NewObject->GetScript<CUnitScript>()->SetDefencePower(objectMove->DefencePower);
					NewObject->GetScript<CUnitScript>()->ApplyCC((CC)objectMove->CC);

					NewObject->GetScript<CUnitScript>()->SetUnitDead(objectMove->bUnitDead);
					NewObject->GetScript<CUnitScript>()->SetRcvMove(true);
					NewObject->GetScript<CUnitScript>()->SetMovePos(Vec3(objectMove->pos.x, objectMove->pos.y, objectMove->pos.z));
				}
				//NewObject->Transform()->SetRelativePos(Vec3(objectMove->pos.x, objectMove->pos.y, objectMove->pos.z));
				NewObject->Transform()->SetRelativeRot(Vec3(objectMove->moveDir.x, objectMove->moveDir.y, objectMove->moveDir.z));

				// 사용이 끝난 후에는 메모리를 해제
				delete objectMove;
				objectMove = nullptr;
			}
			break;

			case SERVER_EVENT_TYPE::ANIM_PACKET:
			{
				CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;

				if (NewObject == nullptr) continue;

				AnimInfo* animInfo = (AnimInfo*)(m_vecEvent[i].lParam);

				if (animInfo->bRepeat)
					NewObject->Animator3D()->PlayRepeat(animInfo->animName, animInfo->bRepeatBlend, animInfo->blend, animInfo->blendTime, animInfo->animSpeed);
				else
					NewObject->Animator3D()->PlayOnce(animInfo->animName, animInfo->blend, animInfo->blendTime, animInfo->animSpeed);

				// 사용이 끝난 후에는 메모리를 해제
				delete animInfo;
				animInfo = nullptr;
			}
			break;

			case SERVER_EVENT_TYPE::SKILL_HIT_PACKET:
			{
				uint64 hitId = (uint64)(m_vecEvent[i].wParam);
				SkillInfo* skillInfo = (SkillInfo*)m_vecEvent[i].lParam;

				// 내가 맞음 계산
				if (hitId == MyPlayer.id)
				{
					// 스킬 쏜애
					CGameObject* skillOwnerObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->OwnerId);

					// 스킬 맞은 애(본인)
					CGameObject* skillTargetObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->TargetId);

					// 피격 이벤트 발생
					GetHitEvent* evn = dynamic_cast<GetHitEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::GET_HIT));
					if (evn != nullptr)
					{
						evn->Clear();
						evn->SetUserObj(skillOwnerObj);
						evn->SetTargetObj(skillTargetObj);
						evn->SetSkillType(skillInfo->skillType);
						evn->SetSkillLevel(skillInfo->SkillLevel);

						CGameEventMgr::GetInst()->NotifyEvent(*evn);
					}
				}

				// 맞은애가 플레이어가 아니면(미니언)
				else if (GameObjMgr::GetInst()->FindPlayer(skillInfo->TargetId) == nullptr)
				{
					// 오브젝트가 맞음. 방장이 계산.
					if (MyPlayer.host)
					{
						// 스킬 쏜애
						CGameObject* skillOwnerObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->OwnerId);
						
						// 스킬 맞은 애(타인)
						CGameObject* skillTargetObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->TargetId);

						// 공격 이벤트 발생
						GetHitEvent* evn = dynamic_cast<GetHitEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::GET_HIT));
						if (evn != nullptr)
						{
							evn->Clear();
							evn->SetUserObj(skillOwnerObj);
							evn->SetTargetObj(skillTargetObj);
							evn->SetSkillType(skillInfo->skillType);
							evn->SetSkillLevel(skillInfo->SkillLevel);
							CGameEventMgr::GetInst()->NotifyEvent(*evn);
						}
					}
				}

			}
			break;

			case SERVER_EVENT_TYPE::DESPAWN_PACKET:
			{
				uint64	despawnId = (uint64)m_vecEvent[i].wParam;
				float   lifespan = (float)m_vecEvent[i].lParam;

				CGameObject* despawnObj = GameObjMgr::GetInst()->DeleteObjectInMap(despawnId);
				if(despawnObj != nullptr)
					despawnObj->SetLifeSpan(lifespan);
			}
			break;

			case SERVER_EVENT_TYPE::KDA_CS_PACKET:
			{
				KDACSInfo* kdacsInfo = (KDACSInfo*)m_vecEvent[i].wParam;
				
				
				if (kdacsInfo->deadObjUnitType == UnitType::CHAMPION)
				{
					// 1. 죽인게 나 && 죽은게 챔피언 -> 본인 K++
					if (kdacsInfo->killerId == MyPlayer.id)
						CSendServerEventMgr::GetInst()->AddMyKillCnt(1);

					// 2. 블루, 레드 스코어 업데이트  // 죽인게 플레이어가 아니면(Mob일경우) X
					CGameObject* killerObj = GameObjMgr::GetInst()->FindPlayer(kdacsInfo->killerId);
					CGameObject* vitimObj = GameObjMgr::GetInst()->FindPlayer(kdacsInfo->victimId);
					if (killerObj != nullptr && Faction::RED == killerObj->GetScript<CUnitScript>()->GetFaction())
						CSendServerEventMgr::GetInst()->AddRedScore(1);
					else if (killerObj != nullptr && Faction::BLUE == killerObj->GetScript<CUnitScript>()->GetFaction())
						CSendServerEventMgr::GetInst()->AddBlueScore(1);

					// 3. 이때 킬로그를 CSendServerEventMgr 에 UI가 사용할 수 있도록 이벤트 등록해둔다.
					tServerEvent evn = {};
					evn.Type = SERVER_EVENT_TYPE::Kill_LOG_PACKET;
					evn.wParam = (DWORD_PTR)killerObj;
					evn.lParam = (DWORD_PTR)vitimObj;
					CSendServerEventMgr::GetInst()->AddUISendEvent(evn);
				}
				else if (kdacsInfo->deadObjUnitType == UnitType::MELEE_MINION
					|| kdacsInfo->deadObjUnitType == UnitType::RANGED_MINION
					|| kdacsInfo->deadObjUnitType == UnitType::SIEGE_MINION
					|| kdacsInfo->deadObjUnitType == UnitType::SUPER_MINION)
				{
					// 1. 죽인게 나 && 죽은게 미니언 -> 본인 CS++
					if (kdacsInfo->killerId == MyPlayer.id)
						CSendServerEventMgr::GetInst()->AddMyCSCnt(1);
				}
				//else if(정글몹) //효과 UI // 죽인게 나 && 죽은게 정글몹 -> 본인CS AddMyCSCnt(4);
				
				// 사용이 끝난 후에는 메모리를 해제
				delete kdacsInfo;
				kdacsInfo = nullptr;
			}
			break;
			case SERVER_EVENT_TYPE::MTRL_PACKET:
			{
				MtrlInfo*	mtrlInfo = (MtrlInfo*)m_vecEvent[i].wParam;
				CGameObject* pObj = GameObjMgr::GetInst()->FindAllObject(mtrlInfo->targetId);
				
				if (pObj == nullptr || pObj->IsDead()) continue;

				pObj->MeshRender()->GetMaterial(mtrlInfo->iMtrlIndex)->SetTexParam(mtrlInfo->tex_param, CResMgr::GetInst()->FindRes<CTexture>(mtrlInfo->wTexName));

				// 사용이 끝난 후에는 메모리를 해제
				delete mtrlInfo;
				mtrlInfo = nullptr;
			}
			break;
			}
		}

		m_vecEvent.clear();
	}
}