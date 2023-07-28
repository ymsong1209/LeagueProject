#include "pch.h"
#include "CTurretScript.h"
#include <Engine\CAnim3D.h>
#include <Engine\CAnimator3D.h>

CTurretScript::CTurretScript()
	:CStructureScript((UINT)SCRIPT_TYPE::TURRETSCRIPT)
{
	m_fAttackPower = 50;
	m_fDefencePower = 100;
	m_fAttackSpeed = 5;
	m_fAttackRange = 300;
	m_fMoveSpeed = 0;
}

CTurretScript::~CTurretScript()
{
}

void CTurretScript::begin()
{
	CStructureScript::begin();
}

void CTurretScript::tick()
{
	CStructureScript::tick();

	CheckStatus();
	ChangeAnim();

	if (m_bUnitDead)
		return;

	/*
	if(공격중이 아니라면)
	{
		if(공격 타겟이 설정되어있지 않다면)
		{
			if(사거리 내부에 들어온 적이 있다면)
			{
				// FindTarget()
				// 정해진 순서에 따라 공격할 타겟 설정
				ex. 1순위: 아군 챔피언에게 피해를 입힌 적 챔피언
				ex. 2순위: 미니언(슈미-대포-원거리-근거리 순)
				// m_iTargetID = 해당 오브젝트의 ID
			}
		}
		else
		{
			// 공격 타겟이 설정되어있다면,
			// 해당 타겟이 여전히 포탑 사거리 내에 있는지 검사
			// if(IsTargetInRange(UINT ID))
			// 공격 타겟 그대로
			// else (해당 타겟이 포탑 사거리 밖이거나, 죽었다면)
			// FindTargetT()
		}

			//최종적으로 타겟이 있는가?
				공격하기
				공격 이벤트 발생

			//타겟이 없는가?
				공격 안함
				
		}
	}
	else	// 공격 중이라면
	{
		
	}

	// HP가 0 이하라면 죽음
	// m_bDead = true;
	// 죽음 이벤트 알림

	*/
}

void CTurretScript::ChangeAnim()
{
	// 남은 체력에 따라 애니메이션 바꿔줌

	float HealthRatio = m_fHP / m_fMaxHP;
	if (HealthRatio >= 0.66f)
	{
		//if(Animator3D()->GetCurAnim()->GetName() !=L"포탑Idle애니메이션이름" )
			// 해당 애니메이션 반복재생. (Idle)
	}
	else if (33.f < HealthRatio && HealthRatio <= 0.66f)
	{
		//if (Animator3D()->GetCurAnim()->GetName() != L"포탑break1애니메이션이름" )
			// 해당 애니메이션 반복재생. (break1)
	}
	else if (0 < HealthRatio && HealthRatio <= 33.f)
	{
		//if (Animator3D()->GetCurAnim()->GetName() != L"포탑break2애니메이션이름" )
			// 해당 애니메이션 반복재생. (break2)
	}
	else
	{
		// 포탑 Dead 상태
		// 시야 제공 기능 삭제
		// 더이상 상호작용 가능한 오브젝트가 아님
	}


}

void CTurretScript::CheckStatus()
{
	// 공격 가능한지 여부 체크

	// 포탑이 파괴되었다면 무조건 공격 불가
	if (m_bUnitDead)
	{
		m_bAttackable = false;
		return;
	}

	if (m_iTurretNumber == 1)	// 1차 포탑의 경우 조건 없음
		m_bAttackable = true;
	else if (m_iTurretNumber == 2) // 2차 포탑
	{
		// 터렛 레이어에서, 같은 Lane의 1차 포탑이 파괴된 경우에만 공격 가능
	}
	else if (m_iTurretNumber == 3)	// 3차(억제기 앞 포탑)
	{
		// 터렛 레이어에서, 같은 Lane의 2차 포탑이 파괴된 경우에만 공격 가능
	}
	else                            // 4차포탑(넥서스 앞 쌍둥이타워)
	{
		// Lane에 상관 없이, 억제기가 하나라도 파괴된 상태라면 공격 가능
	}
}

bool CTurretScript::IsEnemyInRange(UINT _unitID)
{
	// 오브젝트가 사거리 내에 있는지 감지
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CGameObject* Unit = CurLevel->FindObjectByID(_unitID);
	Vec2 UnitPos = Vec2(Unit->Transform()->GetRelativePos().x, Unit->Transform()->GetRelativePos().z);

	Vec2 TurretPos = Vec2(Transform()->GetRelativePos().x, Transform()->GetRelativePos().z);

	float distance = sqrt(pow(TurretPos.x - UnitPos.x, 2) + pow(TurretPos.y - UnitPos.y, 2));

	if (distance <= m_fAttackRange)
		return true;
	else
		return false;
}



/* 어떤 것을 본체(부모, 이 스크립트가 붙을 객체)로 할 것인가?

1. IdleBreak

장점1: 마우스 오버레이시 포탑 전체에 테두리쳐짐
장점2: idle, break1, break2 자체를 가지고 있어서 바꿔주기 쉬움

단점1: 포탑이 죽고 난 뒤에 처리?
	   스스로를(이 스크립트 포함) 그냥 삭제해버리기? (마치 맵의 일부처럼 그냥 모델만 남음)

	   아니면 안보이기?
	   이 경우 클릭 불가능하게 처리가 가능할까?


2. Rubble

장점1: 모든 상태에 Rubble이 있어도 됨으로 끝까지 포탑 로직을 책임질수 있음
	   포탑 죽으면 자식들의 애님이 끝나고  전부 다 밀어버려도 됨

단점1: 본인이 Anim을 가진게 아니기 때문에, 자식의 Anim을 돌려줘야함. 약간 불편
단점2: 마우스 오버레이시 포탑 전체가 아니라 잔해에만 외곽선 나타날수도 있음


*/