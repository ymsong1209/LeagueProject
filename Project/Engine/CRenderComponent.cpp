#include "pch.h"
#include "CRenderComponent.h"

#include "CResMgr.h"
#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_fBounding(500.f)
	, m_bFrustumCheck(true)
	, m_bDynamicShadow(false)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_pSharedMtrl = _Mtrl;
	m_pCurrentMtrl = m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 원본 재질이 없다 -> Nullptr 반환
	if (nullptr == m_pSharedMtrl)
	{
		m_pCurrentMtrl = nullptr;
		return m_pCurrentMtrl;
	}

	// 동적 재질 최초 요청시 제작 해서 준다.
	if (nullptr == m_pDynamicMtrl)
	{		
		m_pDynamicMtrl = m_pSharedMtrl->Clone();		
	}

	// 동적 재질을 현재 사용재질로 지정한다.
	m_pCurrentMtrl = m_pDynamicMtrl;


	return m_pCurrentMtrl;
}

void CRenderComponent::render_depthmap()
{
	Transform()->UpdateData();

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DepthMapMtrl");

	// 사용할 재질 업데이트
	pMtrl->UpdateData();

	// 사용할 메쉬 업데이트 및 렌더링
	GetMesh()->render();

}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	SaveResRef(m_pMesh.Get(), _File);
	SaveResRef(m_pSharedMtrl.Get(), _File);	
	fwrite(&m_fBounding, sizeof(float), 1, _File);
	fwrite(&m_bFrustumCheck, sizeof(bool), 1, _File);
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadResRef(m_pMesh, _File);
	LoadResRef(m_pSharedMtrl, _File);
	fread(&m_fBounding, sizeof(float), 1, _File);
	fread(&m_bFrustumCheck, sizeof(bool), 1, _File);

	SetMaterial(m_pSharedMtrl);
}
