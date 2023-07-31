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
	if (time_diff.count() >= 100)  // ���߿� 20���� ��ħ
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
			map<uint64, CGameObject*> _towers = GameObjMgr::GetInst()->GetTowers();
			for (auto& pair : _towers) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
				GameObjMgr::GetInst()->SendTowerUpdate(id, obj, _service);
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
		if (_vecScriptEvent.size() == 0) break;

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



		}


	}
	CSendServerEventMgr::GetInst()->ClearServerSendEvent();
}

void ServerEventMgr::clienttick()
{
	std::mutex m;
	m.lock();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{

		case SERVER_EVENT_TYPE::MOVE_PACKET:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			ObjectMove* objectMove = (ObjectMove*)(m_vecEvent[i].lParam);

			//NewObject->?()->SetLV(objectMove->LV);
			//NewObject->?()->SetHP(objectMove->HP);
			//NewObject->?()->SetMP(objectMove->MP);
			//NewObject->?()->SetAD(objectMove->AD);
			//NewObject->?()->SetDefence(objectMove->Defence);
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
			
			if(animInfo->bRepeat)
				NewObject->Animator3D()->PlayRepeat(animInfo->animName, animInfo->blend, animInfo->blendTime);
			else
				NewObject->Animator3D()->PlayOnce(animInfo->animName, animInfo->blend, animInfo->blendTime);
			
			// ����� ���� �Ŀ��� �޸𸮸� ����
			delete animInfo;
			animInfo = nullptr;
		}
		break;
		//case SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET:
		//{
		//
		//}
		//break;
		case SERVER_EVENT_TYPE::SKILL_HIT_PACKET:
		{
			// ���� ���̵�� GameObject ã�ƿ���
			CGameObject* Obj = GameObjMgr::GetInst()->FindAllObject(//���� ���̵�);

			// ���� �̺�Ʈ �߻�
			GetHitEvent* evn = dynamic_cast<GetHitEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_GET_HIT));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(Obj);
				//evn->SetSkillType(UnitScript->GetOwner()->GetID());
			
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
		break;
		}

	}

	m_vecEvent.clear(); 
	m.unlock();
}
