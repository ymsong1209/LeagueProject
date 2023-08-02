#include "pch.h"
#include "CMouseCursorUIScript.h"
#include <Engine\CKeyMgr.h>

void CMouseCursorUIScript::begin()
{
	
}

void CMouseCursorUIScript::tick()
{
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();

}

void CMouseCursorUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CMouseCursorUIScript::CMouseCursorUIScript()
	:CScript((UINT)SCRIPT_TYPE::MOUSECURSORUISCRIPT)
{
	//CGameObject* MouseObj = new CGameObject;
	//MouseObj->AddComponent(new CTransform);
	//MouseObj->AddComponent(new CMeshRender);
	//MouseObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//MouseObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L""), 0);
	//MouseObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	//SpawnGameObject(MouseObj, Vec3(100.f, 100.f, 100.f),31);

}

CMouseCursorUIScript::~CMouseCursorUIScript()
{
}
