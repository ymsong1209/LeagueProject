#include "pch.h"
#include "CKrugAttackState.h"
#include <Engine/CAnim3D.h>
#include "CKrugScript.h"
CKrugAttackState::CKrugAttackState()
	:m_iAttackAnimNum(1)
{
}

CKrugAttackState::~CKrugAttackState()
{
}

void CKrugAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 5) m_iAttackAnimNum = 1;

	wstring basename = L"Krug\\krug_attack";
	basename += std::to_wstring(m_iAttackAnimNum);

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);

	Vec3 KrugPos = GetOwner()->Transform()->GetRelativePos();
	if (m_iAttackAnimNum % 2 == 1) {
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\krug\\attack.mp3", 1, 0.5f, true, 200.f, KrugPos, Faction::NONE);
	}
	else {
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\krug\\attack2.mp3", 1, 0.5f, true, 200.f, KrugPos, Faction::NONE);
	}


	CJungleAttackState::Enter();
}

void CKrugAttackState::tick()
{
	CJungleAttackState::tick();
}


void CKrugAttackState::Exit()
{
	CJungleAttackState::Exit();
}



