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

	// �ִϸ��̼�
	wstring animName = L"Malphite\\Spell4";
	GetOwner()->Animator3D()->PlayRepeat(animName, true, 0.1f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, true, true, 0.1f);

	// ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// ������ ���� ���ϱ�
	Vec3 Dir = (m_vTargetPos - CurPos).Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	targetYaw = fmod(targetYaw + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�
	float currentYaw = GetOwner()->Transform()->GetRelativeRot().y;
	currentYaw = fmod(currentYaw + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�

	// ���� ���� ���
	float diff = targetYaw - currentYaw;

	// ���̰� �� ������ ������ �ݴ� �������� ����
	if (diff > XM_PI)
		targetYaw -= 2 * XM_PI;
	else if (diff < -XM_PI)
		targetYaw += 2 * XM_PI;

	float lerpFactor = EditorDT * 0.f;

	// Lerp�� �̿��� ���� ȸ�� ������ ��ǥ ȸ�� ������ ����
	float newYaw = targetYaw; //���� ���� x
	//float newYaw = currentYaw + (targetYaw - currentYaw);

	// ���ο� ȸ�� ������ ����
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

	// ���� ���
	Vec3 Direction = Vec3(m_vTargetPos.x - MalphPos.x, 0.f, m_vTargetPos.z - MalphPos.z);
	Direction.Normalize();

	// ����ü �̵�
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
