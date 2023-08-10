#include "pch.h"
#include "CMalphiteRState.h"
#include "CChampionScript.h"
#include "CSkill.h"

CMalphiteRState::CMalphiteRState()
	:m_fSpeed(400.f)
{
	SetName(L"R");
}

CMalphiteRState::~CMalphiteRState()
{
}


void CMalphiteRState::Enter()
{
	CChampionSkillState::Enter();

	

	

	// 애니메이션
	wstring animName = L"Malphite\\Spell4";
	GetOwner()->Animator3D()->PlayRepeat(animName, true, 0.1f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, true, true, 0.1f);

	
}

void CMalphiteRState::tick()
{

	Vec3 MalphPos = GetOwner()->Transform()->GetRelativePos();

	float length = sqrt(pow(MalphPos.x - m_vTargetPos.x, 2.f) + pow(MalphPos.z - m_vTargetPos.z, 2.f));
	if (length < 5.f) {

		CSkill* MalphiteR = GetOwner()->GetScript<CChampionScript>()->GetSkill(4);
		MalphiteR->SetUserObj(m_UserObj);
		MalphiteR->SetTargetObj(nullptr);
		MalphiteR->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());
		MalphiteR->Use();

		GetOwner()->Fsm()->ChangeState(L"Idle");
		return;
	}

	// 방향 계산
	Vec3 Direction = Vec3(m_vTargetPos.x - MalphPos.x, 0.f, m_vTargetPos.z - MalphPos.z);
	Direction.Normalize();

	// 투사체 이동
	Vec3 NewPos = MalphPos + Direction * m_fSpeed * EditorDT;

	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CMalphiteRState::Exit()
{
	CChampionSkillState::Exit();
}

void CMalphiteRState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
