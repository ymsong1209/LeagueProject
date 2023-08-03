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
				if (obj == nullptr) // �̹� �׾ ����� ������Ʈ
					continue;
				GameObjMgr::GetInst()->SendObjectMove(id, obj, _service);
			}

			// ��ġ�� ������Ʈ�� �������� ������ HP�� ������ ���涧�� ������.(SendTowersUpdate)
			map<uint64, CGameObject*> _placedObject = GameObjMgr::GetInst()->GetPlacedObjects();
			for (auto& pair : _placedObject) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
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
			UINT64 _killerId = (UINT64)(_vecScriptEvent[i].wParam);
			UnitType _deadObjUnitType = (UnitType)(_vecScriptEvent[i].lParam);
			// �ۼ���
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

			// ����� ���� �Ŀ��� �޸𸮸� ����
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
			if (skillInfo->TargetId == MyPlayer.id)
			{
				// ��ų ���
				CGameObject* skillOwnerObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->OwnerId);

				// ��ų ���� ��(����)
				CGameObject* skillTargetObj = GameObjMgr::GetInst()->FindAllObject(skillInfo->TargetId);

				// �ǰ� �̺�Ʈ �߻�
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
			//despawnObj->SetLifeSpan(lifespan); // DT ���峪�� �ӽ÷� DestroyObject �����(��� ����)
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
