#include "pch.h"
#include "CShopKeeperSouthScript.h"
#include <Engine/CAnim3D.h>

CShopKeeperSouthScript::CShopKeeperSouthScript()
	:CScript((UINT)SCRIPT_TYPE::SHOPKEEPERSOUTHSCRIPT)
{
}

CShopKeeperSouthScript::~CShopKeeperSouthScript()
{
}


void CShopKeeperSouthScript::begin()
{
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\shopkeeper_south");
	GetOwner()->Animator3D()->PlayRepeat(L"shopkeeper_south\\Idle1_Base_model.002",false);
}

void CShopKeeperSouthScript::tick()
{
}

