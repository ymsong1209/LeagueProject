#include "pch.h"
#include "ServerEventMgr.h"

#include "GameObjMgr.h"
#include <Engine/ServerPacket.h>
#include <Engine/components.h>

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

	// ��Ģ ��Ŷ(MovePacket) : 100ms = 1/10 sec
    if (time_diff.count() >= 100) 
    {     
        // 1. ���� �÷��̾� move ��Ŷ�� ������ ������. (������,����,lv, HP MP AD Defence)
		// ������ : �ȿ����϶� move ��Ŷ�� �Ⱥ����� LV,HP,MP �� ������Ʈ�� �ȉ�...�׳� ��������� ��.(���߿��� �ȿ����϶��� ��Ŷ������ ���濹��)
        GameObjMgr::GetInst()->SendMyPlayerMove(_service);

		// 2. �����Ͻ�, ��� ������Ʈ move��Ŷ�� ������ ������. (��ž,������,�ؼ��� ����)
		if (MyPlayer.host)
		{
			// �ϴ��� pos�ٸ��� ������Ʈ �������� ���� pos�� ���Ƶ� ��� �������� �����ϱ�
			map<uint64, CGameObject*> _objects = GameObjMgr::GetInst()->GetObjects();
			for (auto& pair : _objects) {
				uint64 id = pair.first; 
				CGameObject* obj = pair.second; 
				GameObjMgr::GetInst()->SendObjectMove(id, obj, _service);
			}

			// ��ġ�� ������Ʈ�� �������� ������ HP�� ������ ���涧�� ������.(SendTowersUpdate)
			map<uint64, CGameObject*> _towers = GameObjMgr::GetInst()->GetObjects();
			for (auto& pair : _towers) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
				GameObjMgr::GetInst()->SendTowerUpdate(id, obj, _service);
			}
		}
		
        // �ð� ������Ʈ
        last_tick_time = now;
    }

	// �ұ�Ģ ��Ŷ
	// 
	// ���⼭!!! ��ũ��Ʈ �̺�Ʈ �Ŵ����� �̺�Ʈ vector�� Ȯ���ϰ� ������. 
	// vector<tEvent> _vecEvent = ScriptMgr::GetInst()->GetVecEvent();
	// for(_vecEvent)
	// { switch case : 
	//  �ִϸ��̼� ����, ��ų���, ��ųHit, ����������, �����̻���,}
	// GameObjMgr::GetInst()->SendMyPlayerAnim(_service);
	// 
	
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

			NewObject->Animator3D()->Play(animInfo->animName, animInfo->blend, animInfo->blendTime);

			// ����� ���� �Ŀ��� �޸𸮸� ����
			delete animInfo;
			animInfo = nullptr;
		}
		break;
		case SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET:
		{

		}
		break;
		case SERVER_EVENT_TYPE::SKILL_HIT_PACKET:
		{

		}
		break;
		}

	}

	m_vecEvent.clear(); 
	m.unlock();
}
