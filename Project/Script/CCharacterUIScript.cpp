#include "pch.h"
#include "CCharacterUIScript.h"
#include "CUIScript.h"
#include <Engine\CEventMgr.h>
#include "CCoolDownUIScript.h"
#include "CExpRatioUIScript.h"
#include "CHpMpRatioUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

CCharacterUIScript::CCharacterUIScript()
	:CUIScript(CHARACTERUISCRIPT)
	, m_iGold(14300)
	, m_iPlayerLevel(18)
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
		//======골드 폰트======
		wstring Gold = to_wstring((int)m_iGold);

		tFont Font3 = {};
		Font3.wInputText = Gold; //골드 폰트
		Font3.fontType = FONT_TYPE::RIX_KOR_L;
		Font3.fFontSize = 16.5;
		Font3.vDisplayPos = Vec2(1030, 974);
		Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font3);
		//=======================

		//======레벨 폰트======
		wstring Level = to_wstring((int)m_iPlayerLevel);

		tFont Font = {};
		Font.wInputText = Level; //골드 폰트
		Font.fontType = FONT_TYPE::RIX_KOR_L;
		Font.fFontSize = 13.5;
		Font.vDisplayPos = Vec2(573.4f, 973.f);
		Font.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::TRANS, Font);
		//=======================
	}
}

void CCharacterUIScript::BeginOverlap(CCollider2D* _Other)
{
}

void CCharacterUIScript::SkillUILoad()
{
	/* 주 의 !!*/
	//머터리얼 이름은 CHARACTER_TYPE + "_" + SkillNum 형태로 되어있어야함 (ex: MALPHGITE_Q)
	//그래야 알아서 캐릭터별로 머터리얼을 찾아옴
	CUIScript::begin();

	PrevCharacter = GetCharacterType();
	wstring UIpath = L"material\\";
	wstring mtrl = L".mtrl";
	wstring under = L"_";
	wstring CharacterType = CHARACTER_TYPE_WSTR[(UINT)GetCharacterType()];
	wstring ChampMtrlPath = UIpath + CharacterType + under + L"CIRCLE" + mtrl;

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

		GetUIBackPanel()->AddChild(Obj);
	}
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