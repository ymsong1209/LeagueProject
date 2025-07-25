#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"

#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CEventMgr.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_LifeTime(0.f)
	, m_CurLifeTime(0.f)
	, m_bLifeSpan(false)
{
}

CGameObject::CGameObject(const CGameObject& _Other)
	: CEntity(_Other)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_LifeTime(0.f)
	, m_CurLifeTime(0.f)
	, m_bLifeSpan(false)
{
	// Component 복사
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Other.m_arrCom[i])
		{
			AddComponent(_Other.m_arrCom[i]->Clone());
		}
	}

	// Script 복사
	for (size_t i = 0; i < _Other.m_vecScript.size(); ++i)
	{
		AddComponent(_Other.m_vecScript[i]->Clone());
	}

	// 자식 오브젝트 복사
	for (size_t i = 0; i < _Other.m_vecChild.size(); ++i)
	{
		AddChild(_Other.m_vecChild[i]->Clone());
	}	
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::RegisterObject()
{
	// 레이어 등록
	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	if (GetRenderComponent() != nullptr) {
		GetRenderComponent()->SetSortExcept(false);
	}
	pCurLayer->RegisterObject(this);
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->RegisterObject();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	if (m_bLifeSpan)
	{
		m_CurLifeTime += DT;
		if (m_LifeTime < m_CurLifeTime)
		{
			DestroyObject(this);
		}
	}


	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
		
	// 소속 레이어가 없는데 finaltick 이 호출되었다.
	assert(-1 != m_iLayerIdx); 

	//// 레이어 등록
	//CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	//pCurLayer->RegisterObject(this);
}

void CGameObject::finaltick_module()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick_module();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick_module();
	}
}

void CGameObject::render()
{
	if (nullptr != m_RenderCom)
		m_RenderCom->render();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	_Component->m_pOwner = this;

	// 컴포넌트가 스크립트인 경우
	if (COMPONENT_TYPE::SCRIPT == _Component->GetType())
	{
		m_vecScript.push_back((CScript*)_Component);
	}

	// 스크립트를 제외한 일반 컴포넌트인 경우
	else
	{		
		// 이미 보유하고 있는 컴포넌트인 경우
		if (m_arrCom[(UINT)_Component->GetType()])
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"이미 동일한 Component가 존재합니다.");
			MessageBox(nullptr, szStr, L"Component 추가 실패.", MB_OK);
			delete _Component;
		}
		else {
			// RenderComponent 확인
			if (COMPONENT_TYPE::MESHRENDER <= _Component->GetType()
				&& _Component->GetType() <= COMPONENT_TYPE::DECAL)
			{
				if (m_RenderCom) {
					wchar_t szStr[256] = {};
					wsprintf(szStr, L"이미 RenderComponent가 존재합니다.");
					MessageBox(nullptr, szStr, L"RenderComponent 추가 실패.", MB_OK);
					delete _Component;
				}
				else {
					m_arrCom[(UINT)_Component->GetType()] = _Component;
					m_RenderCom = (CRenderComponent*)_Component;
				}
			}
			else {
				m_arrCom[(UINT)_Component->GetType()] = _Component;
			}
		}
	}
}

void CGameObject::AddChild(CGameObject* _Object)
{
	if (_Object->m_Parent)
	{
		// 기존 부모가 있으면 연결 해제 후 연결
		_Object->DisconnectFromParent();
	}
	
	else
	{
		// 기존 부모가 없으면, 소속 레이어에서 최상위부모 목록에서 제거된 후 연결
		_Object->ChangeToChildType();
	}
	

	// 부모 자식 연결
	_Object->m_Parent = this;
	if (_Object->GetLayerIndex() == -1) { _Object->m_iLayerIdx = m_iLayerIdx; }
	m_vecChild.push_back(_Object);
}

void CGameObject::ChangeLayer(int _NewLayerIdx)
{
	tEvent evn = {};

	evn.Type = EVENT_TYPE::LAYER_CHANGE;
	evn.wParam = (DWORD_PTR) this;			// 레이어 바꿀 오브젝트
	evn.lParam = (DWORD_PTR)_NewLayerIdx;	// 바꿀 레이어 번호

	CEventMgr::GetInst()->AddEvent(evn);
}

void CGameObject::ChangeLayer(const wstring& _LayerName)
{
	tEvent evn = {};

	int layeridx = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_LayerName)->GetLayerIndex();

	evn.Type = EVENT_TYPE::LAYER_CHANGE;
	evn.wParam = (DWORD_PTR)this;			// 레이어 바꿀 오브젝트
	evn.lParam = (DWORD_PTR)layeridx;	// 바꿀 레이어 번호

	CEventMgr::GetInst()->AddEvent(evn);
}

CGameObject* CGameObject::FindChildObjByName(wstring _name)
{
	// 해당 오브젝트의 자식 벡터를 가져와서 이름으로 검색한다.
	const vector<CGameObject*>& vecChild = m_vecChild;
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		// 가장 먼저 찾아진 오브젝트 한 개를 반환한다.
		if (_name == m_vecChild[i]->GetName())
			return m_vecChild[i];
	}

	return nullptr;
}


bool CGameObject::IsAncestor(CGameObject* _Target)
{
	CGameObject* pParent = m_Parent;
	while (pParent)
	{
		if (pParent == _Target)
		{
			return true;
		}
		pParent = pParent->m_Parent;
	}

	return false;
}

void CGameObject::DisconnectFromParent()
{
	if (!m_Parent)
		return;

	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (this == *iter)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void CGameObject::ChangeToChildType()
{
	assert(-1 <= m_iLayerIdx && m_iLayerIdx < MAX_LAYER);

	if (-1 != m_iLayerIdx)
	{
		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
		pLayer->RemoveFromParentList(this);
	}
}

void CGameObject::AddParentList()
{
	CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pLayer->AddParentList(this);
}
