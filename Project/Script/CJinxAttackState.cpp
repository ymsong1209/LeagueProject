#include "pch.h"
#include "CJinxAttackState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>
#include "CBasicAttackScript.h"



CJinxAttackState::CJinxAttackState()
{
}

CJinxAttackState::~CJinxAttackState()
{
}

void CJinxAttackState::tick()
{
	CChampionAttackState::tick();
}

void CJinxAttackState::Enter()
{
	// 애니메이션
	wstring animName = L"Jinx\\Attack1";
	GetOwner()->Animator3D()->PlayOnce(animName, false);
	SendAnimPacket(animName, false, false, 0.f);







	CChampionAttackState::Enter();
}

void CJinxAttackState::Exit()
{
}

void CJinxAttackState::HandleEvent(CGameEvent& event)
{
}
