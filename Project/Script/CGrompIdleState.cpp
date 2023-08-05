#include "pch.h"
#include "CGrompIdleState.h"
#include <Engine/CAnim3D.h>
#include "CGrompScript.h"
CGrompIdleState::CGrompIdleState()
	:m_iIdleAnimNum(1)
{
}

CGrompIdleState::~CGrompIdleState()
{
}

void CGrompIdleState::Enter()
{
	m_iIdleAnimNum = 1;
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_idle1", false, false, 0.0f);
}

void CGrompIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 6) m_iIdleAnimNum = 1;
		wstring basestring = L"gromp\\_idle";
		basestring += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basestring);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basestring, false, false, 0.0f);
	}
	CJungleIdleState::tick();
}

void CGrompIdleState::Exit()
{
	m_iIdleAnimNum = 1;
	CJungleIdleState::Exit();
}

