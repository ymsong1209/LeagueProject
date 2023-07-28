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
		WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldBar133.mtrl"), 0);
		WorldBar->MeshRender()->SetRaySightCulling(false);
		WorldBar->Transform()->SetRelativeScale(Vec3(31.92f, 6.96f, 100.f));
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
		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();

		Matrix viewmat = MainCam->GetViewMat();
		Matrix projmat = MainCam->GetProjMat();
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

		// Create a vector in normalized device coordinates
		Vec4 ndcVec = Vec4((2.0f * ObjscreenPos.x) / Resolution.x - 1.0f, 1.0f - (2.0f * ObjscreenPos.y) / Resolution.y, 1.f,1.f);
		CCamera* UICam = CRenderMgr::GetInst()->GetCamerafromIdx(1);

		// Get the inverse of the view-projection matrix
		Matrix viewInvMatrix = UICam->GetViewMatInv();  // This should be your current view matrix
		Matrix projInvMatrix = UICam->GetProjMatInv();  // This should be your current projection matrix
		Matrix invViewProjMatrix = viewInvMatrix * projInvMatrix;

		// Transform to world coordinates
		Vec3 worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);
		m_vWorldBar[i]->Transform()->SetRelativePos(Vec3(worldVec.x- 3.f, -worldVec.y + 30.f, 700.f));
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
