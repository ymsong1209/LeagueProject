#include "pch.h"
#include "CCharacterUIScript.h"
#include "CUIScript.h"
#include <Engine\CEventMgr.h>
#include "CCoolDownUIScript.h"
#include "CExpRatioUIScript.h"
#include "CHpMpRatioUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include "CSkillLevelUIScript.h"
#include "CSkillLevelUpUIScript.h"
#include "CUnitScript.h"
#include "CChampionScript.h"
#include <Engine\CEngine.h>

CCharacterUIScript::CCharacterUIScript()
	:CUIScript(CHARACTERUISCRIPT)
	, m_iGold(0)
	, m_iPlayerLevel(0)
	, animationDuration(2.f)
	, animationDistance(30.f)
	, m_iPrevChampLevel(1)
{
}

CCharacterUIScript::~CCharacterUIScript()
{
}

void CCharacterUIScript::begin()
{
	SkillUILoad();

	SpellUILoad();

	BarUILoad();

	UISetting();
}

void CCharacterUIScript::tick()
{
	if (UICamera)
	{
		int CurChampGold = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CChampionScript>()->GetGold();
		//======골드 폰트======
		wstring Gold = to_wstring((int)CurChampGold);

		tFont Font3 = {};
		Font3.wInputText = Gold; //골드 폰트
		Font3.fontType = FONT_TYPE::RIX_KOR_L;
		Font3.fFontSize = 16.5;
		Font3.vDisplayPos = Vec2(1030, 974);
		Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font3);
		//=======================
		int CurChampLevel = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CChampionScript>()->GetLevel();
		//======레벨 폰트======
		wstring Level = to_wstring((int)CurChampLevel);

		tFont Font = {};
		Font.wInputText = Level;
		Font.fontType = FONT_TYPE::RIX_KOR_L;
		Font.fFontSize = 13.5;
		Font.vDisplayPos = Vec2(573.4f, 973.f);
		Font.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font);
		//=======================
	}

	//======레벨업 폰트======
	int CurChampLevel = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CChampionScript>()->GetLevel();
	if (m_iPrevChampLevel != CurChampLevel)
	{
		m_bAnimating = true;

		//
		CSound* newSound = new CSound;
		wstring filepath = CPathMgr::GetInst()->GetContentPath();
		filepath += L"sound2d\\sfx_Champ_LevelUp.mp3";
		newSound->Load(filepath);
		CSoundMgr::GetInst()->AddSound(newSound);
		int soundId = newSound->GetSoundIndex();
		CSoundMgr::GetInst()->Play(soundId, 5, 0.5f, true, 0.f, Vec3(0.f,0.f,0.f));
		CSoundMgr::GetInst()->Stop(soundId);
		CSoundMgr::GetInst()->Play(soundId, 5, 0.5f, true, 0.f, Vec3(0.f, 0.f, 0.f));
	}
		
	wstring Level = to_wstring((int)CurChampLevel);

	if (m_bAnimating)
	{
		m_fTimer += DT;

		float progress = min(m_fTimer / animationDuration, 1.f); // 0.0 ~ 1.0 사이의 진행도
		float easeOutProgress = 1 - pow(1 - progress, 2); // ease-out 효과 (시작은 빠르고 점점 느려짐)
		Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();
		Vec3 Pos = UICamera->GetMainPlayerUICamPos();
		Vec2 FontDefaultPos = Vec2(Pos.x + (Resolution.x / 2), -Pos.y + (Resolution.y / 2));
		Vec2 FontDisPlayPos = Vec2(FontDefaultPos.x, FontDefaultPos.y - 50.f);

		Vec3 PlayerPos = Vec3(FontDisPlayPos.x, FontDisPlayPos.y, 1.f);
		Vec3 YOffsetPos = Vec3(0.f, animationDistance * easeOutProgress, 0.f);
		Vec3 FinalPos = PlayerPos - YOffsetPos;
		Vec3 ShadowPos = Vec3(FinalPos.x + 2.f, FinalPos.y + 2.f, FinalPos.z);

		tFont Font2 = {};
		Font2.wInputText = L"레벨 업!";
		Font2.fontType = FONT_TYPE::RIX_KOR_L;
		Font2.fFontSize = 17;
		Font2.vDisplayPos = ShadowPos;
		Font2.iFontColor = FONT_RGBA(0, 0, 0, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font2);

		tFont Font = {};
		Font.wInputText = L"레벨 업!";
		Font.fontType = FONT_TYPE::RIX_KOR_L;
		Font.fFontSize = 17;
		Font.vDisplayPos = FinalPos;
		Font.iFontColor = FONT_RGBA(7, 176, 227, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font);

		if (m_fTimer >= animationDuration) {
			m_bAnimating = false;
			m_fTimer = 0.f;
		}
	}

	m_iPrevChampLevel = CurChampLevel;
}

void CCharacterUIScript::BeginOverlap(CCollider2D* _Other)
{
}

void CCharacterUIScript::SkillUILoad()
{
	//JINX_E
	/* 주 의 !!*/
	//머터리얼 이름은 CHARACTER_TYPE + "_" + SkillNum 형태로 되어있어야함 (ex: MALPHITE_Q)
	//그래야 알아서 캐릭터별로 머터리얼을 찾아옴
	//챔피언 아이콘은  CharacterType _ CIRCLE 임  (JINX_CIRCLE)
	CUIScript::begin();
	wstring UIpath = L"material\\";
	wstring mtrl = L".mtrl";
	wstring under = L"_";
	wstring CharacterType = ChampionType_WSTR[(UINT)GetCharacterType()];
	wstring ChampMtrlPath = UIpath + CharacterType + under + L"CIRCLE" + mtrl;

	//MALPHITE_CIRCLE
	CharacterImage = new CGameObject; //캐릭터 패널 배치
	CharacterImage->SetName(L"UICharacterPanel");
	CharacterImage->AddComponent(new CTransform);
	CharacterImage->AddComponent(new CMeshRender);
	CharacterImage->Transform()->SetRelativeScale(Vec3(82.f, 82.f, 20.f));
	CharacterImage->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CharacterImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(ChampMtrlPath), 0);
	CharacterImage->Transform()->SetAbsolute(true);
	CharacterImage->Transform()->SetRelativePos(Vec3(-250.f, 0.f, 2.f));
	GetUIFrontPanel()->AddChild(CharacterImage);


	for (UINT i = 0; i < (UINT)SkillNum::END; ++i)  //스킬 이미지 Q,W,E,R,Passive 배치 
	{
		SkillNumber = (SkillNum)i;
		wstring SkillName = CharacterType + under + SKILL_TYPE_WSTR[i];
		wstring fullpath = UIpath + SkillName + mtrl;
		CGameObject* Obj = new CGameObject;
		Obj->SetName(SkillName);
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->Transform()->SetRelativeScale(Vec3(45.f, 45.f, 10.f));
		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
		Obj->Transform()->SetAbsolute(true);
		switch (i) //스킬 오브젝트 저장. 
		{
		case 0:
		{
			Obj->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::Q));
			Skill_Q_Image = Obj;
		}
		break;
		case 1:
		{
			Obj->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::W));
			Skill_W_Image = Obj;
		}
		break;
		case 2:
		{
			Obj->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::E));
			Skill_E_Image = Obj;
		}
		break;
		case 3:
		{
			Obj->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::R));
			Skill_R_Image = Obj;
		}
		break;
		case 4:
		{
			Obj->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::PASSIVE));
			Skill_PASSIVE_Image = Obj;
		}
		break;
		}

		Obj->GetScript<CCoolDownUIScript>()->SetChampType(GetCharacterType());
		GetUIBackPanel()->AddChild(Obj);
	}


	
	CGameObject* SkillLevel_UI = new CGameObject; //아래 스킬 레벨 표시 바 (점점점으로 되어있는거)
	SkillLevel_UI->SetName(L"SkillLevel_UI");
	SkillLevel_UI->AddComponent(new CTransform);
	SkillLevel_UI->AddComponent(new CMeshRender);
	SkillLevel_UI->AddComponent(new CSkillLevelUIScript);
	SkillLevel_UI->Transform()->SetRelativeScale(Vec3(190.f, 6.f, 1.f));
	SkillLevel_UI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SkillLevel_UI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SkillLevelBar.mtrl"),0);
	SkillLevel_UI->Transform()->SetAbsolute(true);
	SkillLevel_UI->Transform()->SetRelativePos(Vec3(-52.f, -10.f, -10.f));
	GetUIBackPanel()->AddChild(SkillLevel_UI);


	//=============================================
	//스킬 레벨업 버튼
	CGameObject* Q_LvUpObj = new CGameObject;
	Q_LvUpObj->SetName(L"Q_LvUpObj");
	Q_LvUpObj->AddComponent(new CTransform);
	Q_LvUpObj->AddComponent(new CMeshRender);
	Q_LvUpObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, -10.f));
	Q_LvUpObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Q_LvUpObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SkillLvUp.mtrl"), 0);
	Q_LvUpObj->Transform()->SetAbsolute(true);
	Q_LvUpObj->Transform()->SetRelativePos(Vec3(-131.f, 62.f, -10.f));
	GetUIBackPanel()->AddChild(Q_LvUpObj);

	CGameObject* W_LvUpObj = new CGameObject;
	W_LvUpObj->SetName(L"W_LvUpObj");
	W_LvUpObj->AddComponent(new CTransform);
	W_LvUpObj->AddComponent(new CMeshRender);
	W_LvUpObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));
	W_LvUpObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	W_LvUpObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SkillLvUp.mtrl"), 0);
	W_LvUpObj->Transform()->SetAbsolute(true);
	W_LvUpObj->Transform()->SetRelativePos(Vec3(-78.f, 62.f, -10.f));
	GetUIBackPanel()->AddChild(W_LvUpObj);

	CGameObject* E_LvUpObj = new CGameObject;
	E_LvUpObj->SetName(L"E_LvUpObj");
	E_LvUpObj->AddComponent(new CTransform);
	E_LvUpObj->AddComponent(new CMeshRender);
	E_LvUpObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));
	E_LvUpObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	E_LvUpObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SkillLvUp.mtrl"), 0);
	E_LvUpObj->Transform()->SetAbsolute(true);
	E_LvUpObj->Transform()->SetRelativePos(Vec3(-27.f, 62.f, -10.f));
	GetUIBackPanel()->AddChild(E_LvUpObj);

	CGameObject* R_LvUpObj = new CGameObject;
	R_LvUpObj->SetName(L"R_LvUpObj");
	R_LvUpObj->AddComponent(new CTransform);
	R_LvUpObj->AddComponent(new CMeshRender);
	R_LvUpObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));
	R_LvUpObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	R_LvUpObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SkillLvUp.mtrl"), 0);
	R_LvUpObj->Transform()->SetAbsolute(true);
	R_LvUpObj->Transform()->SetRelativePos(Vec3(23.24f, 62.f, -10.f));
	GetUIBackPanel()->AddChild(R_LvUpObj);

	CGameObject* Skill_LevelUpObj = new CGameObject;
	Skill_LevelUpObj->SetName(L"Skill_LevelUpObj");
	Skill_LevelUpObj->AddComponent(new CTransform);
	Skill_LevelUpObj->AddComponent(new CSkillLevelUpUIScript);
	Skill_LevelUpObj->GetScript<CSkillLevelUpUIScript>()->SetBackPanel(GetUIBackPanel());
	Skill_LevelUpObj->GetScript<CSkillLevelUpUIScript>()->Set_QWER_SkillLvUpObj(Q_LvUpObj, W_LvUpObj, E_LvUpObj, R_LvUpObj);
	Skill_LevelUpObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
	Skill_LevelUpObj->Transform()->SetAbsolute(true);
	Skill_LevelUpObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	GetUIBackPanel()->AddChild(Skill_LevelUpObj);
	
}

void CCharacterUIScript::SpellUILoad()
{
	//--------------선택한 스펠 정보대로 UI 생성----------------
	wstring DSpell = SUMMONERS_SPELL_WSTR[(UINT)GetSpellDType()];
	wstring FSpell = SUMMONERS_SPELL_WSTR[(UINT)GetSpellFType()];
	wstring UIpath = L"material\\";
	wstring mtrl = L".mtrl";

	for (UINT i = 0; i < 2; ++i)  //선택한 스펠 D,F 생성
	{
		CGameObject* Spell = new CGameObject; //캐릭터 패널 배치
		Spell->AddComponent(new CTransform);
		Spell->AddComponent(new CMeshRender);
		Spell->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Spell->Transform()->SetAbsolute(true);
		if (i == 0)
		{
			Spell_D = Spell;
			Spell_D->SetName(L"Spell_D");
			Spell_D->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::SPELL_D));
			Spell_D->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(UIpath + DSpell + mtrl), 0);
		}
		else
		{
			Spell_F = Spell;
			Spell_F->SetName(L"Spell_F");
			Spell_F->AddComponent(new CCoolDownUIScript(COOL_DOWN_TYPE::SPELL_F));
			Spell_F->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(UIpath + FSpell + mtrl), 0);
		}
		GetUIBackPanel()->AddChild(Spell);
	}
}

void CCharacterUIScript::BarUILoad()
{
	for (UINT i = 0; i < 4; ++i)  //선택한 스펠 D,F 생성
	{
		CGameObject* BarUI = new CGameObject; //캐릭터 패널 배치
		BarUI->AddComponent(new CTransform);
		BarUI->AddComponent(new CMeshRender);
		BarUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		BarUI->Transform()->SetAbsolute(true);

		switch (i) //스킬별 위치 지정
		{
		case 0:
		{
			HPBar = BarUI;
			HPBar->SetName(L"HPBar");
			HPBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\HPBar.mtrl"), 0);
			HPBar->AddComponent(new CHpMpRatioUIScript(BARTYPE::HP));
		}
		break;
		case 1:
		{
			MPBar = BarUI;
			MPBar->SetName(L"MPBar");
			MPBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MPBar.mtrl"), 0);
			MPBar->AddComponent(new CHpMpRatioUIScript(BARTYPE::MP));
		}
		break;
		case 2:
		{
			EXPBar = BarUI;
			EXPBar->SetName(L"EXPBar");
			EXPBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EXPBar.mtrl"), 0);
			EXPBar->AddComponent(new CExpRatioUIScript);
		}
		break;
		case 3:
		{
			WadingTotem = BarUI;
			WadingTotem->SetName(L"WadingTotem");
			WadingTotem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WadingTotem.mtrl"), 0);
		}
		break;
		}
		GetUIBackPanel()->AddChild(BarUI);
	}
}

void CCharacterUIScript::LevelUpFont()
{
	

	/*if (m_iPrevChampLevel != CurChampLevel)
	{
		tFont Font = {};
		Font.wInputText = Level;
		Font.fontType = FONT_TYPE::RIX_KOR_L;
		Font.fFontSize = 13.5;
		Font.vDisplayPos = Vec2(573.4f, 973.f);
		Font.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font);
	}*/

}



void CCharacterUIScript::UISetting()
{
	//---스킬 고정 위치---
	Skill_Q_Image->Transform()->SetRelativePos(Vec3(-130.68f, 18.2f, 2.f));
	Skill_W_Image->Transform()->SetRelativePos(Vec3(-78.5f, 18.2f, 2.f));
	Skill_E_Image->Transform()->SetRelativePos(Vec3(-26.8f, 18.2f, 2.f));
	Skill_R_Image->Transform()->SetRelativePos(Vec3(25.48f, 18.2f, 2.f));
	Skill_PASSIVE_Image->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 7.f));
	Skill_PASSIVE_Image->Transform()->SetRelativePos(Vec3(-174.4f, 25.2f, 2.f));
	Spell_D->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 7.f));
	Spell_D->Transform()->SetRelativePos(Vec3(73.f, 25.2f, 2.f));
	Spell_F->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 7.f));
	Spell_F->Transform()->SetRelativePos(Vec3(110.52f, 25.2f, 2.f));
	HPBar->Transform()->SetRelativeScale(Vec3(312.4f, 13.f, 1.f));
	HPBar->Transform()->SetRelativePos(Vec3(-32.f, -25.76f, -1.f));
	MPBar->Transform()->SetRelativeScale(Vec3(312.4f, 13.f, 1.f));
	MPBar->Transform()->SetRelativePos(Vec3(-32.f, -41.12f, -1.f));
	EXPBar->Transform()->SetRelativeScale(Vec3(36.f, 87.f, 1.f));
	EXPBar->Transform()->SetRelativePos(Vec3(-213.f, 7.f, -1.f));
	WadingTotem->Transform()->SetRelativeScale(Vec3(25.f, 25.f, 1.f));
	WadingTotem->Transform()->SetRelativePos(Vec3(272.32f, 24.68f, 7.f));
}