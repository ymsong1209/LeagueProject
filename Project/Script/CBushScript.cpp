#include "pch.h"
#include "CBushScript.h"
#include "CSendServerEventMgr.h"
#include "CUnitScript.h"

CBushScript::CBushScript()
	:CScript((UINT)SCRIPT_TYPE::BUSHSCRIPT)
{
}

CBushScript::~CBushScript()
{
}


void CBushScript::begin()
{
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
	}
	GetOwner()->Collider3D()->SetIsWall(true);
}

void CBushScript::tick()
{
}

void CBushScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetLayerIndex()) {
		CUnitScript* Otherscript = _Other->GetOwner()->GetScript<CUnitScript>();
		CUnitScript* MyPlayerScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
		if (Otherscript->GetFaction() == MyPlayerScript->GetFaction()) {
			GetOwner()->Collider3D()->SetIsWall(false);
			int mtrlcount = _Other->MeshRender()->GetMtrlCount();
			float alpha = 0.5f;
			for (int i = 0; i < mtrlcount; ++i) {
				_Other->MeshRender()->GetMaterial(i)->GetShader()->SetBSType(BS_TYPE::ALPHA_BLEND);
				_Other->MeshRender()->GetMaterial(i)->SetScalarParam(FLOAT_9, &alpha);
			}
			
		}
	}
}

void CBushScript::OnOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetLayerIndex()) {
		CUnitScript* Otherscript = _Other->GetOwner()->GetScript<CUnitScript>();
		CUnitScript* MyPlayerScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
		if (Otherscript->GetFaction() == MyPlayerScript->GetFaction()) {
			GetOwner()->Collider3D()->SetIsWall(false);
			int mtrlcount = _Other->MeshRender()->GetMtrlCount();
			float alpha = 0.5f;
			for (int i = 0; i < mtrlcount; ++i) {
				_Other->MeshRender()->GetMaterial(i)->GetShader()->SetBSType(BS_TYPE::ALPHA_BLEND);
				_Other->MeshRender()->GetMaterial(i)->SetScalarParam(FLOAT_9, &alpha);
			}
		}
	}
}

void CBushScript::EndOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetLayerIndex()) {
		CUnitScript* Otherscript = _Other->GetOwner()->GetScript<CUnitScript>();
		CUnitScript* MyPlayerScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
		if (Otherscript->GetFaction() == MyPlayerScript->GetFaction()) {
			GetOwner()->Collider3D()->SetIsWall(true);
			int mtrlcount = _Other->MeshRender()->GetMtrlCount();
			float alpha = 1.f;
			for (int i = 0; i < mtrlcount; ++i) {
				_Other->MeshRender()->GetMaterial(i)->GetShader()->SetBSType(BS_TYPE::MASK);
				_Other->MeshRender()->GetMaterial(i)->SetScalarParam(FLOAT_9, &alpha);
			}
		}
	}
}

