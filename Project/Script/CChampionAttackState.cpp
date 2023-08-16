#include "pch.h"
#include "CChampionAttackState.h"
#include <Engine/CFsm.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>

#include "CChampionScript.h"
#include "CSkill.h"
#include "CSendServerEventMgr.h"

CChampionAttackState::CChampionAttackState()
	: m_UserObj(nullptr)
	, m_TargetObj(nullptr)
{
}

CChampionAttackState::~CChampionAttackState()
{
}

void CChampionAttackState::tick()
{
	// �ִϸ��̼��� ���� ���, Idle�� ��ȯ
	if(GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");

	// è�Ǿ��� ��Ÿ�� �̵� ���� 
	GetOwner()->PathFinder()->ClearPath();
	GetOwner()->Transform()->SetRelativeRot(m_EnterRot);
}

void CChampionAttackState::Enter()
{
	/*
	// ==== �������� ��Ÿ �̺�Ʈ Send  ==== //
	SkillInfo* skillInfo = new SkillInfo;
	//skillInfo->SkillId = ���� �� �� ����
	skillInfo->OwnerId = m_iUserObj->GetScript<CUnitScript>()->GetServerID();	 // ���� 
	skillInfo->TargetId = m_iTargetObj->GetScript<CUnitScript>()->GetServerID(); //	�ǰ���
	skillInfo->SkillLevel = 1;	// ��Ÿ�� �ǹ� ����, ������ 1 
	//skillInfo->SkillLevel = m_iUserObj->GetScript<CChampionScript>()->GetSkillLevel(0);
	skillInfo->skillType = SkillType::BASIC_ATTACK;
	//skillInfo->offsetPos = Vec3(0,0,0); ���� Bone ������� Ȱ���� ���� state���� ���� 

	tServerEvent serverEvn = {};
	serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
	serverEvn.wParam = (DWORD_PTR)skillInfo;
	//serverEvn.lParam 
	CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	*/
	CSkill* BasicAttack = GetOwner()->GetScript<CChampionScript>()->GetSkill(0);
	BasicAttack->SetUserObj(m_UserObj);
	BasicAttack->SetTargetObj(m_TargetObj);
	BasicAttack->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());
	
	BasicAttack->Use();

	// �������� �⺻ ���� ����Ѵٴ� ��ȣ ����
	//CSendServerEventMgr::GetInst()->SendUseSkillPacket(
	//	m_UserObj->GetScript<CUnitScript>()->GetServerID(),
	//	m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
	//	1,
	//	SkillType::BASIC_ATTACK, 
	//	Vec3(0, 0, 0), 
	//	false,
	//	Vec3(0, 0, 0),
	//	false,
	//	Vec3(0, 0, 0));

	// ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	m_EnterRot = GetOwner()->Transform()->GetRelativeRot();

	// ������ ���� ���ϱ�
	Vec3 Dir = (m_TargetObj->Transform()->GetRelativePos() - CurPos).Normalize();

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

	float lerpFactor = EditorDT * 18.f;

	// Lerp�� �̿��� ���� ȸ�� ������ ��ǥ ȸ�� ������ ����
	float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;

	// ���ο� ȸ�� ������ ����
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));


	CUnitState::Enter();
}

void CChampionAttackState::Exit()
{
	// ���� �ʱ�ȭ
	//m_UserObj = nullptr;
	m_TargetObj = nullptr;

	CUnitState::Exit();
}

void CChampionAttackState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;
	case GAME_EVENT_TYPE::GET_HIT:
	{
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// ���� Ÿ���� ������ ��쿡�� �̺�Ʈ�� ����
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;
	}
}
