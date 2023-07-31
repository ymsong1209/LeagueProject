#include "pch.h"
#include "CCharacterUIScript.h"
#include "CUIScript.h"
#include <Engine\CEventMgr.h>
#include "CCoolDownUIScript.h"
#include "CExpRatioUIScript.h"
#include "CHpMpRatioUIScript.h"

CCharacterUIScript::CCharacterUIScript()
	:CUIScript(CHARACTERUISCRIPT)
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
	//============================디버깅용 (필요x)============================
	//if (KEY_TAP(KEY::F5))
	//{
	//	if(GetCharacterType() == CHARACTER_TYPE::MALPHIGHT)
	//		SetCharacterType(CHARACTER_TYPE::JINX);
	//	else if(GetCharacterType() == CHARACTER_TYPE::JINX)
	//		SetCharacterType(CHARACTER_TYPE::MALPHIGHT);
	//}

	////----혹시라도 디버깅을 위해 챔피언을 변경한다면 스킬 머터리얼들도 변경-----
	//CHARACTER_TYPE Champ = GetCharacterType();
	//if (PrevCharacter != Champ)
	//{
	//	wstring mtrl = L".mtrl";
	//	wstring under = L"_";
	//	wstring CharacterType = CHARACTER_TYPE_WSTR[(UINT)GetCharacterType()];
	//	wstring ChampMtrlPath = L"material\\" + CharacterType + under + L"CIRCLE" + mtrl;

	//	CharacterImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(ChampMtrlPath), 0);

	//	for (UINT i = 0; i < (UINT)SkillNum::END; ++i)  //스킬 이미지 Q,W,E,R,Passive 배치 
	//	{
	//		SkillNumber = (SkillNum)i;
	//		wstring UIpath = L"material\\";
	//		wstring SkillName = CharacterType + under + SKILL_TYPE_WSTR[i];
	//		wstring fullpath = UIpath + SkillName + mtrl;

	//		switch (i) //스킬별 위치 지정
	//		{
	//		case 0:
	//		{
	//			Skill_Q_Image->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
	//			Skill_Q_Image->SetName(SkillName);
	//		}
	//			break;
	//		case 1:
	//		{
	//			Skill_W_Image->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
	//			Skill_W_Image->SetName(SkillName);
	//		}
	//			break;
	//		case 2:
	//		{
	//			Skill_E_Image->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
	//			Skill_E_Image->SetName(SkillName);
	//		}
	//			break;
	//		case 3:
	//		{
	//			Skill_R_Image->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
	//			Skill_R_Image->SetName(SkillName);
	//		}
	//			break;
	//		case 4:
	//		{
	//			Skill_PASSIVE_Image->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
	//			Skill_PASSIVE_Image->SetName(SkillName);
	//		}
	//			break;
	//		}
	//	}

	//	ScriptInspectorReload();
	//}


	//PrevCharacter = Champ;
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
	SetChampInFo(CHARACTER_TYPE::VEIN, SUMMONERS_SPELL::HEAL, SUMMONERS_SPELL::FLASH);

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
	CharacterImage->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 20.f));
	CharacterImage->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CharacterImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(ChampMtrlPath), 0);
	CharacterImage->Transform()->SetAbsolute(true);
	CharacterImage->Transform()->SetRelativePos(Vec3(-55.f, 0.f, 2.f));
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
		Obj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));
		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
		Obj->Transform()->SetAbsolute(true);
		
		GetUIBackPanel()->AddChild(Obj);

		switch (i) //스킬 오브젝트 저장. 
		{
		case 0:
		{
			Obj->AddComponent(new CCoolDownUIScript);
			Skill_Q_Image = Obj;
		}
			break;
		case 1:
			Skill_W_Image = Obj;
			break;
		case 2:
			Skill_E_Image = Obj;
			break;
		case 3:
			Skill_R_Image = Obj;
			break;
		case 4:
			Skill_PASSIVE_Image = Obj;
			break;
		}
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
		//Spell->AddComponent(new CCoolDownUIScript);
		Spell->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Spell->Transform()->SetAbsolute(true);


		if (i == 0)
		{
			Spell_D = Spell;
			Spell_D->SetName(L"Spell_D");
			Spell_D->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(UIpath + DSpell + mtrl), 0);
		}
		else
		{
			Spell_F = Spell;
			Spell_F->SetName(L"Spell_F");
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

void CCharacterUIScript::SaveToLevelFile(FILE* _File)
{
}

void CCharacterUIScript::LoadFromLevelFile(FILE* _FILE)
{
}

void CCharacterUIScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
}

void CCharacterUIScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
}


void CCharacterUIScript::UISetting()
{
	//---스킬 고정 위치---
	Skill_Q_Image->Transform()->SetRelativePos(Vec3(-28.68f, 4.2f, 2.f));
	Skill_W_Image->Transform()->SetRelativePos(Vec3(-17.5f, 4.2f, 2.f));
	Skill_E_Image->Transform()->SetRelativePos(Vec3(-5.8f, 4.2f, 2.f));
	Skill_R_Image->Transform()->SetRelativePos(Vec3(5.48f, 4.2f, 2.f));
	Skill_PASSIVE_Image->Transform()->SetRelativeScale(Vec3(7.f, 7.f, 7.f));
	Skill_PASSIVE_Image->Transform()->SetRelativePos(Vec3(-38.4f, 5.2f, 2.f));
	Spell_D->Transform()->SetRelativeScale(Vec3(7.f, 7.f, 7.f));
	Spell_D->Transform()->SetRelativePos(Vec3(16.f, 5.2f, 2.f));
	Spell_F->Transform()->SetRelativeScale(Vec3(7.f, 7.f, 7.f));
	Spell_F->Transform()->SetRelativePos(Vec3(24.52f, 5.2f, 2.f));
	HPBar->Transform()->SetRelativeScale(Vec3(68.4f, 3.f, 1.f));
	HPBar->Transform()->SetRelativePos(Vec3(-7.f, -5.76f, -1.f));
	MPBar->Transform()->SetRelativeScale(Vec3(68.4f, 3.f, 1.f));
	MPBar->Transform()->SetRelativePos(Vec3(-7.f, -9.12f, -1.f));
	EXPBar->Transform()->SetRelativeScale(Vec3(7.f, 18.7f, 1.f));
	EXPBar->Transform()->SetRelativePos(Vec3(-48.f, 1.f, -1.f));
	WadingTotem->Transform()->SetRelativeScale(Vec3(6.f, 6.f, 1.f));
	WadingTotem->Transform()->SetRelativePos(Vec3(60.32f, 5.68f, 7.f));
}