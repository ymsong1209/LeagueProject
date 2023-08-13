#include "pch.h"
#include "CInhibitorScript.h"

CInhibitorScript::CInhibitorScript()
	:CStructureScript((UINT)SCRIPT_TYPE::INHIBITORSCRIPT)
{
}

CInhibitorScript::~CInhibitorScript()
{
}

void CInhibitorScript::begin()
{
	CStructureScript::begin();

	// FSM
	if (GetOwner()->Fsm() == nullptr)
		return;
	GetOwner()->Fsm()->ChangeState(L"Spawn");
}

void CInhibitorScript::tick()
{
}

void CInhibitorScript::CheckStatus()
{
	// ���� �������� ���� üũ

	// ��ž ���̾�� ��� ��ž ������Ʈ�� �����ͼ�
	// ���� Lane�� ��� ��ž�� Dead �������� Ȯ��
}
