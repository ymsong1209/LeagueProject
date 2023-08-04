#include "pch.h"
#include "CScorePanelScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <sstream>
#include <iomanip>

void CScorePanelScript::begin()
{
	CGameObject* ScorePanel = new CGameObject; //캐릭터 패널 배치
	ScorePanel->SetName(L"ScorePanel");
	ScorePanel->AddComponent(new CTransform);
	ScorePanel->AddComponent(new CMeshRender);
	ScorePanel->Transform()->SetRelativeScale(Vec3(550.f, 30.f, 20.f));
	ScorePanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ScorePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ScorePanel2.mtrl"), 0);
	ScorePanel->Transform()->SetAbsolute(true);
	SpawnGameObject(ScorePanel, Vec3(533.f, 466.f, 2.f), 31);
}

void CScorePanelScript::tick()
{
	//==========킬스코어 패널 폰트 출력==============
	// 필요한 정보:
	float runTime = WorldInfo.runningTime;
	int blueScore = WorldInfo.blueScore;
	int redScore = WorldInfo.redScore;
	int MyKill = MyInfo.mykill;
	int MyDeath = MyInfo.mydeath;
	int MyCS = MyInfo.myCS;
	//=======디버깅용===============
	blueScore = 5;
	redScore = 12;
	MyKill = 12;
	MyDeath = 4;
	MyCS = 57;
	//===============블루팀/레드팀=====================


	tFont Font2 = {};
	Font2.wInputText = to_wstring(blueScore); // 원래 여기에 닉네임 가져와야함
	Font2.fontType = FONT_TYPE::RIX_KOR_L;
	Font2.fFontSize = 16.7;
	Font2.vDisplayPos = Vec2(1263, 20);
	Font2.iFontColor = FONT_RGBA(0, 198, 237, 255);
	UICamera->AddText(FONT_DOMAIN::MASK, Font2);

	tFont Font3 = {};
	Font3.wInputText = to_wstring(redScore); //레벨 폰트
	Font3.fontType = FONT_TYPE::RIX_KOR_L;
	Font3.fFontSize = 16.7;
	Font3.vDisplayPos = Vec2(1307, 21);
	Font3.iFontColor = FONT_RGBA(255, 54, 54, 255);
	UICamera->AddText(FONT_DOMAIN::MASK, Font3);
	//========================================

	wstring wKill = to_wstring(MyKill);
	//wstring wKill = to_wstring(3);
	wstring Slash = L"/";
	wstring wDeath = to_wstring(MyDeath);
	wstring zero = L"/0";
	//wstring wDeath = to_wstring(4);
	wstring KillDeath = wKill + Slash + wDeath + zero;
	//================킬/뎃/어시======================
	tFont Font4 = {};
	Font4.wInputText = KillDeath; // 원래 여기에 닉네임 가져와야함
	Font4.fontType = FONT_TYPE::RIX_KOR_L;
	Font4.fFontSize = 15;
	Font4.vDisplayPos = Vec2(1412,22);
	Font4.iFontColor = FONT_RGBA(242, 241, 237, 255);
	UICamera->AddText(FONT_DOMAIN::MASK, Font4);
	//========================================

	//=================미니언=====================
	tFont Font5 = {};
	Font5.wInputText = to_wstring(MyCS); //레벨 폰트
	Font5.fontType = FONT_TYPE::RIX_KOR_L;
	Font5.fFontSize = 15;
	Font5.vDisplayPos = Vec2(1490,22);
	Font5.iFontColor = FONT_RGBA(252, 230, 139, 255);
	UICamera->AddText(FONT_DOMAIN::MASK, Font5);
	//========================================

	//================진행시간========================
	PlayTime += DT; //<=== 원래는 서버에서 받아와야하나? 아니면 그냥 내가 해줘도되나?
	//float로 들어오는 시간 분/초 로 변환

	int minutes = static_cast<int>(PlayTime) / 60;
	int seconds = static_cast<int>(PlayTime) % 60;

	wstringstream ss;
	ss << setw(2) << setfill(L'0') << minutes << L":"
		<< setw(2) << setfill(L'0') << seconds;
	wstring time = ss.str();

	tFont Font6 = {};
	Font6.wInputText = time; // 원래 여기에 닉네임 가져와야함
	Font6.fontType = FONT_TYPE::RIX_KOR_L;
	Font6.fFontSize = 15;
	Font6.vDisplayPos = Vec2(1570,22);
	Font6.iFontColor = FONT_RGBA(242, 241, 237, 255);
	UICamera->AddText(FONT_DOMAIN::MASK, Font6);
	//========================================
}

void CScorePanelScript::BeginOverlap(CCollider2D* _Other)
{
}

CScorePanelScript::CScorePanelScript()
	:CScript((UINT)SCRIPT_TYPE::SCOREPANELSCRIPT)
	, PlayTime(0.f)
{


}

CScorePanelScript::~CScorePanelScript()
{
}
