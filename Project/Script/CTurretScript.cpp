#include "pch.h"
#include "CTurretScript.h"
#include <Engine\CAnim3D.h>
#include <Engine\CAnimator3D.h>
#include "CChampionScript.h"
#include "CTurretAttackScript.h"
#include "CBasicAttack.h"

CTurretScript::CTurretScript()
	:CStructureScript((UINT)SCRIPT_TYPE::TURRETSCRIPT)
	, m_fAttackCoolTime(3.0f)
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
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	//m_Skill[0]->SetProjectileObj(); // ����ü ������ ����
	//m_Skill[0]->
	//m_SkillLevel[0] = 1;

	CStructureScript::begin();
}

void CTurretScript::tick()
{
	CStructureScript::tick();

	CheckStatus();
	ChangeAnim();

	if (m_bUnitDead)
		return;

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();

	//��ž ���� ��Ÿ��
	if (m_fAttackCoolTime > 0.f) {
		//������ ���߿� DT�� �ٲ����
		m_fAttackCoolTime -= EditorDT;
	}
	else {
		//Ÿ���� �ִٸ�
		if (m_pTarget) {
			//Ÿ���� �̴Ͼ��̸�, ��ž���̺� �ϴ� è�Ǿ� �ֳ� Ȯ���ؾ���
			if (m_pTarget->GetLayerIndex() == CurLevel->FindLayerByName(L"Minion")->GetLayerIndex()) {
				for (size_t i = 0; i < m_vecCollidingChampion.size(); ++i) {
					CGameObject* Champ = m_vecCollidingChampion[i];
					CChampionScript* Chamscript = m_vecCollidingChampion[i]->GetScript<CChampionScript>();
					if (!Champ->IsDead() && Chamscript->IsAttackingChampion()) {
						m_pTarget = m_vecCollidingChampion[i];
						//�����ؾ���
						m_fAttackCoolTime = 0.f;
						break;
					}
				}
			}

		}
		//Ÿ���� ���ٸ� Ÿ�� ����, ��, �̴Ͼ� ����
		else {
			if (m_vecCollidingMinion.empty()) {
				if (!m_vecCollidingChampion.empty()) {
					m_pTarget = m_vecCollidingChampion[0];
				}
			}
			else {
				m_pTarget = m_vecCollidingMinion[0];
			}
		}


		//Ÿ�� Ȯ������ ����
		if (m_pTarget) {
			//���߿��� prefab�� ��ȯ�ؾ���
			//CGameObject* TurretAttack = CResMgr::GetInst()->FindRes<CPrefab>(L"TurretAttack")->Instantiate();
			CGameObject* TurretAttack = new CGameObject;
			TurretAttack->SetName(L"TurretAttack");
			TurretAttack->AddComponent(new CTransform);
			TurretAttack->AddComponent(new CTurretAttackScript);
			TurretAttack->AddComponent(new CMeshRender);
			TurretAttack->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 20.f));
			TurretAttack->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
			TurretAttack->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
			Vec3 TowerPos = GetOwner()->Transform()->GetRelativePos();
			CTurretAttackScript* script = TurretAttack->GetScript<CTurretAttackScript>();
			//��ž ������ target����
			script->SetTarget(m_pTarget);
			SpawnGameObject(TurretAttack, Vec3(TowerPos.x, TowerPos.y + 50.f, TowerPos.z), L"Projectile");
			//���� ��ü ��ȯ�ϰ� ���� ��ž ��Ÿ�� �ʱ�ȭ
			m_fAttackCoolTime = 3.f;
		}
	}

	
	
	


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

void CTurretScript::BeginOverlap(CCollider2D* _Other)
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	if (_Other->GetOwner()->GetLayerIndex() == CurLevel->FindLayerByName(L"Minion")->GetLayerIndex()) {
		m_vecCollidingMinion.push_back(_Other->GetOwner());
		//Faction���� ���� �ȵǾ�����
		//if (GetFaction() != _Other->GetOwner()->GetScript<CUnitScript>()->GetFaction()) {
		//	m_vecCollidingMinion.push_back(_Other->GetOwner());
		//}
	}

	if (_Other->GetOwner()->GetLayerIndex() == CurLevel->FindLayerByName(L"Champion")->GetLayerIndex()) {

		m_vecCollidingChampion.push_back(_Other->GetOwner());
		//Faction�� ���� ���� �ȵǾ�����
		//if (GetFaction() != _Other->GetOwner()->GetScript<CUnitScript>()->GetFaction()) {
		//	m_vecCollidingChampion.push_back(_Other->GetOwner());
		//}
	}
}

void CTurretScript::OnOverlap(CCollider2D* _Other)
{
}

void CTurretScript::EndOverlap(CCollider2D* _Other)
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	if (_Other->GetOwner()->GetLayerIndex() == CurLevel->FindLayerByName(L"Minion")->GetLayerIndex()) {
		if (GetFaction() != _Other->GetOwner()->GetScript<CUnitScript>()->GetFaction()) {
			auto it = find(m_vecCollidingMinion.begin(), m_vecCollidingMinion.end(), _Other->GetOwner());
			if (it != m_vecCollidingMinion.end()) {
				m_vecCollidingMinion.erase(it);
			}
		}

		//��ž ��Ÿ����� �����, Ÿ���̿��� �ִ� �����Ǿ����
		if (m_pTarget == _Other->GetOwner()) {
			m_pTarget = nullptr;
		}
	}

	if (_Other->GetOwner()->GetLayerIndex() == CurLevel->FindLayerByName(L"Champion")->GetLayerIndex()) {

		auto it = find(m_vecCollidingChampion.begin(), m_vecCollidingChampion.end(), _Other->GetOwner());
		if (it != m_vecCollidingChampion.end()) {
			m_vecCollidingChampion.erase(it);
		}


		//faction�� ���� �ȵ�
		/*if (GetFaction() != _Other->GetOwner()->GetScript<CUnitScript>()->GetFaction()) {
			auto it = find(m_vecCollidingChampion.begin(), m_vecCollidingChampion.end(), _Other->GetOwner());
			if (it != m_vecCollidingChampion.end()) {
				m_vecCollidingChampion.erase(it);
			}
		}*/

		//��ž ��Ÿ����� �����, Ÿ���̿��� �ִ� �����Ǿ����
		if (m_pTarget == _Other->GetOwner()) {
			m_pTarget = nullptr;
		}
	}
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