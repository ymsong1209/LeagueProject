#include "pch.h"
#include "CMinimapUIScript.h"
#include <Engine\CLevelMgr.h>

void CMinimapUIScript::begin()
{
	CGameObject* Map = new CGameObject;
	Map->SetName(L"ParentMap");
	Map->AddComponent(new CTransform);
	Map->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 0.1f));
	SpawnGameObject(Map, Vec3(163.9f, -90.5f, 20.f), 31);

	CGameObject* MinimapFrame = new CGameObject;
	MinimapFrame->SetName(L"MiniMapFrame");
	MinimapFrame->AddComponent(new CMeshRender);
	MinimapFrame->AddComponent(new CTransform);
	MinimapFrame->Transform()->SetRelativeScale(Vec3(63.f, 63.f, 0.1f));
	MinimapFrame->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
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
	Minimap->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 0.1f));
	Minimap->Transform()->SetAbsolute(true);
	Minimap->Transform()->SetRelativePos(Vec3(1.f, 0.f, 27.f));
	Minimap->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Minimap->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MiniMapMtrl"), 0);
	Minimap->MeshRender()->GetDynamicMaterial(0);
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\MiniMap.dds"));
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"FogFilterMap"));
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\jinx_circle.png"));
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\malphite_circle.png"));
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_4, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\soraka_circle_15.png"));
	Minimap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_5, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud\\vayne_circle_11.png"));
	Map->AddChild(Minimap);

	CLevel* CurLevel = CUR_LEVEL;

	Player1 = CurLevel->FindObjectByName(L"Jinx");
	Player2 = CurLevel->FindObjectByName(L"Jinx2");
	Player3 = CurLevel->FindObjectByName(L"Jinx3");
	Player4 = CurLevel->FindObjectByName(L"Jinx4");

}

void CMinimapUIScript::tick()
{
	if (Player1 && Player2 && Player3 && Player4) //나중에는 플레이어별로 있는지 없는지 검사해야할듯
	{
		Vec2 Pos1 = Vec2(Player1->Transform()->GetRelativePos().x -100.f, Player1->Transform()->GetRelativePos().z + 150.f);
		Vec2 Pos2 = Vec2(Player2->Transform()->GetRelativePos().x - 100.f, Player2->Transform()->GetRelativePos().z + 150.f);
		Vec2 Pos3 = Vec2(Player3->Transform()->GetRelativePos().x - 100.f, Player3->Transform()->GetRelativePos().z + 150.f);
		Vec2 Pos4 = Vec2(Player4->Transform()->GetRelativePos().x - 100.f, Player4->Transform()->GetRelativePos().z + 150.f);

		Vec2 MapSize = Vec2(0.232727f, 0.232727f);

		Pos1 *= MapSize;
		Pos2 *= MapSize;
		Pos3 *= MapSize;
		Pos4 *= MapSize;

		Minimap->MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_0, &Pos1);
		Minimap->MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_1, &Pos2);
		Minimap->MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_2, &Pos3);
		Minimap->MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_3, &Pos4);
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
