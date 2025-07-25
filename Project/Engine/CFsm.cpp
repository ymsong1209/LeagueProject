#include "pch.h"
#include "CFsm.h"
#include "CState.h"

CFsm::CFsm()
	: CComponent(COMPONENT_TYPE::FSM)
	, m_pCurState(nullptr)
{
}

CFsm::CFsm(const CFsm& _other)
	: CComponent(_other)
	, m_pCurState(nullptr)
{
	//_other.m_mapAnim의 객체들을 반복문을 돌며 깊은 복사하여 복사할 맵에 넣어준다.
	map<wstring, CState*>::const_iterator iter = _other.m_mapState.begin();
	for (; iter != _other.m_mapState.end(); iter++)
	{
		//State Clone
		CState* pState = iter->second->Clone();
		//복사한 애니메이션 맵에 새로 만든 애니메이션 삽입
		m_mapState.insert(make_pair(iter->first, pState));
	}

	//복사가 끝나면 원본 State와 같은 State로 맞춰둔다.
	if (_other.m_pCurState) {
		m_pCurState = m_mapState.find(_other.m_pCurState->GetName())->second;
	}
}

CFsm::~CFsm()
{
	Safe_Del_Map(m_mapState);
}

void CFsm::tick()
{
	// FSM은 본인이 가진 State를 관리하고, 실행만 시켜준다
	if (m_pCurState == nullptr)
		return;

	m_pCurState->tick();
}

void CFsm::AddState(const wstring& _strName, CState* _pState)
{
	//해당 name의 state가 이미 있음
	if (FindState(_strName)) {
		delete _pState;
		return;
	}

	// State 맵에 해당 State를 추가한다
	m_mapState.insert(make_pair(_strName, _pState));

	// 해당 state의 소유 FSM을 본인으로 설정한다.
	_pState->m_pOwnerFSM = this;

	// 해당 State의 이름을 저장한다.
	_pState->SetName(_strName);
}

CState* CFsm::FindState(const wstring& _strName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_strName);
	if (iter != m_mapState.end())
		return iter->second;

	return nullptr;
}

void CFsm::ChangeState(const wstring& _strStateName)
{
	CState* pNextState = FindState(_strStateName);
	assert(pNextState);		// 해당 State가 없을 시 Assert
	
	// 같은 state로 두번 이상 들어오려함. 방어처리
	if (m_pCurState != nullptr && m_pCurState == pNextState) {
		return;
	}

	// 현재 상태의 Exit 호출
	if (m_pCurState != nullptr)
		m_pCurState->Exit();

	// 현재 상태를 바꿀 상태로 치환
	m_pCurState = pNextState;

	// 다음 상태 Enter 호출
	m_pCurState->Enter();
}
