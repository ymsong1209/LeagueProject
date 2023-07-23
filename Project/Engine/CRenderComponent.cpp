#include "pch.h"
#include "CRenderComponent.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CAnimator3D.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_fBounding(150.f)
	, m_bFrustumCheck(true)
	, m_bDynamicShadow(false)
	, m_bShowDebugBoundShape(false)
	, m_bUseBoundingOffset(false)
	, m_vBoundingBoxOffset(Vec3(0.f, 0.f, 0.f))
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _other)
	: CComponent(_other)
	, m_pMesh(_other.m_pMesh)
	, m_vecMtrls(_other.m_vecMtrls)
	, m_fBounding(_other.m_fBounding)
	, m_bFrustumCheck(_other.m_bFrustumCheck)
	, m_bDynamicShadow(_other.m_bDynamicShadow)
	, m_bShowDebugBoundShape(_other.m_bShowDebugBoundShape)
	, m_bUseBoundingOffset(_other.m_bUseBoundingOffset)
	, m_vBoundingBoxOffset(_other.m_vBoundingBoxOffset)
{

}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::render_depthmap()
{
	Transform()->UpdateData();

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DepthMapMtrl");

	if (GetOwner()->Animator3D() != nullptr)
	{
		GetOwner()->Animator3D()->GetFinalBoneMat()->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
		pMtrl->SetAnim3D(true);
	}

	else
	{
		pMtrl->SetAnim3D(false);
	}

	// 사용할 재질 업데이트
	pMtrl->UpdateData();

	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// 사용할 재질 업데이트
			//GetMaterial(i)->UpdateData();

			// 사용할 메쉬 업데이트 및 렌더링
			GetMesh()->render(i);
		}
	}

	if (GetOwner()->GetComponent(COMPONENT_TYPE::ANIMATOR3D)) {
		GetOwner()->Animator3D()->GetFinalBoneMat()->Clear();
	}
	

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
	// Mesh가 먼저 지정되어 있지 않으면 , m_vecMtrls의 크기가 0이기 때문에 오류가 발생합니다.
	// SetMesh함수를 참고하시고 이와 관련해 얘기할 것이 있으면 장건희를 호출하세요
	if (_idx >= m_vecMtrls.size())
		assert(false, "CRenderComponent::SetMaterial 오류");

	m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
	m_vecMtrls[_idx].pCurMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
	//Camera에서 sortobject할때 getmaterial로 판정함. 이때 mtrl이 없으면 nullptr반환
	if (m_vecMtrls.size() == 0) return nullptr;

	if (_idx >= m_vecMtrls.size())
		return nullptr;

	if (nullptr == m_vecMtrls[_idx].pCurMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;
	}

	return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
	if (_idx >= m_vecMtrls.size())
		return nullptr;

	m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

	if (m_vecMtrls[_idx].pDynamicMtrl.Get())
	{
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
	}

	return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
	if (_idx >= m_vecMtrls.size())
		return nullptr;

	// 원본 재질이 없다 -> Nullptr 반환
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

bool CRenderComponent::IsDynamicMtrlEmpty(UINT _idx)
{
	if (_idx >= m_vecMtrls.size())
		assert(false, "CRenderComponenp::IsDynamicMtrlEmpty 오류");

	return (nullptr == m_vecMtrls[_idx].pDynamicMtrl);
}

void CRenderComponent::ClearDynamicMtrl(UINT _idx)
{
	if (_idx >= m_vecMtrls.size())
		return;

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
	fwrite(&m_bUseBoundingOffset, sizeof(bool), 1, _File);
	fwrite(&m_vBoundingBoxOffset, sizeof(Vec3), 1, _File);


	bool IsDynamicMtrlExist = false;

	// 동적 재질을 사용하는 경우 이에 대한 정보 저장
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		if (m_vecMtrls[i].pDynamicMtrl != nullptr)
		{
			IsDynamicMtrlExist = true;
			fwrite(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

			// 동적 재질이 존재한다면 그에대한 정보를 저장해 줘야함

			// Const 정보 저장
			tMtrlConst SaveConstMtrlData = m_vecMtrls[i].pDynamicMtrl->GetMtrlConst();
			fwrite(&SaveConstMtrlData, sizeof(tMtrlConst), 1, _File);

			// Texture 정보 저장
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

			// 동적 재질이 존재하지 않았다면 더이상 저장할 정보는 없음

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
	fread(&m_bUseBoundingOffset, sizeof(bool), 1, _File);
	fread(&m_vBoundingBoxOffset, sizeof(Vec3), 1, _File);
	
	// 동적 재질에 대한 정보를 가져와야 되는지 확인
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		bool IsDynamicMtrlExist;

		fread(&IsDynamicMtrlExist, sizeof(bool), 1, _File);

		// 만약에 동적 재질이 존재했었으면 동적재질을 만들고, 그에대한 정보를 불러와야함
		if (IsDynamicMtrlExist == true)
		{
			// Dynamic Material 생성먼저한다
			GetDynamicMaterial(i);


			// ConstMtrl Data 불러오기
			tMtrlConst LoadConstMtrlData;
			fread(&LoadConstMtrlData, sizeof(tMtrlConst), 1, _File);
			m_vecMtrls[i].pDynamicMtrl->SetMtrlConst(LoadConstMtrlData);


			// Texture 정보 불러오기
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

	_objValue.AddMember("fBounding", m_fBounding, allocator);
	_objValue.AddMember("bFrustumCheck", m_bFrustumCheck, allocator);
	_objValue.AddMember("bDynamicShadow", m_bDynamicShadow, allocator);
	_objValue.AddMember("bShowDebugBoundShape" , m_bShowDebugBoundShape, allocator);
	_objValue.AddMember("bUseBoundingOffset", m_bUseBoundingOffset, allocator);
	_objValue.AddMember("vBoundingBoxOffset", SaveVec3Json(m_vBoundingBoxOffset, allocator), allocator);

	UINT iMtrlCount = GetMtrlCount();
	_objValue.AddMember("iMtrlCount", iMtrlCount, allocator);

	// SharedMtrl
	for (UINT i = 0; i < m_vecMtrls.size(); ++i)
	{
		string key = "vecMtrls[" + std::to_string(i) + "]SharedMtrl";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		_objValue.AddMember(keyName, SaveResRefJson(m_vecMtrls[i].pSharedMtrl.Get(), allocator), allocator);
	}

	// 동적 재질을 사용하는 경우 이에 대한 정보 저장
	// DynamicMtrl
	bool IsDynamicMtrlExist = false;
	
	Value DynamicMtrlsArray(kArrayType);
	for (UINT i = 0; i < m_vecMtrls.size(); ++i)
	{
		Value dynamicMtrlValue(kObjectType);
	
		// 가독성용
		string key = "vecMtrls[" + std::to_string(i) + "]DynamicMtrl";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		dynamicMtrlValue.AddMember(keyName, Value(kNullType), allocator); 
	
		if (m_vecMtrls[i].pDynamicMtrl != nullptr)
		{
			IsDynamicMtrlExist = true;
			dynamicMtrlValue.AddMember("IsDynamicMtrlExist", IsDynamicMtrlExist, allocator);
	
			// 동적 재질이 존재한다면, 정보 저장
	
			// Const 정보 저장(tMtrlConst)
			tMtrlConst SaveConstMtrlData = m_vecMtrls[i].pDynamicMtrl->GetMtrlConst();
			dynamicMtrlValue.AddMember("ConstMtrlData", SavetMtrlConst(SaveConstMtrlData, allocator), allocator);
	
			// Texture 정보 저장
			for (UINT j = 0; j < (UINT)TEX_PARAM::TEX_END; ++j)
			{
				Ptr<CTexture> TextureData = m_vecMtrls[i].pDynamicMtrl->GetTexParam(TEX_PARAM(j));
	
				string key = "TEX_PARAM[" + std::to_string(j) + "]";
				Value keyName(kStringType);
				keyName.SetString(key.c_str(), key.length(), allocator);
				dynamicMtrlValue.AddMember(keyName, SaveResRefJson(TextureData.Get(), allocator), allocator);
			}
		}
		else
		{
			IsDynamicMtrlExist = false;
			dynamicMtrlValue.AddMember("IsDynamicMtrlExist", IsDynamicMtrlExist, allocator);

			// 동적 재질이 존재하지 않았다면, 정보 저장 X
		}
		DynamicMtrlsArray.PushBack(dynamicMtrlValue, allocator);
	}
	_objValue.AddMember("DynamicMtrls", DynamicMtrlsArray, allocator);
}

void CRenderComponent::LoadFromLevelJsonFile(const Value& _componentValue)
{
	LoadResRefJson(m_pMesh, _componentValue["Mesh"]);
	SetMesh(m_pMesh);

	m_fBounding = _componentValue["fBounding"].GetFloat();
	m_bFrustumCheck = _componentValue["bFrustumCheck"].GetBool();
	m_bDynamicShadow = _componentValue["bDynamicShadow"].GetBool();
	m_bShowDebugBoundShape = _componentValue["bShowDebugBoundShape"].GetBool();
	m_bUseBoundingOffset = _componentValue["bUseBoundingOffset"].GetBool();
	m_vBoundingBoxOffset = LoadVec3Json(_componentValue["vBoundingBoxOffset"]);

	UINT iMtrlCount = _componentValue["iMtrlCount"].GetUint();
	m_vecMtrls.resize(iMtrlCount);

	for (UINT i = 0; i < m_vecMtrls.size(); ++i)
	{
		Ptr<CMaterial> pMtrl;
		string key = "vecMtrls[" + std::to_string(i) + "]SharedMtrl";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());
		LoadResRefJson(pMtrl, _componentValue[keyName]);

		SetMaterial(pMtrl, i);
	}

	// 동적 재질에 대한 정보를 가져와야 되는지 확인
	const Value& DynamicMtrlsArray = _componentValue["DynamicMtrls"];
	for (UINT i = 0; i < DynamicMtrlsArray.Size(); ++i)
	{
		bool IsDynamicMtrlExist;
		IsDynamicMtrlExist = DynamicMtrlsArray[i]["IsDynamicMtrlExist"].GetBool();

		// true일경우, 동적재질을 만들고, 정보 Load
		if (IsDynamicMtrlExist == true)
		{
			// Dynamic Material 생성먼저한다
			GetDynamicMaterial(i);

			// ConstMtrl Data 불러오기
			tMtrlConst LoadConstMtrlData;
			LoadConstMtrlData = LoadtMtrlConst(DynamicMtrlsArray[i]["ConstMtrlData"]);
			m_vecMtrls[i].pDynamicMtrl->SetMtrlConst(LoadConstMtrlData);

			// Texture 정보 불러오기
			for (UINT j = 0; j < (UINT)TEX_PARAM::TEX_END; ++j)
			{
				string key = "TEX_PARAM[" + std::to_string(j) + "]";
				Value keyName(kStringType);
				keyName.SetString(key.c_str(), key.length());

				Ptr<CTexture> LoadTextureData;
				LoadResRefJson(LoadTextureData, DynamicMtrlsArray[i][keyName]);

				m_vecMtrls[i].pDynamicMtrl->SetTexParam((TEX_PARAM)j, LoadTextureData);

			}
		}
	}
}
