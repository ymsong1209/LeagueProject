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
	// ��Ģ ��Ŷ(MovePacket) : 100ms = 1/10 sec
    // ========================================
	if (time_diff.count() >= 100)  // ���߿� 40���� ��ħ
    {     
        // 1. ���� �÷��̾� move ��Ŷ�� ������ ������. (ObjectMove)
		// ������ : �ȿ����϶� move ��Ŷ�� �Ⱥ����� LV,HP,MP �� ������Ʈ�� �ȉ�...�׳� ��������� ��.(���߿��� �ȿ����϶��� ��Ŷ������ ���濹��)
        GameObjMgr::GetInst()->SendMyPlayerMove(_service);

		// 2. �����Ͻ�, ��� ������Ʈ move��Ŷ�� ������ ������.(��ž,������,�ؼ��� ����)
		if (MyPlayer.host)
		{
			// �ϴ��� pos�ٸ��� ������Ʈ �������� ���� pos�� ���Ƶ� ��� �������� �����ϱ�
			map<uint64, CGameObject*> _objects = GameObjMgr::GetInst()->GetObjects();
			for (auto& pair : _objects) {
				uint64 id = pair.first; 
				CGameObject* obj = pair.second; 
				if (obj->IsDead()) // �̹� �׾ ����� ������Ʈ
					continue;
				GameObjMgr::GetInst()->SendObjectMove(id, obj, _service);
			}

			// ��ġ�� ������Ʈ�� �������� ������ HP�� ������ ���涧�� ������.(SendTowersUpdate)
			map<uint64, CGameObject*> _placedObject = GameObjMgr::GetInst()->GetPlacedObjects();
			for (auto& pair : _placedObject) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
				if (obj->IsDead()) // �̹� �׾ ����� ������Ʈ
					continue;
				GameObjMgr::GetInst()->SendPlacedObjectUpdate(id, obj, _service);
			}
		}
		
        // �ð� ������Ʈ
        last_tick_time = now;
    }

	// ==================================================================
	// �ұ�Ģ ��Ŷ (�ִϸ��̼� ����, ��ų���, ��ųHit, ����������, �����̻�)
	// ==================================================================
	vector<tServerEvent> _vecScriptEvent = CSendServerEventMgr::GetInst()->GetVecEvent();
	for (int i = 0; i < _vecScriptEvent.size(); ++i)
	{
		switch (_vecScriptEvent[i].Type)
		{

		case SERVER_EVENT_TYPE::SEND_ANIM_PACKET:
		{
			AnimInfo* animInfo = (AnimInfo*)(_vecScriptEvent[i].lParam);
			uint64 _objectId = animInfo->targetId; // �ִϸ��̼� ������ id

			// ���� �÷��̾� �ִϸ��̼� ����
			if (MyPlayer.id == _objectId) 
				GameObjMgr::GetInst()->SendObjectAnim(animInfo, _service);
			
			// ������ ���, �÷��̾� �ƴ� ������Ʈ �ִϸ��̼� ����
			else if(MyPlayer.host)		
				GameObjMgr::GetInst()->SendObjectAnim(animInfo, _service);

			delete animInfo; // �޸� ��� ����
			animInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET:
		{
			SkillInfo* skillInfo = (SkillInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendSkillSpawn(skillInfo, _service);
			delete skillInfo; // �޸� ��� ����
			skillInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SEND_HIT_PACKET:
		{
			HitInfo* hitInfo = (HitInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendSkillHit(hitInfo, _service);
			delete hitInfo; // �޸� ��� ����
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
			delete _kdacsInfo; // �޸� ��� ����
			_kdacsInfo = nullptr;
			break;
		}
		case SERVER_EVENT_TYPE::SEND_MTRL_PACKET:
		{
			MtrlInfo* _mtrlInfo = (MtrlInfo*)(_vecScriptEvent[i].wParam);
			GameObjMgr::GetInst()->SendObjectMtrl(_mtrlInfo, _service);
			delete _mtrlInfo; // �޸� ��� ����
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

				// ����� ���� �Ŀ��� �޸𸮸� ����
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

				// ����� ���� �Ŀ��� �޸𸮸� ����
				delete animInfo;
				animInfo = nullptr;
			}
			break;

			case SERVER_EVENT_TYPE::SKILL_HIT_PACKET:
			{
				uint64 hitId = (uint64)(m_vecEvent[i].wParam);
				SkillInfo* skillInfo = (SkillInfo*)m_vecEvent[i].lParam;

				// ���� ���� ���
				if (hitId == MyPlayer.id)
				{
					// ��ų ���
					CGameObject* skillOwnerObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->OwnerId);

					// ��ų ���� ��(����)
					CGameObject* skillTargetObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->TargetId);

					// �ǰ� �̺�Ʈ �߻�
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

				// �����ְ� �÷��̾ �ƴϸ�(�̴Ͼ�)
				else if (GameObjMgr::GetInst()->FindPlayer(skillInfo->TargetId) == nullptr)
				{
					// ������Ʈ�� ����. ������ ���.
					if (MyPlayer.host)
					{
						// ��ų ���
						CGameObject* skillOwnerObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->OwnerId);
						
						// ��ų ���� ��(Ÿ��)
						CGameObject* skillTargetObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->TargetId);

						// ���� �̺�Ʈ �߻�
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
					// 1. ���ΰ� �� && ������ è�Ǿ� -> ���� K++
					if (kdacsInfo->killerId == MyPlayer.id)
						CSendServerEventMgr::GetInst()->AddMyKillCnt(1);

					// 2. ���, ���� ���ھ� ������Ʈ  // ���ΰ� �÷��̾ �ƴϸ�(Mob�ϰ��) X
					CGameObject* killerObj = GameObjMgr::GetInst()->FindPlayer(kdacsInfo->killerId);
					CGameObject* vitimObj = GameObjMgr::GetInst()->FindPlayer(kdacsInfo->victimId);
					if (killerObj != nullptr && Faction::RED == killerObj->GetScript<CUnitScript>()->GetFaction())
						CSendServerEventMgr::GetInst()->AddRedScore(1);
					else if (killerObj != nullptr && Faction::BLUE == killerObj->GetScript<CUnitScript>()->GetFaction())
						CSendServerEventMgr::GetInst()->AddBlueScore(1);

					// 3. �̶� ų�α׸� CSendServerEventMgr �� UI�� ����� �� �ֵ��� �̺�Ʈ ����صд�.
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
					// 1. ���ΰ� �� && ������ �̴Ͼ� -> ���� CS++
					if (kdacsInfo->killerId == MyPlayer.id)
						CSendServerEventMgr::GetInst()->AddMyCSCnt(1);
				}
				//else if(���۸�) //ȿ�� UI // ���ΰ� �� && ������ ���۸� -> ����CS AddMyCSCnt(4);
				
				// ����� ���� �Ŀ��� �޸𸮸� ����
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

				// ����� ���� �Ŀ��� �޸𸮸� ����
				delete mtrlInfo;
				mtrlInfo = nullptr;
			}
			break;
			}
		}

		m_vecEvent.clear();
	}
}