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
	if(�������� �ƴ϶��)
	{
		if(���� Ÿ���� �����Ǿ����� �ʴٸ�)
		{
			if(��Ÿ� ���ο� ���� ���� �ִٸ�)
			{
				// FindTarget()
				// ������ ������ ���� ������ Ÿ�� ����
				ex. 1����: �Ʊ� è�Ǿ𿡰� ���ظ� ���� �� è�Ǿ�
				ex. 2����: �̴Ͼ�(����-����-���Ÿ�-�ٰŸ� ��)
				// m_iTargetID = �ش� ������Ʈ�� ID
			}
		}
		else
		{
			// ���� Ÿ���� �����Ǿ��ִٸ�,
			// �ش� Ÿ���� ������ ��ž ��Ÿ� ���� �ִ��� �˻�
			// if(IsTargetInRange(UINT ID))
			// ���� Ÿ�� �״��
			// else (�ش� Ÿ���� ��ž ��Ÿ� ���̰ų�, �׾��ٸ�)
			// FindTargetT()
		}

			//���������� Ÿ���� �ִ°�?
				�����ϱ�
				���� �̺�Ʈ �߻�

			//Ÿ���� ���°�?
				���� ����
				
		}
	}
	else	// ���� ���̶��
	{
		
	}

	// HP�� 0 ���϶�� ����
	// m_bDead = true;
	// ���� �̺�Ʈ �˸�

	*/
}

void CTurretScript::ChangeAnim()
{
	// ���� ü�¿� ���� �ִϸ��̼� �ٲ���

	float HealthRatio = m_fHP / m_fMaxHP;
	if (HealthRatio >= 0.66f)
	{
		//if(Animator3D()->GetCurAnim()->GetName() !=L"��žIdle�ִϸ��̼��̸�" )
			// �ش� �ִϸ��̼� �ݺ����. (Idle)
	}
	else if (33.f < HealthRatio && HealthRatio <= 0.66f)
	{
		//if (Animator3D()->GetCurAnim()->GetName() != L"��žbreak1�ִϸ��̼��̸�" )
			// �ش� �ִϸ��̼� �ݺ����. (break1)
	}
	else if (0 < HealthRatio && HealthRatio <= 33.f)
	{
		//if (Animator3D()->GetCurAnim()->GetName() != L"��žbreak2�ִϸ��̼��̸�" )
			// �ش� �ִϸ��̼� �ݺ����. (break2)
	}
	else
	{
		// ��ž Dead ����
		// �þ� ���� ��� ����
		// ���̻� ��ȣ�ۿ� ������ ������Ʈ�� �ƴ�
	}


}

void CTurretScript::CheckStatus()
{
	// ���� �������� ���� üũ

	// ��ž�� �ı��Ǿ��ٸ� ������ ���� �Ұ�
	if (m_bUnitDead)
	{
		m_bAttackable = false;
		return;
	}

	if (m_iTurretNumber == 1)	// 1�� ��ž�� ��� ���� ����
		m_bAttackable = true;
	else if (m_iTurretNumber == 2) // 2�� ��ž
	{
		// �ͷ� ���̾��, ���� Lane�� 1�� ��ž�� �ı��� ��쿡�� ���� ����
	}
	else if (m_iTurretNumber == 3)	// 3��(������ �� ��ž)
	{
		// �ͷ� ���̾��, ���� Lane�� 2�� ��ž�� �ı��� ��쿡�� ���� ����
	}
	else                            // 4����ž(�ؼ��� �� �ֵ���Ÿ��)
	{
		// Lane�� ��� ����, �����Ⱑ �ϳ��� �ı��� ���¶�� ���� ����
	}
}

bool CTurretScript::IsEnemyInRange(UINT _unitID)
{
	// ������Ʈ�� ��Ÿ� ���� �ִ��� ����
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



/* � ���� ��ü(�θ�, �� ��ũ��Ʈ�� ���� ��ü)�� �� ���ΰ�?

1. IdleBreak

����1: ���콺 �������̽� ��ž ��ü�� �׵θ�����
����2: idle, break1, break2 ��ü�� ������ �־ �ٲ��ֱ� ����

����1: ��ž�� �װ� �� �ڿ� ó��?
	   �����θ�(�� ��ũ��Ʈ ����) �׳� �����ع�����? (��ġ ���� �Ϻ�ó�� �׳� �𵨸� ����)

	   �ƴϸ� �Ⱥ��̱�?
	   �� ��� Ŭ�� �Ұ����ϰ� ó���� �����ұ�?


2. Rubble

����1: ��� ���¿� Rubble�� �־ ������ ������ ��ž ������ å������ ����
	   ��ž ������ �ڽĵ��� �ִ��� ������  ���� �� �о������ ��

����1: ������ Anim�� ������ �ƴϱ� ������, �ڽ��� Anim�� ���������. �ణ ����
����2: ���콺 �������̽� ��ž ��ü�� �ƴ϶� ���ؿ��� �ܰ��� ��Ÿ������ ����


*/