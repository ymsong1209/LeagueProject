#include "pch.h"
#include "CSkillLevelUpUIScript.h"
#include <Engine\CAnimator2D.h>
#include <Engine\CAnim2D.h>
#include "CSkillLvEffectUIScript.h"
#include "CUnitScript.h"

CSkillLevelUpUIScript::CSkillLevelUpUIScript()
	:CScript((UINT)SCRIPT_TYPE::SKILLLEVELUPUISCRIPT)
{
}

CSkillLevelUpUIScript::~CSkillLevelUpUIScript()
{
}


void CSkillLevelUpUIScript::begin()
{
	Q_LvUpObj->GetRenderComponent()->SetSortExcept(true);
	W_LvUpObj->GetRenderComponent()->SetSortExcept(true);
	E_LvUpObj->GetRenderComponent()->SetSortExcept(true);
	R_LvUpObj->GetRenderComponent()->SetSortExcept(true);

	//=====������=====
	/*m_iSkillPoint = 30;
	m_iRLevel = 5;*/
	//===================
}

void CSkillLevelUpUIScript::tick()
{

	//���⼭ ���� �÷��̾� ��ų ����Ʈ ��������
	//���� : int m_iSkillPoint = CUnitScript* pUnitScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetSkillPoint();
	if (m_iSkillPoint != 0)
	{
		CUnitScript* pUnitScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
		m_iQLevel = pUnitScript->GetSkillLevel(1);
		m_iWLevel = pUnitScript->GetSkillLevel(2);
		m_iELevel = pUnitScript->GetSkillLevel(3);
		m_iRLevel = pUnitScript->GetSkillLevel(4);


		if(m_iQLevel < 5) //5���� �� �ƴҶ��� ��ų ������ ǥ�ð� ������
			Q_LvUpObj->GetRenderComponent()->SetSortExcept(false);
		else
			Q_LvUpObj->GetRenderComponent()->SetSortExcept(true);

		if(m_iWLevel < 5)
			W_LvUpObj->GetRenderComponent()->SetSortExcept(false);
		else
			W_LvUpObj->GetRenderComponent()->SetSortExcept(true);

		if(m_iELevel < 5)
			E_LvUpObj->GetRenderComponent()->SetSortExcept(false);
		else
			E_LvUpObj->GetRenderComponent()->SetSortExcept(true);

		if(m_iRLevel < 5)
			R_LvUpObj->GetRenderComponent()->SetSortExcept(false);
		else
			R_LvUpObj->GetRenderComponent()->SetSortExcept(true);

		if (KEY_TAP(KEY::_1) && m_iQLevel < 5)
		{
			CGameObject* Up_Effect = new CGameObject; //ĳ���� �г� ��ġ
			Up_Effect->SetName(L"Up_Effect");
			Up_Effect->AddComponent(new CTransform);
			Up_Effect->AddComponent(new CMeshRender);
			Up_Effect->AddComponent(new CAnimator2D);
			Up_Effect->AddComponent(new CSkillLvEffectUIScript);
			Up_Effect->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 1.f));
			Up_Effect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Up_Effect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Anim2DUIMtrl.mtrl"), 0);

			CAnim2D* Anim2d = new CAnim2D;
			Anim2d->Load(L"anim2d\\SkillLevelUpAnim.anim2d");
			Up_Effect->Animator2D()->AddAnim(Anim2d);
			Up_Effect->Animator2D()->Play(L"SkillLevelUpAnim", false);
			SpawnGameObject(Up_Effect, Vec3(-130.f, -405.805f, 333.f), 31);
		}

		else if (KEY_TAP(KEY::_2) && m_iWLevel < 5)
		{
			CGameObject* Up_Effect = new CGameObject; //ĳ���� �г� ��ġ
			Up_Effect->SetName(L"Up_Effect");
			Up_Effect->AddComponent(new CTransform);
			Up_Effect->AddComponent(new CMeshRender);
			Up_Effect->AddComponent(new CAnimator2D);
			Up_Effect->AddComponent(new CSkillLvEffectUIScript);
			Up_Effect->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 1.f));
			Up_Effect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Up_Effect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Anim2DUIMtrl.mtrl"), 0);

			CAnim2D* Anim2d = new CAnim2D;
			Anim2d->Load(L"anim2d\\SkillLevelUpAnim.anim2d");
			Up_Effect->Animator2D()->AddAnim(Anim2d);
			Up_Effect->Animator2D()->Play(L"SkillLevelUpAnim", false);
			SpawnGameObject(Up_Effect, Vec3(-77.f, -405.805f, 333.f), 31);
		}

		else if (KEY_TAP(KEY::_3) && m_iELevel < 5)
		{
			CGameObject* Up_Effect = new CGameObject; //ĳ���� �г� ��ġ
			Up_Effect->SetName(L"Up_Effect");
			Up_Effect->AddComponent(new CTransform);
			Up_Effect->AddComponent(new CMeshRender);
			Up_Effect->AddComponent(new CAnimator2D);
			Up_Effect->AddComponent(new CSkillLvEffectUIScript);
			Up_Effect->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 1.f));
			Up_Effect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Up_Effect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Anim2DUIMtrl.mtrl"), 0);

			CAnim2D* Anim2d = new CAnim2D;
			Anim2d->Load(L"anim2d\\SkillLevelUpAnim.anim2d");
			Up_Effect->Animator2D()->AddAnim(Anim2d);
			Up_Effect->Animator2D()->Play(L"SkillLevelUpAnim", false);
			SpawnGameObject(Up_Effect, Vec3(-26.f, -405.805f, 333.f), 31);
		}

		else if (KEY_TAP(KEY::_4) && m_iRLevel < 5)
		{
			CGameObject* Up_Effect = new CGameObject; //ĳ���� �г� ��ġ
			Up_Effect->SetName(L"Up_Effect");
			Up_Effect->AddComponent(new CTransform);
			Up_Effect->AddComponent(new CMeshRender);
			Up_Effect->AddComponent(new CAnimator2D);
			Up_Effect->AddComponent(new CSkillLvEffectUIScript);
			Up_Effect->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 1.f));
			Up_Effect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Up_Effect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Anim2DUIMtrl.mtrl"), 0);

			CAnim2D* Anim2d = new CAnim2D;
			Anim2d->Load(L"anim2d\\SkillLevelUpAnim.anim2d");
			Up_Effect->Animator2D()->AddAnim(Anim2d);
			Up_Effect->Animator2D()->Play(L"SkillLevelUpAnim", false);
			SpawnGameObject(Up_Effect, Vec3(25.f, -405.805f, 333.f), 31);
		}
	}
	else
	{
		//���� ȣ�� x
		Q_LvUpObj->GetRenderComponent()->SetSortExcept(true);
		W_LvUpObj->GetRenderComponent()->SetSortExcept(true);
		E_LvUpObj->GetRenderComponent()->SetSortExcept(true);
		R_LvUpObj->GetRenderComponent()->SetSortExcept(true);
	}

	//======������========
	//if (KEY_TAP(KEY::_0))
	//{
	//	m_iSkillPoint += 1;
	//}
	//======================
}

void CSkillLevelUpUIScript::BeginOverlap(CCollider2D* _Other)
{
}