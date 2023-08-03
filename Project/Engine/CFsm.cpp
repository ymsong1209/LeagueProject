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
	//_other.m_mapAnim�� ��ü���� �ݺ����� ���� ���� �����Ͽ� ������ �ʿ� �־��ش�.
	map<wstring, CState*>::const_iterator iter = _other.m_mapState.begin();
	for (; iter != _other.m_mapState.end(); iter++)
	{
		//State Clone
		CState* pState = iter->second->Clone();
		//������ �ִϸ��̼� �ʿ� ���� ���� �ִϸ��̼� ����
		m_mapState.insert(make_pair(iter->first, pState));
	}

	//���簡 ������ ���� State�� ���� State�� ����д�.
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
	// FSM�� ������ ���� State�� �����ϰ�, ���ุ �����ش�
	if (m_pCurState == nullptr)
		return;

	m_pCurState->tick();
}

void CFsm::AddState(const wstring& _strName, CState* _pState)
{
	//�ش� name�� state�� �̹� ����
	if (FindState(_strName)) {
		delete _pState;
		return;
	}

	// State �ʿ� �ش� State�� �߰��Ѵ�
	m_mapState.insert(make_pair(_strName, _pState));

	// �ش� state�� ���� FSM�� �������� �����Ѵ�.
	_pState->m_pOwnerFSM = this;

	// �ش� State�� �̸��� �����Ѵ�.
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
	assert(pNextState);		// �ش� State�� ���� �� Assert
	
	// ���� state�� �ι� �̻� ��������. ���ó��
	if (m_pCurState != nullptr && m_pCurState == pNextState) {
		return;
	}

	// ���� ������ Exit ȣ��
	if (m_pCurState != nullptr)
		m_pCurState->Exit();

	// ���� ���¸� �ٲ� ���·� ġȯ
	m_pCurState = pNextState;

	// ���� ���� Enter ȣ��
	m_pCurState->Enter();
}
