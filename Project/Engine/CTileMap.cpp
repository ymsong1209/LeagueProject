#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iTileCountX(1)
	, m_iTileCountY(1)
{	
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"),0);

	m_Buffer = new CStructuredBuffer;
	m_Buffer->Create(sizeof(tTile), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_ONLY, true);
}

CTileMap::~CTileMap()
{
	if (nullptr != m_Buffer)
		delete m_Buffer;
}

void CTileMap::finaltick()
{
}

void CTileMap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// 재질 업데이트
	GetMaterial(0)->SetScalarParam(INT_0, &m_iTileCountX);
	GetMaterial(0)->SetScalarParam(INT_1, &m_iTileCountY);
	GetMaterial(0)->UpdateData();

	// 구조화버퍼 업데이트
	UpdateData();

	// 렌더
	GetMesh()->render(0);
}

void CTileMap::render(UINT _iSubset)
{
	render();
}


void CTileMap::UpdateData()
{
	m_Buffer->SetData(m_vecTile.data(), sizeof(tTile) * (UINT)m_vecTile.size());
	m_Buffer->UpdateData(20, PIPELINE_STAGE::PS_PIXEL);
}

void CTileMap::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	m_iTileCountX = _iXCount;
	m_iTileCountY = _iYCount;

	m_vecTile.clear();
	m_vecTile.resize(m_iTileCountX * m_iTileCountY);

	if (m_Buffer->GetElementCount() < m_vecTile.size())
	{
		m_Buffer->Create(sizeof(tTile), (UINT)m_vecTile.size(), SB_TYPE::READ_ONLY, true);
	}

	// 타일 세팅 테스트
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{			
			m_vecTile[i * m_iTileCountX + j].vLeftTop.x = m_vSliceSize.x * j;
			m_vecTile[i * m_iTileCountX + j].vLeftTop.y = 0.f;
			m_vecTile[i * m_iTileCountX + j].vSlice = m_vSliceSize;
		}
	}

	m_vecTile[0].vLeftTop = Vec2(m_vSliceSize.x * 7.f, m_vSliceSize.y * 5.f);
}

void CTileMap::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_iTileCountX, sizeof(UINT), 1, _File);
	fwrite(&m_iTileCountY, sizeof(UINT), 1, _File);
	fwrite(&m_vSliceSize, sizeof(Vec2), 1, _File);	
	fwrite(m_vecTile.data(), sizeof(tTile), m_vecTile.size(), _File);
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_iTileCountX, sizeof(UINT), 1, _File);
	fread(&m_iTileCountY, sizeof(UINT), 1, _File);
	fread(&m_vSliceSize, sizeof(Vec2), 1, _File);

	SetTileCount(m_iTileCountX, m_iTileCountY);

	fread(m_vecTile.data(), sizeof(tTile), m_iTileCountX * m_iTileCountY, _File);
}

void CTileMap::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CRenderComponent::SaveToLevelJsonFile(_objValue, allocator);

	_objValue.AddMember("iTileCountX", m_iTileCountX, allocator);
	_objValue.AddMember("iTileCountY", m_iTileCountY, allocator);
	_objValue.AddMember("vSliceSize", SaveVec2Json(m_vSliceSize, allocator), allocator);

	// m_vecTile
	Value vecTileArray(kArrayType);
	for (const auto& pair : m_vecTile)
	{
		Value TileObject(kObjectType);

		TileObject.AddMember("vLeftTop", SaveVec2Json(pair.vLeftTop, allocator), allocator);
		TileObject.AddMember("vSlice", SaveVec2Json(pair.vSlice, allocator), allocator);
	
		vecTileArray.PushBack(TileObject, allocator);
	}
	_objValue.AddMember("vecTile",vecTileArray, allocator);

}

void CTileMap::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CRenderComponent::LoadFromLevelJsonFile(_componentValue);

	m_iTileCountX = _componentValue["iTileCountX"].GetUint();
	m_iTileCountY = _componentValue["iTileCountY"].GetUint();
	m_vSliceSize = LoadVec2Json(_componentValue["vSliceSize"]);

	SetTileCount(m_iTileCountX, m_iTileCountY);

	const Value& vecTileArray = _componentValue["vecTile"];
	for (size_t i = 0; i < vecTileArray.Size(); ++i)
	{
		tTile newTile = {};
		newTile.vLeftTop = LoadVec2Json(vecTileArray[i]["vLeftTop"]);
		newTile.vSlice = LoadVec2Json(vecTileArray[i]["vSlice"]);

		m_vecTile.push_back(newTile);
	}
}
