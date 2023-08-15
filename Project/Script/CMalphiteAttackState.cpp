#include "pch.h"
#include "CMalphiteAttackState.h"


#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>
#include "CBasicAttackScript.h"

#include "CMalphiteScript.h"


CMalphiteAttackState::CMalphiteAttackState()
	:m_iAttackAnimNum(1)

{
}

CMalphiteAttackState::~CMalphiteAttackState()
{
}

void CMalphiteAttackState::tick()
{
	CChampionAttackState::tick();
}

void CMalphiteAttackState::Enter()
{
	// 애니메이션
	wstring basename = L"Malphite\\Attack";

	CMalphiteScript* chamscript = GetOwner()->GetScript<CMalphiteScript>();

	if (chamscript->GetWActive()) {
		basename = L"Malphite\\malphite_attack3";
	}
	else {
		float time = chamscript->GetAttackAnimCoolTime();
		if (time < 2.f) {
			basename += std::to_wstring(m_iAttackAnimNum);
			++m_iAttackAnimNum;
			if (m_iAttackAnimNum > 2) m_iAttackAnimNum = 1;
		}
		else {
			chamscript->SetAttackAnimCoolTime(0);
			m_iAttackAnimNum = 1;
			basename += std::to_wstring(m_iAttackAnimNum);
		}
	}

	
	GetOwner()->Animator3D()->PlayOnce(basename, false);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, true, false, 0.0f);


	if (m_iMalphiteSoundNum == 1) {
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\malphite\\Malph_BasicAttack1.mp3", 1, 0.5f, true, 100.f, vPos, Faction::NONE);
		m_iMalphiteSoundNum = 2;
	}
	else {
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\malphite\\Malph_BasicAttack2.mp3", 1, 0.5f, true, 100.f, vPos, Faction::NONE);
		m_iMalphiteSoundNum = 1;
	}



	CChampionAttackState::Enter();
}

void CMalphiteAttackState::Exit()
{
	CChampionAttackState::Exit();
}

void CMalphiteAttackState::HandleEvent(CGameEvent& event)
{
	CChampionAttackState::HandleEvent(event);
}
