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
	//============================������ (�ʿ�x)============================
	//if (KEY_TAP(KEY::F5))
	//{
	//	if(GetCharacterType() == CHARACTER_TYPE::MALPHIGHT)
	//		SetCharacterType(CHARACTER_TYPE::JINX);
	//	else if(GetCharacterType() == CHARACTER_TYPE::JINX)
	//		SetCharacterType(CHARACTER_TYPE::MALPHIGHT);
	//}

	////----Ȥ�ö� ������� ���� è�Ǿ��� �����Ѵٸ� ��ų ���͸���鵵 ����-----
	//CHARACTER_TYPE Champ = GetCharacterType();
	//if (PrevCharacter != Champ)
	//{
	//	wstring mtrl = L".mtrl";
	//	wstring under = L"_";
	//	wstring CharacterType = CHARACTER_TYPE_WSTR[(UINT)GetCharacterType()];
	//	wstring ChampMtrlPath = L"material\\" + CharacterType + under + L"CIRCLE" + mtrl;

	//	CharacterImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(ChampMtrlPath), 0);

	//	for (UINT i = 0; i < (UINT)SkillNum::END; ++i)  //��ų �̹��� Q,W,E,R,Passive ��ġ 
	//	{
	//		SkillNumber = (SkillNum)i;
	//		wstring UIpath = L"material\\";
	//		wstring SkillName = CharacterType + under + SKILL_TYPE_WSTR[i];
	//		wstring fullpath = UIpath + SkillName + mtrl;

	//		switch (i) //��ų�� ��ġ ����
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
	/* �� �� !!*/
	//���͸��� �̸��� CHARACTER_TYPE + "_" + SkillNum ���·� �Ǿ��־���� (ex: MALPHGITE_Q)
	//�׷��� �˾Ƽ� ĳ���ͺ��� ���͸����� ã�ƿ�
	CUIScript::begin();

	PrevCharacter = GetCharacterType();
	wstring UIpath = L"material\\";
	wstring mtrl = L".mtrl";
	wstring under = L"_";
	wstring CharacterType = CHARACTER_TYPE_WSTR[(UINT)GetCharacterType()];
	wstring ChampMtrlPath = UIpath + CharacterType + under + L"CIRCLE" + mtrl;

	CharacterImage = new CGameObject; //ĳ���� �г� ��ġ
	CharacterImage->SetName(L"UICharacterPanel");
	CharacterImage->AddComponent(new CTransform);
	CharacterImage->AddComponent(new CMeshRender);
	CharacterImage->Transform()->SetRelativeScale(Vec3(82.f, 82.f, 20.f));
	CharacterImage->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CharacterImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(ChampMtrlPath), 0);
	CharacterImage->Transform()->SetAbsolute(true);
	CharacterImage->Transform()->SetRelativePos(Vec3(-250.f, 0.f, 2.f));
	GetUIFrontPanel()->AddChild(CharacterImage);


	for (UINT i = 0; i < (UINT)SkillNum::END; ++i)  //��ų �̹��� Q,W,E,R,Passive ��ġ 
	{
		SkillNumber = (SkillNum)i;
		wstring SkillName = CharacterType + under + SKILL_TYPE_WSTR[i];
		wstring fullpath = UIpath + SkillName + mtrl;
		CGameObject* Obj = new CGameObject;
		Obj->SetName(SkillName);
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCoolDownUIScript);
		Obj->Transform()->SetRelativeScale(Vec3(45.f, 45.f, 10.f));
		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullpath), 0);
		Obj->Transform()->SetAbsolute(true);
		GetUIBackPanel()->AddChild(Obj);

		switch (i) //��ų ������Ʈ ����. 
		{
		case 0:
			Skill_Q_Image = Obj;
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
	//--------------������ ���� ������� UI ����----------------
	wstring DSpell = SUMMONERS_SPELL_WSTR[(UINT)GetSpellDType()];
	wstring FSpell = SUMMONERS_SPELL_WSTR[(UINT)GetSpellFType()];
	wstring UIpath = L"material\\";
	wstring mtrl = L".mtrl";

	for (UINT i = 0; i < 2; ++i)  //������ ���� D,F ����
	{
		CGameObject* Spell = new CGameObject; //ĳ���� �г� ��ġ
		Spell->AddComponent(new CTransform);
		Spell->AddComponent(new CMeshRender);
		Spell->AddComponent(new CCoolDownUIScript);
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
	for (UINT i = 0; i < 4; ++i)  //������ ���� D,F ����
	{
		CGameObject* BarUI = new CGameObject; //ĳ���� �г� ��ġ
		BarUI->AddComponent(new CTransform);
		BarUI->AddComponent(new CMeshRender);
		BarUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		BarUI->Transform()->SetAbsolute(true);

		switch (i) //��ų�� ��ġ ����
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
	//---��ų ���� ��ġ---
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