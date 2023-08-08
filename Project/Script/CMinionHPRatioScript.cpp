#include "pch.h"
#include "CMinionHPRatioScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>
#include "CUnitScript.h"

void CMinionHPRatioScript::begin()
{
	Transform()->SetRelativeScale(Vec3(154.f, 15.5f, 1.f));
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//===================�ν��Ͻ� ���XXX==========================
	CGameObject* ParentObj = GetOwner()->GetParent();
	Faction OwnerFaction = ParentObj->GetScript<CUnitScript>()->GetFaction();
	if (OwnerFaction == Faction::BLUE)
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionBar.mtrl"), 0);
	else if (OwnerFaction == Faction::RED)
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedMinionBar.mtrl"), 0);

	//===================�ν��Ͻ� ���==========================
	// 
	//CGameObject* ParentObj = GetOwner()->GetParent();
	//Faction OwnerFaction = ParentObj->GetScript<CUnitScript>()->GetFaction();
	//if (OwnerFaction == Faction::BLUE)
		//MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionBarHP.mtrl"), 0);
	//else if (OwnerFaction == Faction::RED)
		//MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedMinionBarHP.mtrl"), 0);

}

void CMinionHPRatioScript::tick()
{
	if (!GetOwner()->GetParent()->IsDead())
	{
		CGameObject* ParentObj = GetOwner()->GetParent();
		bool IsCulling = ParentObj->GetRenderComponent()->IsCulled();
		bool IsRaySightCulling = ParentObj->GetRenderComponent()->IsUsingRaySightCulling();
		if (IsCulling && IsRaySightCulling)
			return;

		m_fCurHP = ParentObj->GetScript<CUnitScript>()->GetCurHP();
		m_fTotalHP = ParentObj->GetScript<CUnitScript>()->GetMaxHP();
		Transform()->SetAbsolute(false);
		Transform()->SetBillBoard(true);
		//============================================================================
		// ���� �ڽ��� ���������̰�, �θ�� ���������̶� ������ ��. �ڽ��� �θ��� Pos,Scale,Rot�� �����ͼ� �ڽ��� ��Ŀ� �����Ű�µ� 
		// �̰����� ���� ������ ��ǥ�̱⶧���� ���ٻ��� ��ǥ�� ���������� ��� ������ �༭ �׳ɶȰ��� ����� ��ġ�ؾ� �ҰͰ���.
		//============================================================================
		//----------HP------------
		
		m_fRatio = m_fCurHP / m_fTotalHP;
		if (m_fCurHP >= m_fTotalHP)
			m_fCurHP = m_fTotalHP;

		if (m_fCurHP <= 0.f)
			m_fCurHP = 0.f;
		//-------------------------

		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fRatio);
	}

}

void CMinionHPRatioScript::BeginOverlap(CCollider2D* _Other)
{
}


CMinionHPRatioScript::CMinionHPRatioScript()
	:CScript((UINT)SCRIPT_TYPE::MINIONHPRATIOSCRIPT)
{
}

CMinionHPRatioScript::~CMinionHPRatioScript()
{
}
