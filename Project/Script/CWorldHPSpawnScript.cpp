#include "pch.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CEngine.h>

void CWorldHPSpawnScript::begin()
{
	//의문: 이시점에서는 이미 아더플레이어 인포에 모든 플레이어들이 무조건 다들어와있어야함
	//비긴이 모든플레이어가 접속 된 후에 호출되어야 (벡터에 모든플레이어의 정보가 들어있어야) 

	//모두의 체력바를 보여줄수있음 이쪽 주의를해보도록 하자.
	CGameObject* Jinx = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Jinx");
	tWorldHPInfo info = {};
	info.Pos = Vec3(100.f, 100.f, 100.f);
	info.tHP = 30.f;
	info.tMP = 75.f;
	info.tTotalHP = 100.f;
	info.tTotalMP = 100.f;

	m_vOtherplayerInfo.push_back(info);

	for (size_t i = 0; i < m_vOtherplayerInfo.size(); ++i)
	{
		CGameObject* WorldBar = new CGameObject;
		WorldBar->SetName(L"WorldBar");
		WorldBar->AddComponent(new CTransform);
		WorldBar->AddComponent(new CMeshRender);
		WorldBar->AddComponent(new CWorldHPUIScript);
		WorldBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldBar.mtrl"), 0);
		WorldBar->MeshRender()->SetRaySightCulling(false);
		WorldBar->Transform()->SetRelativeScale(Vec3(36.72f, 7.83f, 100.f));
		WorldBar->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
		Vec3 Pos = m_vOtherplayerInfo[i].Pos;

		CGameObject* Jinx = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Jinx");
		SpawnGameObject(WorldBar, Jinx->Transform()->GetRelativePos(), 31);
		m_vWorldBar.push_back(WorldBar);
	}
}

void CWorldHPSpawnScript::tick()
{
	for (size_t i = 0; i < m_vOtherplayerInfo.size(); ++i)
	{
		CGameObject* Jinx = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Jinx");
		Vec3 Pos = Jinx->Transform()->GetRelativePos();

		//---------기즈모 타겟물체의 화면상의 좌표를 구함
		Matrix viewmat = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
		Matrix projmat = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();
		Matrix MatViewProj = viewmat * projmat;

		// 4D 벡터로 변환 (w 요소를 1로 설정)
		Vec4 Pos4 = Vec4(Pos.x, Pos.y, Pos.z, 1.0f);

		// MatViewProj에 곱해줌
		Vec4 ProjPos = Pos4.Transform(Pos4, MatViewProj);

		// w로 나눠서 클리핑(ndc) 좌표계로 변환
		ProjPos /= ProjPos.w;

		// 이 시점에서, projPos의 xy 좌표는 -1.0에서 1.0 사이의 값을 가지게 됩니다.
		// 이 좌표를 사용해서 스크린 좌표를 계산하려면 뷰포트 크기를 알아야 합니다.
		Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();  //화면 해상도를 가져옴
		Vec2 screenSize(Resolution.x, Resolution.y);
		Vec2 ObjscreenPos = ((Vec2(ProjPos.x, ProjPos.y) + Vec2(1.f, 1.f)) / Vec2(2.f, 2.f)) * screenSize;
		//오브젝트의 스크린상의 위치값을 구함

		// DirectX의 경우, y 좌표는 위에서 아래로 증가하므로, y 좌표를 뒤집을 필요가 있음.
		//ObjscreenPos.y = screenSize.y - ObjscreenPos.y;

		//CRenderMgr::GetInst()->GetCamerafromIdx(1)->SetProjType(PROJ_TYPE::PERSPECTIVE);
		//CRenderMgr::GetInst()->GetCamerafromIdx(1)->CalcViewMat();
		//CRenderMgr::GetInst()->GetCamerafromIdx(1)->CalcProjMat();
		//m_vWorldBar[i]->Transform()->SetRelativePos(Vec3(ObjscreenPos.x, ObjscreenPos.y, 100.f));
		//m_vWorldBar[i]->Transform()->finaltick();
		//CRenderMgr::GetInst()->GetCamerafromIdx(1)->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);


		float vpX = ObjscreenPos.x;
		float vpY = ObjscreenPos.y;
		float vpZ = 1.f;  // This is typically obtained from the depth buffer

		// viewport size
		float vpWidth = Resolution.x;
		float vpHeight = Resolution.y;

		// Convert to normalized device coordinates
		float ndcX = (2.0f * vpX) / vpWidth - 1.0f;
		float ndcY = 1.0f - (2.0f * vpY) / vpHeight;
		float ndcZ = vpZ;  // This remains the same

		// Create a vector in normalized device coordinates
		XMVECTOR ndcVec = XMVectorSet(ndcX, ndcY, ndcZ, 1.0f);

		// Get the inverse of the view-projection matrix
		XMMATRIX viewInvMatrix = CRenderMgr::GetInst()->GetCamerafromIdx(1)->GetViewMatInv();  // This should be your current view matrix
		XMMATRIX projInvMatrix = CRenderMgr::GetInst()->GetCamerafromIdx(1)->GetProjMatInv();  // This should be your current projection matrix
		XMMATRIX invViewProjMatrix = viewInvMatrix * projInvMatrix;

		// Transform to world coordinates
		XMVECTOR worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);

		// Extract the world coordinates from the vector
		float worldX = XMVectorGetX(worldVec);
		float worldY = XMVectorGetY(worldVec);
		float worldZ = XMVectorGetZ(worldVec);

		m_vWorldBar[i]->Transform()->SetRelativePos(Vec3(XMVectorGetX(worldVec) - 3.f, -XMVectorGetY(worldVec) + 30.f, 1.f));
	}
}

void CWorldHPSpawnScript::BeginOverlap(CCollider2D* _Other)
{
}

CWorldHPSpawnScript::CWorldHPSpawnScript()
	:CScript((UINT)SCRIPT_TYPE::WORLDHPSPAWNSCRIPT)
{
}

CWorldHPSpawnScript::~CWorldHPSpawnScript()
{
}
