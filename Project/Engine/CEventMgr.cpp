#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "CTransform.h"

#include <mutex>
CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}


void CEventMgr::tick()
{
	m_LevelChanged = false;

	GC_Clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{
		// wParam : GameObject, lParam : Layer Index
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;
			CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(NewObject, iLayerIdx, false);
			if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
			{
				NewObject->begin();
			}

			m_LevelChanged = true;
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			CGameObject* DeleteObject = (CGameObject*)m_vecEvent[i].wParam;

			if (false == DeleteObject->m_bDead)
			{
				DeleteObject->m_bDead = true;
				m_vecGC.push_back(DeleteObject);
			}			
		}
			break;

		case EVENT_TYPE::ADD_CHILD:
			// wParam : ParentObject, lParam : ChildObject
		{
			CGameObject* pDestObj = (CGameObject*)m_vecEvent[i].wParam;
			CGameObject* pSrcObj = (CGameObject*)m_vecEvent[i].lParam;

			// �θ�� ������ ������Ʈ�� ������, Child ������Ʈ�� �ֻ��� �θ� ������Ʈ�� �ȴ�.
			if (nullptr == pDestObj)
			{
				if (pSrcObj->GetParent())
				{
					// ���� �θ���� ���� ����
					pSrcObj->DisconnectFromParent();
					// �ֻ��� �θ� ������Ʈ��, �Ҽ� ���̾ ���
					pSrcObj->AddParentList();
				}
			}
			else
			{
				pDestObj->AddChild(pSrcObj);
			}

			m_LevelChanged = true;
		}
			

		
			break;
		case EVENT_TYPE::DELETE_RESOURCE:
			// wParam : RES_TYPE, lParam : Resource Adress
		{
			RES_TYPE type = (RES_TYPE)m_vecEvent[i].wParam;
			CRes* pRes = (CRes*)m_vecEvent[i].lParam;
			CResMgr::GetInst()->DeleteRes(type, pRes->GetKey());
		}

			break;
		case EVENT_TYPE::LEVEL_CHANGE:
		{
			// wParam : Level Adress
			CLevel* Level = (CLevel*)m_vecEvent[i].wParam;
			CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
			
			//���� ������ Play������ Ȯ��
			bool IsPlay = false;
			if (CurLevel != nullptr) {
				if (CurLevel->GetState() == LEVEL_STATE::PLAY) {
					IsPlay = true;
				}
			}

			CRenderMgr::GetInst()->SetGizMoTargetObj(nullptr);
			//���� �ٲ� ����� Ÿ�ٵ� null�� �ٲ���

			CLevelMgr::GetInst()->ChangeLevel(Level);
			m_LevelChanged = true;

			//���� �ʱ�ȭ
			const map<wstring, Ptr<CRes>> SoundRes = CResMgr::GetInst()->GetResources(RES_TYPE::SOUND);
			for (const auto& kv : SoundRes) {
				Ptr<CRes> Res = kv.second;
				((CSound*)Res.Get())->Stop();
			}

			//���� ������ play������ ���� ������ play�� �ٲ�����.
			if (IsPlay) {
				CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::PLAY);
			}
		}
			break;		


		case EVENT_TYPE::MOVE_PACKET:
		{
			std::mutex m;
			m.lock();
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			PlayerMove* playerMove = (PlayerMove*)(m_vecEvent[i].lParam);

			// ���� obj�� playerMove�� ����Ͽ� �̵� �̺�Ʈ�� ó���� �� �ֽ��ϴ�.

			CTransform* trans = (CTransform*)NewObject->GetComponent(COMPONENT_TYPE::TRANSFORM);
			trans->SetRelativePos(Vec3(playerMove->pos.x, playerMove->pos.y, playerMove->pos.z));
			trans->SetRelativeRot(Vec3(playerMove->moveDir.x, playerMove->moveDir.y, playerMove->moveDir.z));

			// ����� ���� �Ŀ��� �޸𸮸� �����ϴ� ���� ���� ������.
			//delete playerMove;
			m.unlock();
		}

		break;
		}
	}

	m_vecEvent.clear();
}


void CEventMgr::GC_Clear()
{
	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		if (nullptr != m_vecGC[i])
		{
			// �ڽ� Ÿ�� ������Ʈ�� ���
			if (m_vecGC[i]->GetParent())			
				m_vecGC[i]->DisconnectFromParent();
			
			delete m_vecGC[i];

			m_LevelChanged = true;
		}		
	}
	m_vecGC.clear();
}