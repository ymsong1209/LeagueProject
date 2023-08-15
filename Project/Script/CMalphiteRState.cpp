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

	// 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 가야할 방향 구하기
	Vec3 Dir = (m_vTargetPos - CurPos).Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	targetYaw = fmod(targetYaw + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기
	float currentYaw = GetOwner()->Transform()->GetRelativeRot().y;
	currentYaw = fmod(currentYaw + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기

	// 각도 차이 계산
	float diff = targetYaw - currentYaw;

	// 차이가 π를 넘으면 각도를 반대 방향으로 보간
	if (diff > XM_PI)
		targetYaw -= 2 * XM_PI;
	else if (diff < -XM_PI)
		targetYaw += 2 * XM_PI;

	float lerpFactor = EditorDT * 0.f;

	// Lerp를 이용해 현재 회전 각도와 목표 회전 각도를 보간
	float newYaw = targetYaw; //보간 적용 x
	//float newYaw = currentYaw + (targetYaw - currentYaw);

	// 새로운 회전 각도를 적용
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));
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
