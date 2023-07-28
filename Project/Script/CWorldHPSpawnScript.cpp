#include "pch.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CEngine.h>

void CWorldHPSpawnScript::begin()
{
	//�ǹ�: �̽��������� �̹� �ƴ��÷��̾� ������ ��� �÷��̾���� ������ �ٵ����־����
	//����� ����÷��̾ ���� �� �Ŀ� ȣ��Ǿ�� (���Ϳ� ����÷��̾��� ������ ����־��) 

	//����� ü�¹ٸ� �����ټ����� ���� ���Ǹ��غ����� ����.
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

		//---------����� Ÿ�ٹ�ü�� ȭ����� ��ǥ�� ����
		Matrix viewmat = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
		Matrix projmat = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();
		Matrix MatViewProj = viewmat * projmat;

		// 4D ���ͷ� ��ȯ (w ��Ҹ� 1�� ����)
		Vec4 Pos4 = Vec4(Pos.x, Pos.y, Pos.z, 1.0f);

		// MatViewProj�� ������
		Vec4 ProjPos = Pos4.Transform(Pos4, MatViewProj);

		// w�� ������ Ŭ����(ndc) ��ǥ��� ��ȯ
		ProjPos /= ProjPos.w;

		// �� ��������, projPos�� xy ��ǥ�� -1.0���� 1.0 ������ ���� ������ �˴ϴ�.
		// �� ��ǥ�� ����ؼ� ��ũ�� ��ǥ�� ����Ϸ��� ����Ʈ ũ�⸦ �˾ƾ� �մϴ�.
		Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();  //ȭ�� �ػ󵵸� ������
		Vec2 screenSize(Resolution.x, Resolution.y);
		Vec2 ObjscreenPos = ((Vec2(ProjPos.x, ProjPos.y) + Vec2(1.f, 1.f)) / Vec2(2.f, 2.f)) * screenSize;
		//������Ʈ�� ��ũ������ ��ġ���� ����

		// DirectX�� ���, y ��ǥ�� ������ �Ʒ��� �����ϹǷ�, y ��ǥ�� ������ �ʿ䰡 ����.
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
