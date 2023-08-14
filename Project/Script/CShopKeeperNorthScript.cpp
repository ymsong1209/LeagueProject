#include "pch.h"
#include "CShopKeeperNorthScript.h"
#include <Engine/CAnim3D.h>

CShopKeeperNorthScript::CShopKeeperNorthScript()
	:CScript((UINT)SCRIPT_TYPE::SHOPKEEPERNORTHSCRIPT)
{
}

CShopKeeperNorthScript::~CShopKeeperNorthScript()
{
}


void CShopKeeperNorthScript::begin()
{
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\shopkeeper_north");
	GetOwner()->Animator3D()->PlayOnce(L"shopkeeper_north\\sru_storekeepernorth_idle1");
}

void CShopKeeperNorthScript::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAnimNum;
		if (m_iAnimNum > 6) {
			m_iAnimNum = 1;
		}
		wstring basename = L"shopkeeper_north\\sru_storekeepernorth_idle";
		basename += std::to_wstring(m_iAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename, false, 0.f);
	}
}

