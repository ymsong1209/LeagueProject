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
	Inventory->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 0.1f)); //���� �츮�� ����ϴ� ������ �̸��� ������ ���͸��� �̸��� ���ƾ���
	SpawnGameObject(Inventory, Vec3(45.f, -108.f, 400.f), 31);
	//==============������  �ӽ� ������ ���� (���� è�Ǿ� ��ũ��Ʈ���� �޾ƿ;���)==================
	for (size_t i = 0; i < 6; ++i)
	{
		m_Inventory[i] = new CItem();

		switch (i)
		{
		case 0:
			m_Inventory[i]->SetName(L"item_Boots");
			break;
		case 1:
			m_Inventory[i]->SetName(L"item_noonquiver");
			break;
		case 2:
			m_Inventory[i]->SetName(L"item_theCollector");
			break;
		case 3:
			m_Inventory[i]->SetName(L"item_imperialmandate"); //������ �̸��� ���� (è�Ǿ� ��ũ��Ʈ���� �޾ƿ� ����)
			break;
		case 4:
			m_Inventory[i]->SetName(L"Item_zeal");
			break;
		case 5:
			m_Inventory[i]->SetName(L"Item_shardoftrueice");
			break;
		}
	}
	//===============================================================





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

		Item->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 0.1f));
		Item->Transform()->SetAbsolute(true);
		Item->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
		m_vecItemObj.push_back(Item);
		Inventory->AddChild(Item);

		PosSetting(i);
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

void CInventoryUIScript::PosSetting(int _i)
{
	CTransform* Transform = m_vecItemObj[_i]->Transform();
	switch (_i)
	{
	case 0:
		Transform->SetRelativePos(Vec3(120.68f, -313.8f, 100.f));
		break;
	case 1:
		Transform->SetRelativePos(Vec3(158.52f, -313.8f, 100.f));
		break;
	case 2:
		Transform->SetRelativePos(Vec3(192.32f, -313.8f, 100.f));
		break;
	case 3:
		Transform->SetRelativePos(Vec3(120.68f, -347.56f, 100.f));
		break;
	case 4:
		Transform->SetRelativePos(Vec3(158.52f, -347.56f, 100.f));
		break;
	case 5:
		Transform->SetRelativePos(Vec3(192.32f, -347.56f, 100.f));
		break;
	}
}
