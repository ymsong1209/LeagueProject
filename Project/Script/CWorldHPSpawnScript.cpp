#include "pch.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"
#include <Engine\CLevelMgr.h>

void CWorldHPSpawnScript::begin()
{
	//�ǹ�: �̽��������� �̹� �ƴ��÷��̾� ������ ��� �÷��̾���� ������ �ٵ����־����
	//����� ����÷��̾ ���� �� �Ŀ� ȣ��Ǿ�� (���Ϳ� ����÷��̾��� ������ ����־��) 

	//����� ü�¹ٸ� �����ټ����� ���� ���Ǹ��غ����� ����.
	/*CGameObject* Jinx = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Jinx");
	tWorldHPInfo info = {};
	info.Pos = Vec3(100.f,100.f,100.f);
	info.tHP = 30.f;
	info.tMP = 75.f;
	info.tTotalHP = 100.f;
	info.tTotalMP = 100.f;
	
	m_vOtherplayerInfo.push_back(info);

	for (size_t i = 0; i < m_vOtherplayerInfo.size(); ++i)
	{
		CGameObject* WorldBar = new CGameObject;
		WorldBar->AddComponent(new CTransform);
		WorldBar->AddComponent(new CMeshRender);
		WorldBar->AddComponent(new CWorldHPUIScript);
		WorldBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldBar.mtrl"), 0);
		WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldHpBar.mtrl"), 1);
		WorldBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\WorldMpBar.mtrl"), 2);
		WorldBar->MeshRender()->SetRaySightCulling(false);

		Vec3 Pos = m_vOtherplayerInfo[i].Pos;

		SpawnGameObject(WorldBar, Pos, 0);
		m_vWorldBar.push_back(WorldBar);
	}*/
}

void CWorldHPSpawnScript::tick()
{
	//for (size_t i = 0; i < m_vOtherplayerInfo.size(); ++i)
	//{
	//	Vec3 Pos = m_vOtherplayerInfo[i].Pos;
	//	float HP = m_vOtherplayerInfo[i].tHP;
	//	float MP = m_vOtherplayerInfo[i].tMP;
	//	float TotalHP = m_vOtherplayerInfo[i].tTotalHP;
	//	float TotalMP = m_vOtherplayerInfo[i].tTotalMP;

	//	m_vWorldBar[i]->Transform()->SetRelativePos(Pos); //�÷��̾��� ��ġ
	//	CWorldHPUIScript* Script = m_vWorldBar[i]->GetScript<CWorldHPUIScript>();
	//	Script->SetBarInfo(TotalHP, TotalMP, HP, MP);
	//}
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
