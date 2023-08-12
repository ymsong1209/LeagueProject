#include "pch.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CEngine.h>
#include "CSendServerEventMgr.h"
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CTimedEffect.h"

void CWorldHPSpawnScript::begin()
{
	//�ǹ�: �̽��������� �̹� �ƴ��÷��̾� ������ ��� �÷��̾���� ������ �ٵ����־����
	//����� ����÷��̾ ���� �� �Ŀ� ȣ��Ǿ�� (���Ϳ� ����÷��̾��� ������ ����־��) 
	//����� ü�¹ٸ� �����ټ����� ���� ���Ǹ��غ����� ����.
	vector<CGameObject*> m_vecAllPlayer = CSendServerEventMgr::GetInst()->GetVecAllPlayer();
	for (size_t i = 0; i < m_vecAllPlayer.size(); ++i)
	{
		if (CSendServerEventMgr::GetInst()->GetMyPlayer() == m_vecAllPlayer[i])
			m_MyPlayer = m_vecAllPlayer[i];
		else
			m_OtherPlayer.push_back(m_vecAllPlayer[i]);
	}

	for (size_t i = 0; i < m_OtherPlayer.size(); ++i)
	{
		CGameObject* WorldBar = new CGameObject;
		WorldBar->SetName(L"OtherPlayer_WorldBar");
		WorldBar->AddComponent(new CTransform);
		WorldBar->AddComponent(new CMeshRender);
		WorldBar->AddComponent(new CWorldHPUIScript(m_OtherPlayer[i]));
		WorldBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		switch ((UINT)m_OtherPlayer[i]->GetScript<CUnitScript>()->GetFaction())
		{
		case (UINT)Faction::BLUE: //blue
			WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueWorldBar133.mtrl"), 0);
			break;
		case (UINT)Faction::RED:  //red
			WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedWorldBar133.mtrl"), 0);
			break;
		}

		WorldBar->Transform()->SetRelativeScale(Vec3(133.f, 29.f, 17.02f));
		Vec3 Pos = m_OtherPlayer[i]->Transform()->GetRelativePos();
		SpawnGameObject(WorldBar, Pos, 31);
		m_vOtherWorldBar.push_back(WorldBar);
		// VectorŸ��: ��� �÷��̾ �������� ������->CSendServerEventMgr::GetInst()->GetVecAllPlayer();
		// �� �÷��̾ �������� ������ �����ּҰ����ؼ� ������ �װ��� ���� CSendServerEventMgr::GetInst()->GetMyPlayer();
	}

	//���� �÷��̾�
	CGameObject* WorldBar = new CGameObject;
	WorldBar->SetName(L"MyPlayer_WorldBar");
	WorldBar->AddComponent(new CTransform);
	WorldBar->AddComponent(new CMeshRender);
	WorldBar->AddComponent(new CWorldHPUIScript(m_MyPlayer));
	WorldBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldBar133.mtrl"), 0);
	WorldBar->Transform()->SetRelativeScale(Vec3(133.f, 29.f, 17.02f));
	Vec3 Pos = m_MyPlayer->Transform()->GetRelativePos();
	SpawnGameObject(WorldBar, Pos, 31);
	m_MyPlayerWorldBar = WorldBar;
}

void CWorldHPSpawnScript::tick()
{
	if (CUR_LEVEL->GetState() == LEVEL_STATE::PLAY && UICamera)
	{
		//���� �÷��̾�
		UISpawn(m_MyPlayer, m_MyPlayerWorldBar);

		//�ƴ� �÷��̾�
		for (size_t i = 0; i < m_OtherPlayer.size(); ++i)
		{
			CCamera* UICam = CRenderMgr::GetInst()->GetCamerafromIdx(1);
			UISpawn(m_OtherPlayer[i], m_vOtherWorldBar[i]);

		}
	}
}

void CWorldHPSpawnScript::BeginOverlap(CCollider2D* _Other)
{
}

void CWorldHPSpawnScript::UISpawn(CGameObject* _PlayerObj, CGameObject* _WorldBarObj)
{
	bool IsCull = _PlayerObj->GetRenderComponent()->IsCulled();
	bool UseRaySight = _PlayerObj->GetRenderComponent()->IsUsingRaySightCulling();
	if (IsCull && UseRaySight)
	{
		return;
	}

	Vec3 Pos = _PlayerObj->Transform()->GetRelativePos();
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
	Matrix viewInvMatrix = UICamera->GetViewMatInv();
	Matrix projInvMatrix = UICamera->GetProjMatInv();
	Matrix invViewProjMatrix = viewInvMatrix * projInvMatrix;

	// Transform to world coordinates
	Vec3 worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);

	Vec3 OffsetPos = Vec3(-3.f, 117.f, 0.f);
	Vec3 FinalPos = Vec3(worldVec.x + OffsetPos.x, -worldVec.y + OffsetPos.y, 700.f);
	_WorldBarObj->Transform()->SetRelativePos(FinalPos);


	//==========�г���, ���� ��Ʈ ���==============
	CUnitScript* UnitScript = _PlayerObj->GetScript<CUnitScript>();
	wstring NickName = UnitScript->GetNickname();


	Vec2 FontDefaultPos = Vec2(worldVec.x + (Resolution.x / 2), worldVec.y + (Resolution.y / 2));
	Vec2 FontDisPlayPos = Vec2(FontDefaultPos.x, FontDefaultPos.y - 150.f);



	//���� ���Լ��� �ƴ��÷��̾�, �����÷��̾� ��ο��� ������ �޾� ����˴ϴ�~
	// 
	// 
	// 
	//===========������ ���!============
	DisplayLastCCEffect(_PlayerObj, NickName, FontDisPlayPos);
	//==================================


	tFont Font3 = {};
	Font3.wInputText = to_wstring(UnitScript->GetLevel()); //���� ��Ʈ
	Font3.fontType = FONT_TYPE::RIX_KOR_L;
	Font3.fFontSize = 13.2;
	Font3.vDisplayPos = Vec2(FontDefaultPos.x - 56.f, FontDefaultPos.y - 127.5f);
	Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
	UICamera->AddText(FONT_DOMAIN::OPAQE, Font3);
}


void CWorldHPSpawnScript::DisplayLastCCEffect(CGameObject* _PlayerObj, const wstring& nickname, Vec2 _DisplayPos)
{
	// �ٸ� �Լ�����
	uint32_t m_eCurCC = _PlayerObj->GetScript<CUnitScript>()->GetCC();
	wstring highestPriorityCC = GetHighestPriorityCC(m_eCurCC, nickname);

	tFont Font2 = {};
	Font2.wInputText = highestPriorityCC; // ���� ���⿡ �г��� �����;���
	Font2.fontType = FONT_TYPE::RIX_KOR_L;
	Font2.fFontSize = 13.5;
	Font2.vDisplayPos = _DisplayPos;
	Font2.iFontColor = FONT_RGBA(252, 252, 250, 255);
	UICamera->AddText(FONT_DOMAIN::OPAQE, Font2);
	return; // ����� �Ϸ�Ǹ� �Լ� ����
}

wstring CWorldHPSpawnScript::GetHighestPriorityCC(UINT m_eCurCC, wstring _NickName)
{
	// �켱���� ������� CC Ÿ���� �˻�
	if (m_eCurCC & CC::AIRBORNE) {
		return L"���߿� ��";
	}
	if (m_eCurCC & CC::STUN) {
		return L"����";
	}
	if (m_eCurCC & CC::ROOT) {
		return L"�ӹ�";
	}
	if (m_eCurCC & CC::SILENCE) {
		return L"ħ��";
	}
	if (m_eCurCC & CC::SLOW) {
		return _NickName;
	}
	return _NickName;
}

CWorldHPSpawnScript::CWorldHPSpawnScript()
	:CScript((UINT)SCRIPT_TYPE::WORLDHPSPAWNSCRIPT)
{
}

CWorldHPSpawnScript::~CWorldHPSpawnScript()
{
	m_OtherPlayer.clear();
}
