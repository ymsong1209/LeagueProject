#include "pch.h"
#include "CJinxIdleState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"
CJinxIdleState::CJinxIdleState()
{
	SetName(L"Idle");
}

CJinxIdleState::~CJinxIdleState()
{
}

void CJinxIdleState::tick()
{
	// SFX / Voice 재생
}

void CJinxIdleState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true, 0.1f);

	// server에 보낼 애니메이션 패킷 이벤트 등록
	AnimInfo* animInfo = new AnimInfo();
	animInfo->animIdx = GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetServerID();
	animInfo->animName = L"Jinx\\Idle1_Base";
	animInfo->blend = true;
	animInfo->blendTime = 0.1f;

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SEND_ANIM_PACKET;
	//evn.wParam = (DWORD_PTR);
	evn.lParam = (DWORD_PTR)animInfo;
	CSendServerEventMgr::GetInst()->AddServerSendEvent(evn);

	CChampionIdleState::Enter();
}

void CJinxIdleState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Jinx\\Idle1_Base")->Reset();
	CChampionIdleState::Exit();
}

void CJinxIdleState::HandleEvent(CGameEvent& event)
{
	CChampionIdleState::HandleEvent(event);
}
