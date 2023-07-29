#include "pch.h"
#include "ServerEventMgr.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "GameObjMgr.h"
#include <Engine/ServerPacket.h>
#include <Engine/components.h>
#include <Script/CSendServerEventMgr.h>

#include <Script/CChampionScript.h>

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
	if (time_diff.count() >= 80) 
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

	// ==========================================
	// �ұ�Ģ ��Ŷ
	// =========================================
	// ���⼭!!! ��ũ��Ʈ �̺�Ʈ �Ŵ����� �̺�Ʈ vector�� Ȯ���ϰ� ������. 

	//  �ִϸ��̼� ����, ��ų���, ��ųHit, ����������, �����̻���,}
	// GameObjMgr::GetInst()->SendMyPlayerAnim(_service);
	// 
	vector<tServerEvent> _vecEvent = CSendServerEventMgr::GetInst()->GetVecEvent();
	for (int i = 0; i < _vecEvent.size(); ++i)
	{
		if (_vecEvent.size() == 0) break;

		switch (_vecEvent[i].Type)
		{

		case SERVER_EVENT_TYPE::SEND_ANIM_PACKET:
		{
			// wParam �ϴ� �Ⱦ�����. animinfo animIdx�� ������Ʈid�� �޾ƿ��� ����.
			//CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			AnimInfo* animInfo = (AnimInfo*)(_vecEvent[i].lParam);
			
			uint64 _objectId = animInfo->animIdx; // �ִϸ��̼� ������ id

			//if (_objectId == MyPlayer.id) break;

			// ���⼭ ������ ������. 
			std::cout << "Send C_OBJECT_ANIM Pakcet : Send ID : " << _objectId << endl;

			AnimInfoPacket animInfoPacket = {};
			animInfoPacket.blend = animInfo->blend;
			animInfoPacket.blendTime = animInfo->blendTime;

			wstring _animName = animInfo->animName;

			PKT_C_OBJECT_ANIM_WRITE  pktWriter(_objectId, animInfoPacket);
			PKT_C_OBJECT_ANIM_WRITE::AnimNameList animNamePacket = pktWriter.ReserveAnimNameList(_animName.size());
			for (int i = 0; i < _animName.size(); i++)
				animNamePacket[i] = { _animName[i] };

			SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
			_service->Broadcast(sendBuffer);

			delete animInfo;
			animInfo = nullptr;

			std::cout << "===============================" << endl;
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
			
			cout << "Real PlayRepeat about " << animInfo->animIdx << endl;
			// Play�Լ��� �ٲ���?
			NewObject->Animator3D()->PlayRepeat(animInfo->animName, animInfo->blend, animInfo->blendTime);

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
