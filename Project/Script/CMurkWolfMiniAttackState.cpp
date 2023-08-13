#include "pch.h"
#include "CMurkWolfMiniAttackState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniAttackState::CMurkWolfMiniAttackState()
	: m_iAttackAnimNum(1)
{
}

CMurkWolfMiniAttackState::~CMurkWolfMiniAttackState()
{
}

void CMurkWolfMiniAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 2) m_iAttackAnimNum = 1;

	wstring basename = L"MurkWolf_Mini\\sru_murkwolf_attack";
	if (m_iAttackAnimNum == 1) {
		basename += std::to_wstring(2);
		Vec3 MurkWolfMiniPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\murkwolf_mini\\attack.mp3", 1, 0.5f, true, 200.f, MurkWolfMiniPos, Faction::NONE);
	}
	else if (m_iAttackAnimNum == 2) {
		basename += L"3.002";
		Vec3 MurkWolfMiniPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\murkwolf_mini\\attack2.mp3", 1, 0.5f, true, 200.f, MurkWolfMiniPos, Faction::NONE);
	}

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	CJungleAttackState::Enter();
}

void CMurkWolfMiniAttackState::tick()
{
	CJungleAttackState::tick();
}


void CMurkWolfMiniAttackState::Exit()
{
	CJungleAttackState::Exit();
}

