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

	// ì‚¬ìš©í•  ì¬ì§ˆ ì—…ë°ì´íŠ¸
	pMtrl->UpdateData();

	// ì‚¬ìš©í•  ë©”ì‰¬ ì—…ë°ì´íŠ¸ ë° ë Œë”ë§
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
	//Cameraì—ì„œ sortobjectí• ë•Œ getmaterialë¡œ íŒì •í•¨. ì´ë•Œ mtrlì´ ì—†ìœ¼ë©´ nullptrë°˜í™˜
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
	// ì›ë³¸ ì¬ì§ˆì´ ì—†ë‹¤ -> Nullptr ë°˜í™˜
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

	// µ¿Àû ÀçÁúÀ» »ç¿ëÇÏ´Â °æ¿ì ÀÌ¿¡ ´ëÇÑ Á¤º¸ ÀúÀå
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		if (m_vecMtrls[i].pDynamicMtrl != nullptr)
		{
			IsDynamicMtrlExist = true;
			fwrite(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

			// µ¿Àû ÀçÁúÀÌ Á¸ÀçÇÑ´Ù¸é ±×¿¡´ëÇÑ Á¤º¸¸¦ ÀúÀåÇØ Áà¾ßÇÔ

			// Const Á¤º¸ ÀúÀå
			tMtrlConst SaveConstMtrlData = m_vecMtrls[i].pDynamicMtrl->GetMtrlConst();
			fwrite(&SaveConstMtrlData, sizeof(tMtrlConst), 1, _File);

			// Texture Á¤º¸ ÀúÀå
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

			// µ¿Àû ÀçÁúÀÌ Á¸ÀçÇÏÁö ¾Ê¾Ò´Ù¸é ´õÀÌ»ó ÀúÀåÇÒ Á¤º¸´Â ¾øÀ½

		}
	}

}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadResRef(m_pMesh, _File);
	SetMesh(m_pMesh);

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

	
	// µ¿Àû ÀçÁú¿¡ ´ëÇÑ Á¤º¸¸¦ °¡Á®¿Í¾ß µÇ´ÂÁö È®ÀÎ
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		bool IsDynamicMtrlExist;

		fread(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

		// ¸¸¾à¿¡ µ¿Àû ÀçÁúÀÌ Á¸ÀçÇß¾úÀ¸¸é µ¿ÀûÀçÁúÀ» ¸¸µé°í, ±×¿¡´ëÇÑ Á¤º¸¸¦ ºÒ·¯¿Í¾ßÇÔ
		if (IsDynamicMtrlExist == true)
		{
			// Dynamic Material »ı¼º¸ÕÀúÇÑ´Ù
			GetDynamicMaterial(i);


			// ConstMtrl Data ºÒ·¯¿À±â
			tMtrlConst LoadConstMtrlData;
			fread(&LoadConstMtrlData, sizeof(tMtrlConst), 1, _File);
			m_vecMtrls[i].pDynamicMtrl->SetMtrlConst(LoadConstMtrlData);


			// Texture Á¤º¸ ºÒ·¯¿À±â
			for (UINT j = 0; j < (UINT)TEX_PARAM::TEX_END; ++j)
			{
				Ptr<CTexture> LoadTextureData;
				LoadResRef(LoadTextureData, _File);

				m_vecMtrls[i].pDynamicMtrl->SetTexParam((TEX_PARAM)j, LoadTextureData);

			}
		}
	}
}

void CRenderComponent::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	string key = "Mesh";
	Value keyName(kStringType);
	keyName.SetString(key.c_str(), key.length(), allocator);
	_objValue.AddMember(keyName, SaveResRefJson(m_pMesh.Get(), allocator), allocator);

	UINT iMtrlCount = GetMtrlCount();
	_objValue.AddMember("MtrlCount", iMtrlCount, allocator);

	for (UINT i = 0; i < m_vecMtrls.size(); ++i)
	{
		string key = "vecMtrls[" + std::to_string(i) + "]SharedMtrl";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		_objValue.AddMember(keyName, SaveResRefJson(m_vecMtrls[i].pSharedMtrl.Get(), allocator), allocator);
	}

	_objValue.AddMember("fBounding", m_fBounding, allocator);
	_objValue.AddMember("bFrustumCheck", m_bFrustumCheck, allocator);
	_objValue.AddMember("bDynamicShadow", m_bDynamicShadow, allocator);

}

void CRenderComponent::LoadFromLevelJsonFile(const Value& _componentValue)
{
	LoadResRefJson(m_pMesh, _componentValue["Mesh"]);

	UINT iMtrlCount = _componentValue["MtrlCount"].GetUint();
	m_vecMtrls.resize(iMtrlCount);
	for (UINT i = 0; i < m_vecMtrls.size(); ++i)
	{
		string key = "vecMtrls[" + std::to_string(i) + "]SharedMtrl";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());
		LoadResRefJson(m_vecMtrls[i].pSharedMtrl, _componentValue[keyName]);
	}

	m_fBounding = _componentValue["fBounding"].GetFloat();
	m_bFrustumCheck = _componentValue["bFrustumCheck"].GetBool();
	m_bDynamicShadow = _componentValue["bDynamicShadow"].GetBool();
}
