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
				if (obj == nullptr) // 이미 죽어서 사라진 오브젝트
					continue;
				GameObjMgr::GetInst()->SendObjectMove(id, obj, _service);
			}

			// 배치형 오브젝트는 움직임이 없으니 HP에 변동이 생길때만 보낸다.(SendTowersUpdate)
			map<uint64, CGameObject*> _placedObject = GameObjMgr::GetInst()->GetPlacedObjects();
			for (auto& pair : _placedObject) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
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
			UINT64 _killerId = (UINT64)(_vecScriptEvent[i].wParam);
			UnitType _deadObjUnitType = (UnitType)(_vecScriptEvent[i].lParam);
			// 작성중
			//GameObjMgr::GetInst()->SendKDACS(_killerId, _deadObjUnitType, _service);
			break;
		}


		}

	}
	CSendServerEventMgr::GetInst()->ClearServerSendEvent();
}

void ServerEventMgr::clienttick()
{
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{

		case SERVER_EVENT_TYPE::MOVE_PACKET:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			ObjectMove* objectMove = (ObjectMove*)(m_vecEvent[i].lParam);

			if (NewObject->GetScript<CUnitScript>() != nullptr)
			{
				// NewObject->GetScript<CUnitScript>()->SetLV(objectMove->LV);
				NewObject->GetScript<CUnitScript>()->SetCurHP(objectMove->HP);
				NewObject->GetScript<CUnitScript>()->SetCurMP(objectMove->MP);
				NewObject->GetScript<CUnitScript>()->SetAttackPower(objectMove->AttackPower);
				NewObject->GetScript<CUnitScript>()->SetDefencePower(objectMove->DefencePower);
			}
			NewObject->Transform()->SetRelativePos(Vec3(objectMove->pos.x, objectMove->pos.y, objectMove->pos.z));
			NewObject->Transform()->SetRelativeRot(Vec3(objectMove->moveDir.x, objectMove->moveDir.y, objectMove->moveDir.z));

			// 사용이 끝난 후에는 메모리를 해제
			delete objectMove;
			objectMove = nullptr;
		}
		break;

		case SERVER_EVENT_TYPE::ANIM_PACKET:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			AnimInfo* animInfo = (AnimInfo*)(m_vecEvent[i].lParam);

			if (animInfo->bRepeat)
				NewObject->Animator3D()->PlayRepeat(animInfo->animName, true, animInfo->blend, animInfo->blendTime);
			else
				NewObject->Animator3D()->PlayOnce(animInfo->animName, animInfo->blend, animInfo->blendTime);

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
			if (skillInfo->TargetId == MyPlayer.id)
			{
				// 스킬 쏜애
				CGameObject* skillOwnerObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->OwnerId);

				// 스킬 맞은 애(본인)
				CGameObject* skillTargetObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->TargetId);

				// 피격 이벤트 발생
				GetHitEvent* evn = dynamic_cast<GetHitEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_GET_HIT));
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
					GetHitEvent* evn = dynamic_cast<GetHitEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_GET_HIT));
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
			float   lifespan =  (float)m_vecEvent[i].lParam;

			CGameObject* despawnObj = GameObjMgr::GetInst()->DeleteObjectInMap(despawnId);
			DestroyObject(despawnObj);
			//despawnObj->SetLifeSpan(lifespan); // DT 고장나서 임시로 DestroyObject 사용중(즉시 삭제)
		}
		break;

		case SERVER_EVENT_TYPE::KDA_CS_PACKET:
		{
			uint64	 killerId = (uint64)m_vecEvent[i].wParam;
			UnitType deadObjUnitType = (UnitType)m_vecEvent[i].lParam;
		}
		break;

		}
	}

	m_vecEvent.clear(); 
}
