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

	// 규칙 패킷 : 100ms = 1/10 sec
    if (time_diff.count() >= 100) 
    {      
        // 1. 본인 플레이어 move 패킷을 서버에 보낸다.
        GameObjMgr::GetInst()->SendMyPlayerMove(_service);

		// 2. 본인 플레이어 anim 패킷을 서버에 보낸다.
		//GameObjMgr::GetInst()->SendMyPlayerAnim(_service);
		
		if (MyPlayer.host)
			// 방장일시, 모든 오브젝트 move패킷을 서버에 보낸다. 
			// _objects에 있는 모든것.
			//GameObjMgr::GetInst()->SendAllObjectMove(); // 안에서 오브젝트만큼 반복문을 돌듯.

		// S_SPAWN_OBJECT
		// 핸들러에서 서버에서 모두에게 spawnObject 패킷이 온다. (오브젝트 타입 : 미니언)
		// 내가 방장이면 찐 미니언 스크립트를 단 미니언을 생성하고
		// 다른 클라이언트들은 other미니언 스크립트를 단 미니언을 생성한다. 
		// C_UPDATE_OBJECT
		// 이후 플레이어를 제외한 모든 오브젝트(미니언, 투사체)는 방장이 관련 패킷을 보낸다. -> 이 정보를 서버가 모두에게 뿌린다. (S_UPDATE_OBJECT)


        // 시간 업데이트
        last_tick_time = now;
    }

	// MyPlayer 가 스킬을 쓰는지 플래그 체크 -> 서버에 스킬을 사용한다는 허락 패킷을 보낸다.

    // 서버는 해당 id플레이어가 해당 스킬을 사용한다고 브로드캐스팅. 그걸 받고 상태를 바꿔줌. 
	// 투사체 스킬일시, 생성은 방장이 한다. 
	// if 방장일시 모든걸 처리

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
			ObjectMove* playerMove = (ObjectMove*)(m_vecEvent[i].lParam);


			NewObject->Transform()->SetRelativePos(Vec3(playerMove->pos.x, playerMove->pos.y, playerMove->pos.z));
			NewObject->Transform()->SetRelativeRot(Vec3(playerMove->moveDir.x, playerMove->moveDir.y, playerMove->moveDir.z));


			// 사용이 끝난 후에는 메모리를 해제
			delete playerMove;
			playerMove = nullptr;
		}

		break;
		}

	}

	m_vecEvent.clear(); 
	m.unlock();
}
