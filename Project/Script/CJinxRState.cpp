#include "pch.h"
#include "CJinxRState.h"
#include "CChampionScript.h"
#include "CSkill.h"
#include "CJinxScript.h"
#include <thread>
#include <Engine/CRenderMgr.h>

CJinxRState::CJinxRState()
{
	SetName(L"R");
}

CJinxRState::~CJinxRState()
{
}

void CJinxRState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxR = GetOwner()->GetScript<CChampionScript>()->GetSkill(4);
	JinxR->SetUserObj(m_UserObj);
	JinxR->SetTargetObj(m_TargetObj);
	JinxR->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	thread t([=]() {
		Sleep(1000);
	JinxR->Use();
		});
	t.detach();


	// �ִϸ��̼�
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Spell4";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_spell4";

	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	// ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// ���콺 ����
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();
	CGameObject* Map = CUR_LEVEL->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;	// ���콺 ��ǥ
	// �� ��ǥ ������ ������ ����
	Vec3 MyPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 MouseDir = (TargetPos - MyPos).Normalize();

	// �÷��̾� ���� �������� ȸ��
	float targetYaw = atan2f(-MouseDir.x, -MouseDir.z);
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

	float lerpFactor = DT * 18.f;

	// Lerp�� �̿��� ���� ȸ�� ������ ��ǥ ȸ�� ������ ����
	float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;

	// ���ο� ȸ�� ������ ����
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));
}

void CJinxRState::tick()
{

	CChampionSkillState::tick();
}



void CJinxRState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxRState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
