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
	info.team = 0;
	info.Level = 18;
	info.NickName = L"�ٿ뵵 ����";
	m_vOtherplayerInfo.push_back(info);

	for (size_t i = 0; i < m_vOtherplayerInfo.size(); ++i)
	{
		CGameObject* WorldBar = new CGameObject;
		WorldBar->SetName(L"WorldBar");
		WorldBar->AddComponent(new CTransform);
		WorldBar->AddComponent(new CMeshRender);
		WorldBar->AddComponent(new CWorldHPUIScript);
		WorldBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		switch (m_vOtherplayerInfo[i].team)
		{
		case 0: //blue
			WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueWorldBar133.mtrl"), 0);
			break;
		case 1: //red
			WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedWorldBar133.mtrl"), 0);
			break;
		case 2: //player
			WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldBar133.mtrl"), 0);
			break;
		}

		WorldBar->MeshRender()->SetRaySightCulling(false);
		WorldBar->Transform()->SetRelativeScale(Vec3(133.f, 29.f, 17.02f));
		//WorldBar->Transform()->SetRelativeScale(Vec3(34.83f, 7.594f, 17.02f));
		//WorldBar->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
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
		CCamera* UICam = CRenderMgr::GetInst()->GetCamerafromIdx(1);
		if (CUR_LEVEL->GetState() == LEVEL_STATE::PLAY && UICam)
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
			Vec4 ndcVec = Vec4((2.0f * ObjscreenPos.x) / Resolution.x - 1.0f, 1.0f - (2.0f * ObjscreenPos.y) / Resolution.y, 1.f, 1.f);

			// Get the inverse of the view-projection matrix
			Matrix viewInvMatrix = UICam->GetViewMatInv();
			Matrix projInvMatrix = UICam->GetProjMatInv();
			Matrix invViewProjMatrix = viewInvMatrix * projInvMatrix;

			// Transform to world coordinates
			Vec3 worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);

			Vec3 OffsetPos = Vec3(-3.f, 117.f, 0.f);
			Vec3 FinalPos = Vec3(worldVec.x + OffsetPos.x, -worldVec.y + OffsetPos.y, 700.f);
			m_vWorldBar[i]->Transform()->SetRelativePos(FinalPos);


			//==========�г���, ���� ��Ʈ ���==============
			Vec2 FontDefaultPos = Vec2(worldVec.x + (Resolution.x / 2), worldVec.y + (Resolution.y / 2));

			tFont Font2 = {};
			Font2.wInputText = m_vOtherplayerInfo[i].NickName; // ���� ���⿡ �г��� �����;���
			Font2.fontType = FONT_TYPE::RIX_KOR_L;
			Font2.fFontSize = 13.5;
			Font2.vDisplayPos = Vec2(FontDefaultPos.x, FontDefaultPos.y - 150.f);
			Font2.iFontColor = FONT_RGBA(252, 252, 250, 255);
			UICam->AddText(FONT_DOMAIN::OPAQE, Font2);

			tFont Font3 = {};
			Font3.wInputText = to_wstring(m_vOtherplayerInfo[i].Level); //���� ��Ʈ
			Font3.fontType = FONT_TYPE::RIX_KOR_L;
			Font3.fFontSize = 13.2;
			Font3.vDisplayPos = Vec2(FontDefaultPos.x - 56.f, FontDefaultPos.y - 127.5f);
			Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
			UICam->AddText(FONT_DOMAIN::OPAQE, Font3);
		}
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
