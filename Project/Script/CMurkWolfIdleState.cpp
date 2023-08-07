#include "pch.h"
#include "CMurkWolfIdleState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfScript.h"
CMurkWolfIdleState::CMurkWolfIdleState()
	:m_iIdleAnimNum(1)
{
}

CMurkWolfIdleState::~CMurkWolfIdleState()
{
}

void CMurkWolfIdleState::Enter()
{
	CJungleIdleState::Enter();
	m_iIdleAnimNum = 1;
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_idle1_model.002");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\sru_murkwolf_idle1_model.002", false, false, false, 0.f);
}

void CMurkWolfIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 5) m_iIdleAnimNum = 1;
		wstring basestring = L"MurkWolf\\sru_murkwolf_idle";
		if (m_iIdleAnimNum == 1) {
			basestring = L"MurkWolf\\sru_murkwolf_idle1_model.002";
		}
		else {
			basestring += std::to_wstring(m_iIdleAnimNum);
		}
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basestring);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basestring, false, false, false, 0.f);
	}
	CJungleIdleState::tick();
}

void CMurkWolfIdleState::Exit()
{
	m_iIdleAnimNum = 1;
	CJungleIdleState::Exit();
}
