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

	// 규칙 패킷(MovePacket) : 100ms = 1/10 sec
    if (time_diff.count() >= 100) 
    {     
        // 1. 본인 플레이어 move 패킷을 서버에 보낸다. (움직임,방향,lv, HP MP AD Defence)
		// 문제점 : 안움직일땐 move 패킷을 안보내서 LV,HP,MP 등 업데이트가 안됌...그냥 움직여라는 뜻.(나중에는 안움직일때도 패킷보내게 변경예정)
        GameObjMgr::GetInst()->SendMyPlayerMove(_service);

		// 2. 방장일시, 모든 오브젝트 move패킷을 서버에 보낸다. (포탑,억제기,넥서스 제외)
		if (MyPlayer.host)
		{
			// 일단은 pos다르면 업데이트 안하지만 추후 pos가 같아도 계속 보내도록 변경하기
			map<uint64, CGameObject*> _objects = GameObjMgr::GetInst()->GetObjects();
			for (auto& pair : _objects) {
				uint64 id = pair.first; 
				CGameObject* obj = pair.second; 
				GameObjMgr::GetInst()->SendObjectMove(id, obj, _service);
			}

			// 배치형 오브젝트는 움직임이 없으니 HP에 변동이 생길때만 보낸다.(SendTowersUpdate)
			map<uint64, CGameObject*> _towers = GameObjMgr::GetInst()->GetObjects();
			for (auto& pair : _towers) {
				uint64 id = pair.first;
				CGameObject* obj = pair.second;
				GameObjMgr::GetInst()->SendTowerUpdate(id, obj, _service);
			}
		}
		
        // 시간 업데이트
        last_tick_time = now;
    }

	// 불규칙 패킷
	// 
	// 여기서!!! 스크립트 이벤트 매니저의 이벤트 vector를 확인하고 보낸다. 
	// vector<tEvent> _vecEvent = ScriptMgr::GetInst()->GetVecEvent();
	// for(_vecEvent)
	// { switch case : 
	//  애니메이션 변경, 스킬사용, 스킬Hit, 데미지맞음, 상태이상중,}
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

			// 사용이 끝난 후에는 메모리를 해제
			delete objectMove;
			objectMove = nullptr;
		}
		break;

		case SERVER_EVENT_TYPE::ANIM_PACKET:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			AnimInfo* animInfo = (AnimInfo*)(m_vecEvent[i].lParam);

			NewObject->Animator3D()->Play(animInfo->animName, animInfo->blend, animInfo->blendTime);

			// 사용이 끝난 후에는 메모리를 해제
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
