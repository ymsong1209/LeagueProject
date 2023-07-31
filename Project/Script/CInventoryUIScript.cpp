#include "pch.h"
#include "CInventoryUIScript.h"
#include "CCoolDownUIScript.h"

CInventoryUIScript::CInventoryUIScript()
	:CScript((UINT)SCRIPT_TYPE::INVENTORYUISCRIPT)
{
}

CInventoryUIScript::~CInventoryUIScript()
{
	for (size_t i = 0; i < 6; ++i)
	{
		delete m_Inventory[i];
	}

}

void CInventoryUIScript::begin()
{
	CGameObject* Inventory = new CGameObject;
	Inventory->SetName(L"Inventory");
	Inventory->AddComponent(new CTransform);
	Inventory->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 0.1f)); //현재 우리가 사용하는 아이템 이름과 아이템 머터리얼 이름이 같아야함
	SpawnGameObject(Inventory, Vec3(45.f, -108.f, 100.f), 31);
	//--------------디버깅용  임시 아이템 지정 (원래 챔피언 스크립트에서 받아와야함)------------------
	for (size_t i = 0; i < 4; ++i)
	{
		m_Inventory[i] = new CItem();

		switch (i)
		{
		case 0:
			m_Inventory[i]->SetName(L"item_imperialmandate"); //아이템 이름을 지정 (챔피언 스크립트에서 받아올 예정)
			break;
		case 1:
			m_Inventory[i]->SetName(L"item_noonquiver");
			break;
		case 2:
			m_Inventory[i]->SetName(L"item_theCollector");
			break;
		case 3:
			m_Inventory[i]->SetName(L"BootsMtrl");
			break;
		}
	}
	//----------------------------초반 아이템 칸 생성------------------------------------

	wstring material = L"material\\";
	wstring mtrl = L".mtrl";

	for (size_t i = 0; i < 6; ++i)
	{
		wstring ItemNum = L"item" + std::to_wstring(i + 1);
		CGameObject* Item = new CGameObject;
		Item->SetName(ItemNum);
		Item->AddComponent(new CTransform);
		Item->AddComponent(new CMeshRender);
		Item->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		if (m_Inventory[i] != nullptr)
		{
			wstring ItemName = m_Inventory[i]->GetName();
			wstring fullname = material + ItemName + mtrl;
			Item->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullname), 0);
		}

		Item->Transform()->SetRelativeScale(Vec3(6.4f, 6.4f, 0.1f));
		Item->Transform()->SetAbsolute(true);
		Item->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
		m_vecItemObj.push_back(Item);
		Inventory->AddChild(Item);
	}
}

void CInventoryUIScript::tick()
{
	wstring material = L"material\\";
	wstring mtrl = L".mtrl";
	for (size_t i = 0; i < 6; ++i)
	{
		if (m_vecItemObj.size() != 0)
		{
			if (m_Inventory[i])
			{
				wstring ItemName = m_Inventory[i]->GetName();
				wstring fullname = material + ItemName + mtrl;
				m_vecItemObj[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(fullname), 0);
			}

			else
				m_vecItemObj[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ItemAlpha.mtrl"), 0);
		}
	}


	if (KEY_TAP(KEY::F4))
	{
		m_Inventory[0] = nullptr;
	}
}

void CInventoryUIScript::BeginOverlap(CCollider2D* _Other)
{
}

