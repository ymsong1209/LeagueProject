#include "pch.h"
#include "CChampionDeathState.h"
#include <Engine/CFsm.h>
#include <Engine/CEngine.h>
#include <Engine/CResMgr.h>
#include <Engine/CRenderMgr.h>

CChampionDeathState::CChampionDeathState()
{
}

CChampionDeathState::~CChampionDeathState()
{
}

void CChampionDeathState::tick()
{
}

void CChampionDeathState::Enter()
{
	Vec2 WindowResolution = CEngine::GetInst()->GetWindowResolution();

	CGameObject* DeathPostprocess = new CGameObject;
	DeathPostprocess->SetName(L"Gray_Shader");
	DeathPostprocess->AddComponent(new CTransform);
	DeathPostprocess->AddComponent(new CMeshRender);
 
	DeathPostprocess->MeshRender()->SetFrustumCheck(false);

	DeathPostprocess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	DeathPostprocess->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl"), 0);
	DeathPostprocess->MeshRender()->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"GrayShader"));
	DeathPostprocess->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"RTCopyTex"));

	m_pTempGrayShaderObj = DeathPostprocess;


	SpawnGameObject(DeathPostprocess, Vec3(0.f, 0.f, 0.f), 0);
	
	
	CUnitState::Enter();
}

void CChampionDeathState::Exit()
{
	if (m_pTempGrayShaderObj != nullptr)
		DestroyObject(m_pTempGrayShaderObj);

	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	Vec3 OriginMainCamPos = MainCam->Transform()->GetRelativePos();
	Vec2 WindowResolution = CEngine::GetInst()->GetWindowResolution();
	MainCam->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos().x, OriginMainCamPos.y, GetOwner()->Transform()->GetRelativePos().z - WindowResolution.y / 4.f);

	CUnitState::Exit();
}

void CChampionDeathState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_RESPAWN:
	{
		// 우물로 이동
		GetOwnerFSM()->ChangeState(L"Respawn");
		break;
	}
	}
}
