#include "pch.h"
#include "CMinimapUIScript.h"
#include <Engine\CLevelMgr.h>
#include "CUnitScript.h"

void CMinimapUIScript::begin()
{
	CGameObject* Map = new CGameObject;
	Map->SetName(L"ParentMap");
	Map->AddComponent(new CTransform);
	Map->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 0.1f));
	SpawnGameObject(Map, Vec3(163.9f, -90.5f, 20.f), 31);

	vector<CGameObject*> AllPlayer = CSendServerEventMgr::GetInst()->GetVecAllPlayer();
	for (size_t i = 0; i < AllPlayer.size(); ++i)
	{
		m_vAllPlayer.push_back(AllPlayer[i]);
	}

	CGameObject* MinimapFrame = new CGameObject;
	MinimapFrame->SetName(L"MiniMapFrame");
	MinimapFrame->AddComponent(new CMeshRender);
	MinimapFrame->AddComponent(new CTransform);
	MinimapFrame->Transform()->SetRelativeScale(Vec3(270.f, 270.f, 0.1f));
	MinimapFrame->Transform()->SetRelativePos(Vec3(501.f, -275.f, 0.f));
	MinimapFrame->Transform()->SetAbsolute(true);
	MinimapFrame->Transform()->SetUseMouseOutline(false);
	MinimapFrame->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MinimapFrame->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MinimapFrame.mtrl"), 0);
	Map->AddChild(MinimapFrame);

	// MiniMap
	Minimap = new CGameObject;
	Minimap->SetName(L"MiniMap");
	Minimap->AddComponent(new CMeshRender);
	Minimap->AddComponent(new CTransform);
	Minimap->Transform()->SetRelativeScale(Vec3(255.f, 255.f, 0.1f));
	Minimap->Transform()->SetAbsolute(true);
	Minimap->Transform()->SetRelativePos(Vec3(505.f, -275.f, 27.f));
	Minimap->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Minimap->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MiniMapMtrl"), 0);
	Minimap->MeshRender()->GetDynamicMaterial(0);
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\MiniMap.dds"));
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"FogFilterMap"));
	Map->AddChild(Minimap);

}

void CMinimapUIScript::tick()
{
	if (m_vAllPlayer.size() != 0)
	{
		Vec2 MapSize = Vec2(0.232727f, 0.232727f);
		for (size_t i = 0; i < m_vAllPlayer.size(); ++i)
		{
			ChampionType CHAMPTYPE = m_vAllPlayer[i]->GetScript<CUnitScript>()->GetChampType();
			Vec2 Pos = Vec2(m_vAllPlayer[i]->Transform()->GetRelativePos().x - 100.f, m_vAllPlayer[i]->Transform()->GetRelativePos().z + 150.f);
			Pos *= MapSize;
			Minimap->MeshRender()->GetMaterial(0)->SetScalarParam((SCALAR_PARAM)(24 + i), &Pos);

			switch ((UINT)CHAMPTYPE)
			{
			case (UINT)ChampionType::JINX:
				Minimap->MeshRender()->GetMaterial(0)->SetTexParam((TEX_PARAM)(2 + i), CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\jinx_circle.png"));
				break;
			case (UINT)ChampionType::MALPHITE:
				Minimap->MeshRender()->GetMaterial(0)->SetTexParam((TEX_PARAM)(2 + i), CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\malphite_circle.png"));
				break;
			case (UINT)ChampionType::AMUMU:
				Minimap->MeshRender()->GetMaterial(0)->SetTexParam((TEX_PARAM)(2 + i), CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\amumu_circle_0.png"));
				break;
			case (UINT)ChampionType::BLITZCRANK:
				Minimap->MeshRender()->GetMaterial(0)->SetTexParam((TEX_PARAM)(2 + i), CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\steamgolem_circle.png"));
				break;
			}
		}
	}
}

void CMinimapUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CMinimapUIScript::CMinimapUIScript()
	:CScript((UINT)SCRIPT_TYPE::MINIMAPUISCRIPT)
{
}

CMinimapUIScript::~CMinimapUIScript()
{
}
