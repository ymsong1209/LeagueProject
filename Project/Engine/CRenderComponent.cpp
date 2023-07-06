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

void CRenderComponent::render_depthmap()
{
	Transform()->UpdateData();

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DepthMapMtrl");

	// ����� ���� ������Ʈ
	pMtrl->UpdateData();

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render(0);

}

void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
	m_pMesh = _Mesh;

	if (!m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}


	if (nullptr != m_pMesh)
		m_vecMtrls.resize(m_pMesh->GetSubsetCount());
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx)
{
	m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
	m_vecMtrls[_idx].pCurMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
	//Camera���� sortobject�Ҷ� getmaterial�� ������. �̶� mtrl�� ������ nullptr��ȯ
	if (m_vecMtrls.size() == 0) return nullptr;

	if (nullptr == m_vecMtrls[_idx].pCurMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;
	}

	return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
	m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

	if (m_vecMtrls[_idx].pDynamicMtrl.Get())
	{
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
	}

	return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
	// ���� ������ ���� -> Nullptr ��ȯ
	if (nullptr == m_vecMtrls[_idx].pSharedMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = nullptr;
		return m_vecMtrls[_idx].pCurMtrl;
	}

	if (nullptr == m_vecMtrls[_idx].pDynamicMtrl)
	{
		m_vecMtrls[_idx].pDynamicMtrl = m_vecMtrls[_idx].pSharedMtrl->Clone();
		m_vecMtrls[_idx].pDynamicMtrl->SetName(m_vecMtrls[_idx].pSharedMtrl->GetName() + L"_Clone");
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pDynamicMtrl;
	}

	return m_vecMtrls[_idx].pCurMtrl;
}

void CRenderComponent::ClearDynamicMtrl(UINT _idx)
{
	if (m_vecMtrls[_idx].pCurMtrl   ==  m_vecMtrls[_idx].pDynamicMtrl)
	{
		if ( m_vecMtrls[_idx].pSharedMtrl != nullptr)
		{
			m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;
		}

		else
		{
			m_vecMtrls[_idx].pCurMtrl = nullptr;
		}
	}

	m_vecMtrls[_idx].pDynamicMtrl = nullptr;
}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	
	SaveResRef(m_pMesh.Get(), _File);

	UINT iMtrlCount = GetMtrlCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveResRef(m_vecMtrls[i].pSharedMtrl.Get(), _File);
	}


	fwrite(&m_fBounding, sizeof(float), 1, _File);
	fwrite(&m_bFrustumCheck, sizeof(bool), 1, _File);
	fwrite(&m_bDynamicShadow, sizeof(bool), 1, _File);


	bool IsDynamicMtrlExist = false;

	// ���� ������ ����ϴ� ��� �̿� ���� ���� ����
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		if (m_vecMtrls[i].pDynamicMtrl != nullptr)
		{
			IsDynamicMtrlExist = true;
			fwrite(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

			// ���� ������ �����Ѵٸ� �׿����� ������ ������ �����

			// Const ���� ����
			tMtrlConst SaveConstMtrlData = m_vecMtrls[i].pDynamicMtrl->GetMtrlConst();
			fwrite(&SaveConstMtrlData, sizeof(tMtrlConst), 1, _File);

			// Texture ���� ����
			//Ptr<CTexture> GetTexParam(TEX_PARAM _param) { return m_arrTex[(UINT)_param]; }

			for (UINT j = 0; j < (UINT)TEX_PARAM::TEX_END ;  ++j)
			{
				Ptr<CTexture> SaveTextureData = m_vecMtrls[i].pDynamicMtrl->GetTexParam(TEX_PARAM(j));
				SaveResRef(SaveTextureData.Get(), _File);

			}
		}

		else
		{
			IsDynamicMtrlExist = false;
			fwrite(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

			// ���� ������ �������� �ʾҴٸ� ���̻� ������ ������ ����

		}
	}

}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadResRef(m_pMesh, _File);

	UINT iMtrlCount = GetMtrlCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _File);

	m_vecMtrls.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		Ptr<CMaterial> pMtrl;
		LoadResRef(pMtrl, _File);
		SetMaterial(pMtrl, i);
	}

	fread(&m_fBounding, sizeof(float), 1, _File);
	fread(&m_bFrustumCheck, sizeof(bool), 1, _File);
	fread(&m_bDynamicShadow, sizeof(bool), 1, _File);

	
	// ���� ������ ���� ������ �����;� �Ǵ��� Ȯ��
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		bool IsDynamicMtrlExist;

		fread(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

		// ���࿡ ���� ������ �����߾����� ���������� �����, �׿����� ������ �ҷ��;���
		if (IsDynamicMtrlExist == true)
		{
			// Dynamic Material ���������Ѵ�
			GetDynamicMaterial(i);


			// ConstMtrl Data �ҷ�����
			tMtrlConst LoadConstMtrlData;
			fread(&LoadConstMtrlData, sizeof(tMtrlConst), 1, _File);
			m_vecMtrls[i].pDynamicMtrl->SetMtrlConst(LoadConstMtrlData);


			// Texture ���� �ҷ�����
			for (UINT j = 0; j < (UINT)TEX_PARAM::TEX_END; ++j)
			{
				Ptr<CTexture> LoadTextureData;
				LoadResRef(LoadTextureData, _File);

				m_vecMtrls[i].pDynamicMtrl->SetTexParam((TEX_PARAM)j, LoadTextureData);

			}
		}
	}
}
