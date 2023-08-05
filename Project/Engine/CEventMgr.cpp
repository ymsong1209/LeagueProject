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

			// 부모로 지정된 오브젝트가 없으면, Child 오브젝트가 최상위 부모 오브젝트가 된다.
			if (nullptr == pDestObj)
			{
				if (pSrcObj->GetParent())
				{
					// 기존 부모와의 연결 해제
					pSrcObj->DisconnectFromParent();
					// 최상위 부모 오브젝트로, 소속 레이어에 등록
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
			
			//현재 레벨이 Play였는지 확인
			bool IsPlay = false;
			if (CurLevel != nullptr) {
				if (CurLevel->GetState() == LEVEL_STATE::PLAY) {
					IsPlay = true;
				}
			}

			CRenderMgr::GetInst()->SetGizMoTargetObj(nullptr);
			//레벨 바뀔때 기즈모 타겟도 null로 바꿔줌

			CLevelMgr::GetInst()->ChangeLevel(Level);
			m_LevelChanged = true;

			//사운드 초기화
			const map<wstring, Ptr<CRes>> SoundRes = CResMgr::GetInst()->GetResources(RES_TYPE::SOUND);
			for (const auto& kv : SoundRes) {
				Ptr<CRes> Res = kv.second;
				((CSound*)Res.Get())->Stop();
			}

			//이전 레벨이 play였으면 현재 레벨도 play로 바껴야함.
			if (IsPlay) {
				CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::PLAY);
			}
		}
			break;		
		case EVENT_TYPE::LAYER_CHANGE:
		{
			CGameObject* Obj = (CGameObject*)m_vecEvent[i].wParam;
			int NewLayerIdx = (int)m_vecEvent[i].lParam;

			// 인자로 들어온 오브젝트가 최상위 부모 오브젝트라면
			if (Obj->GetParent() == nullptr)
			{
				// 레이어의 부모 벡터에서 해당 오브젝트 삭제
				Obj->ChangeToChildType();
				if (NewLayerIdx != -1)
				{
					CUR_LEVEL->AddGameObject(Obj, NewLayerIdx, false);
				}
			}
			else
			{
				Obj->m_iLayerIdx = NewLayerIdx;
			}

			m_LevelChanged = true;
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
			// 자식 타입 오브젝트인 경우
			if (m_vecGC[i]->GetParent())			
				m_vecGC[i]->DisconnectFromParent();
			
			delete m_vecGC[i];
			m_vecGC[i] = nullptr;

			m_LevelChanged = true;
		}		
	}
	m_vecGC.clear();
}