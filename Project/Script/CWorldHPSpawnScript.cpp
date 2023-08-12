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
	//의문: 이시점에서는 이미 아더플레이어 인포에 모든 플레이어들이 무조건 다들어와있어야함
	//비긴이 모든플레이어가 접속 된 후에 호출되어야 (벡터에 모든플레이어의 정보가 들어있어야) 
	//모두의 체력바를 보여줄수있음 이쪽 주의를해보도록 하자.
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
		// Vector타입: 모든 플레이어를 서버에서 가져옴->CSendServerEventMgr::GetInst()->GetVecAllPlayer();
		// 내 플레이어를 서버에서 가져옴 둘의주소값비교해서 같으면 그것이 나임 CSendServerEventMgr::GetInst()->GetMyPlayer();
	}

	//메인 플레이어
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
		//메인 플레이어
		UISpawn(m_MyPlayer, m_MyPlayerWorldBar);

		//아더 플레이어
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


	//==========닉네임, 레벨 폰트 출력==============
	CUnitScript* UnitScript = _PlayerObj->GetScript<CUnitScript>();
	wstring NickName = UnitScript->GetNickname();


	Vec2 FontDefaultPos = Vec2(worldVec.x + (Resolution.x / 2), worldVec.y + (Resolution.y / 2));
	Vec2 FontDisPlayPos = Vec2(FontDefaultPos.x, FontDefaultPos.y - 150.f);



	//지금 이함수는 아더플레이어, 메인플레이어 모두에게 정보를 받아 시행됩니당~
	// 
	// 
	// 
	//===========다혜님 요기!============
	DisplayLastCCEffect(_PlayerObj, NickName, FontDisPlayPos);
	//==================================


	tFont Font3 = {};
	Font3.wInputText = to_wstring(UnitScript->GetLevel()); //레벨 폰트
	Font3.fontType = FONT_TYPE::RIX_KOR_L;
	Font3.fFontSize = 13.2;
	Font3.vDisplayPos = Vec2(FontDefaultPos.x - 56.f, FontDefaultPos.y - 127.5f);
	Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
	UICamera->AddText(FONT_DOMAIN::OPAQE, Font3);
}


void CWorldHPSpawnScript::DisplayLastCCEffect(CGameObject* _PlayerObj, const wstring& nickname, Vec2 _DisplayPos)
{
	// 다른 함수에서
	uint32_t m_eCurCC = _PlayerObj->GetScript<CUnitScript>()->GetCC();
	wstring highestPriorityCC = GetHighestPriorityCC(m_eCurCC, nickname);

	tFont Font2 = {};
	Font2.wInputText = highestPriorityCC; // 원래 여기에 닉네임 가져와야함
	Font2.fontType = FONT_TYPE::RIX_KOR_L;
	Font2.fFontSize = 13.5;
	Font2.vDisplayPos = _DisplayPos;
	Font2.iFontColor = FONT_RGBA(252, 252, 250, 255);
	UICamera->AddText(FONT_DOMAIN::OPAQE, Font2);
	return; // 출력이 완료되면 함수 종료
}

wstring CWorldHPSpawnScript::GetHighestPriorityCC(UINT m_eCurCC, wstring _NickName)
{
	// 우선순위 순서대로 CC 타입을 검사
	if (m_eCurCC & CC::AIRBORNE) {
		return L"공중에 뜸";
	}
	if (m_eCurCC & CC::STUN) {
		return L"기절";
	}
	if (m_eCurCC & CC::ROOT) {
		return L"속박";
	}
	if (m_eCurCC & CC::SILENCE) {
		return L"침묵";
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
