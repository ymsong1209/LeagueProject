#include "pch.h"
#include "CMinionHPBarPosScript.h"
#include "CUnitScript.h"

CMinionHPBarPosScript::CMinionHPBarPosScript()
	:CScript((UINT)SCRIPT_TYPE::MINIONHPBARPOSSCRIPT)
{
}

CMinionHPBarPosScript::~CMinionHPBarPosScript()
{
}


void CMinionHPBarPosScript::begin()
{
	Transform()->SetAbsolute(false);
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MinionBarPanel.mtrl"), 0);
}

void CMinionHPBarPosScript::tick()
{
	CGameObject* ParentObj = GetOwner()->GetParent();
	if (!ParentObj->IsDead() && ParentObj)
	{
		Transform()->SetRelativePos(Vec3(0.f, 100.f, 50.f));
		Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
	}
}

void CMinionHPBarPosScript::BeginOverlap(CCollider2D* _Other)
{
}


