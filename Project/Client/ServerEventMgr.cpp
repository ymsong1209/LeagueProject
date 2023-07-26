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

    // 100ms = 1/10 sec
    if (time_diff.count() >= 100) 
    {      
        // ���� �÷��̾� move ��Ŷ�� ������ ������.
        GameObjMgr::GetInst()->SendMyPlayerMove(_service);

		// �����Ͻ�, ��� ������Ʈ move��Ŷ�� ������ ������. 
		if (MyPlayer.host)
			//GameObjMgr::GetInst()->SendAllObjectMove(); // �ȿ��� ������Ʈ��ŭ �ݺ����� ����.

		// S_SPAWN_OBJECT
		// �ڵ鷯���� �������� ��ο��� spawnObject ��Ŷ�� �´�. (������Ʈ Ÿ�� : �̴Ͼ�)
		// ���� �����̸� �� �̴Ͼ� ��ũ��Ʈ�� �� �̴Ͼ��� �����ϰ�
		// �ٸ� Ŭ���̾�Ʈ���� other�̴Ͼ� ��ũ��Ʈ�� �� �̴Ͼ��� �����Ѵ�. 
		// C_UPDATE_OBJECT
		// ���� �÷��̾ ������ ��� ������Ʈ(�̴Ͼ�, ����ü)�� ������ ���� ��Ŷ�� ������. -> �� ������ ������ ��ο��� �Ѹ���. (S_UPDATE_OBJECT)


        // �ð� ������Ʈ
        last_tick_time = now;
    }

	// MyPlayer �� ��ų�� ������ �÷��� üũ -> ������ ��ų�� ����Ѵٴ� ��� ��Ŷ�� ������.

    // ������ �ش� id�÷��̾ �ش� ��ų�� ����Ѵٰ� ��ε�ĳ����. �װ� �ް� ���¸� �ٲ���. 
	// ����ü ��ų�Ͻ�, ������ ������ �Ѵ�. 
	// if �����Ͻ� ���� ó��

}

void ServerEventMgr::clienttick()
{
	std::mutex m;
	m.lock();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{

		case EVENT_TYPE::MOVE_PACKET:
		{

			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			PlayerMove* playerMove = (PlayerMove*)(m_vecEvent[i].lParam);


			NewObject->Transform()->SetRelativePos(Vec3(playerMove->pos.x, playerMove->pos.y, playerMove->pos.z));
			NewObject->Transform()->SetRelativeRot(Vec3(playerMove->moveDir.x, playerMove->moveDir.y, playerMove->moveDir.z));

			// ���� ������ ���µ� üũ�ؾ���.
			if(playerMove->state == PlayerMove::MOVE 
				&& GameObjMgr::GetInst()->GetPrevState() == PlayerMove::IDLE)
				NewObject->Animator3D()->Play(L"Jinx\\Run_Base", true, 0.15f);

			else if(playerMove->state == PlayerMove::IDLE
				&& GameObjMgr::GetInst()->GetPrevState() == PlayerMove::MOVE)
				NewObject->Animator3D()->Play(L"Jinx\\Idle1_Base", true, 0.1f);


			// ����� ���� �Ŀ��� �޸𸮸� ����
			delete playerMove;
			playerMove = nullptr;
		}

		break;
		}

	}

	m_vecEvent.clear(); 
	m.unlock();
}
