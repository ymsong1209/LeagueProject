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


	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Spell4";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_spell4";

	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	// 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 마우스 방향
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();
	CGameObject* Map = CUR_LEVEL->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;	// 마우스 좌표
	// 두 좌표 사이의 방향을 구함
	Vec3 MyPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 MouseDir = (TargetPos - MyPos).Normalize();

	// 플레이어 공격 방향으로 회전
	float targetYaw = atan2f(-MouseDir.x, -MouseDir.z);
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

	float lerpFactor = DT * 18.f;

	// Lerp를 이용해 현재 회전 각도와 목표 회전 각도를 보간
	float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;

	// 새로운 회전 각도를 적용
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
