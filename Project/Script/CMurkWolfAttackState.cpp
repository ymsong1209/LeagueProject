#include "pch.h"
#include "CMurkWolfAttackState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfScript.h"
CMurkWolfAttackState::CMurkWolfAttackState()
	: m_iAttackAnimNum(1)
{
}

CMurkWolfAttackState::~CMurkWolfAttackState()
{
}

void CMurkWolfAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 2) m_iAttackAnimNum = 1;

	wstring basename = L"MurkWolf\\sru_murkwolf_attack";
	if (m_iAttackAnimNum == 1) {
		basename += std::to_wstring(1);
		Vec3 MurkWolfPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\murkwolf\\attack1.mp3", 1, 0.5f, true, 120.f, MurkWolfPos, Faction::NONE);

	}
	else if (m_iAttackAnimNum == 2) {
		basename += std::to_wstring(4);
		Vec3 MurkWolfPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\murkwolf\\attack2.mp3", 1, 0.5f, true, 120.f, MurkWolfPos, Faction::NONE);
	}

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	CJungleAttackState::Enter();
}

void CMurkWolfAttackState::tick()
{
	CJungleAttackState::tick();
}


void CMurkWolfAttackState::Exit()
{
	CJungleAttackState::Exit();
}



