#include "pch.h"
#include "CScriptMgr.h"

#include "CAttackRangeScript.h"
#include "CBasicAttackScript.h"
#include "CBlueScript.h"
#include "CCameraMoveScript.h"
#include "CChampionScript.h"
#include "CGrompScript.h"
#include "CInGameCameraScript.h"
#include "CInhibitorScript.h"
#include "CJinxScript.h"
#include "CJinxWScript.h"
#include "CJungleMonsterScript.h"
#include "CKrugMiniScript.h"
#include "CKrugScript.h"
#include "CMinionScript.h"
#include "CMobScript.h"
#include "CMurkWolfMiniScript.h"
#include "CMurkWolfScript.h"
#include "CNexusScript.h"
#include "COtherPlayerScript.h"
#include "CPlayerScript.h"
#include "CProjectileScript.h"
#include "CRazorBeakMiniScript.h"
#include "CRazorBeakScript.h"
#include "CRedScript.h"
#include "CSoundTestScript.h"
#include "CStructureScript.h"
#include "CTurretAttackScript.h"
#include "CTurretScript.h"
#include "CUnitScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackRangeScript");
	_vec.push_back(L"CBasicAttackScript");
	_vec.push_back(L"CBlueScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChampionScript");
	_vec.push_back(L"CGrompScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CInhibitorScript");
	_vec.push_back(L"CJinxScript");
	_vec.push_back(L"CJinxWScript");
	_vec.push_back(L"CJungleMonsterScript");
	_vec.push_back(L"CKrugMiniScript");
	_vec.push_back(L"CKrugScript");
	_vec.push_back(L"CMinionScript");
	_vec.push_back(L"CMobScript");
	_vec.push_back(L"CMurkWolfMiniScript");
	_vec.push_back(L"CMurkWolfScript");
	_vec.push_back(L"CNexusScript");
	_vec.push_back(L"COtherPlayerScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectileScript");
	_vec.push_back(L"CRazorBeakMiniScript");
	_vec.push_back(L"CRazorBeakScript");
	_vec.push_back(L"CRedScript");
	_vec.push_back(L"CSoundTestScript");
	_vec.push_back(L"CStructureScript");
	_vec.push_back(L"CTurretAttackScript");
	_vec.push_back(L"CTurretScript");
	_vec.push_back(L"CUnitScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttackRangeScript" == _strScriptName)
		return new CAttackRangeScript;
	if (L"CBasicAttackScript" == _strScriptName)
		return new CBasicAttackScript;
	if (L"CBlueScript" == _strScriptName)
		return new CBlueScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CChampionScript" == _strScriptName)
		return new CChampionScript;
	if (L"CGrompScript" == _strScriptName)
		return new CGrompScript;
	if (L"CInGameCameraScript" == _strScriptName)
		return new CInGameCameraScript;
	if (L"CInhibitorScript" == _strScriptName)
		return new CInhibitorScript;
	if (L"CJinxScript" == _strScriptName)
		return new CJinxScript;
	if (L"CJinxWScript" == _strScriptName)
		return new CJinxWScript;
	if (L"CJungleMonsterScript" == _strScriptName)
		return new CJungleMonsterScript;
	if (L"CKrugMiniScript" == _strScriptName)
		return new CKrugMiniScript;
	if (L"CKrugScript" == _strScriptName)
		return new CKrugScript;
	if (L"CMinionScript" == _strScriptName)
		return new CMinionScript;
	if (L"CMobScript" == _strScriptName)
		return new CMobScript;
	if (L"CMurkWolfMiniScript" == _strScriptName)
		return new CMurkWolfMiniScript;
	if (L"CMurkWolfScript" == _strScriptName)
		return new CMurkWolfScript;
	if (L"CNexusScript" == _strScriptName)
		return new CNexusScript;
	if (L"COtherPlayerScript" == _strScriptName)
		return new COtherPlayerScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CProjectileScript" == _strScriptName)
		return new CProjectileScript;
	if (L"CRazorBeakMiniScript" == _strScriptName)
		return new CRazorBeakMiniScript;
	if (L"CRazorBeakScript" == _strScriptName)
		return new CRazorBeakScript;
	if (L"CRedScript" == _strScriptName)
		return new CRedScript;
	if (L"CSoundTestScript" == _strScriptName)
		return new CSoundTestScript;
	if (L"CStructureScript" == _strScriptName)
		return new CStructureScript;
	if (L"CTurretAttackScript" == _strScriptName)
		return new CTurretAttackScript;
	if (L"CTurretScript" == _strScriptName)
		return new CTurretScript;
	if (L"CUnitScript" == _strScriptName)
		return new CUnitScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTACKRANGESCRIPT:
		return new CAttackRangeScript;
		break;
	case (UINT)SCRIPT_TYPE::BASICATTACKSCRIPT:
		return new CBasicAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::BLUESCRIPT:
		return new CBlueScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHAMPIONSCRIPT:
		return new CChampionScript;
		break;
	case (UINT)SCRIPT_TYPE::GROMPSCRIPT:
		return new CGrompScript;
		break;
	case (UINT)SCRIPT_TYPE::INGAMECAMERASCRIPT:
		return new CInGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::INHIBITORSCRIPT:
		return new CInhibitorScript;
		break;
	case (UINT)SCRIPT_TYPE::JINXSCRIPT:
		return new CJinxScript;
		break;
	case (UINT)SCRIPT_TYPE::JINXWSCRIPT:
		return new CJinxWScript;
		break;
	case (UINT)SCRIPT_TYPE::JUNGLEMONSTERSCRIPT:
		return new CJungleMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::KRUGMINISCRIPT:
		return new CKrugMiniScript;
		break;
	case (UINT)SCRIPT_TYPE::KRUGSCRIPT:
		return new CKrugScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIONSCRIPT:
		return new CMinionScript;
		break;
	case (UINT)SCRIPT_TYPE::MOBSCRIPT:
		return new CMobScript;
		break;
	case (UINT)SCRIPT_TYPE::MURKWOLFMINISCRIPT:
		return new CMurkWolfMiniScript;
		break;
	case (UINT)SCRIPT_TYPE::MURKWOLFSCRIPT:
		return new CMurkWolfScript;
		break;
	case (UINT)SCRIPT_TYPE::NEXUSSCRIPT:
		return new CNexusScript;
		break;
	case (UINT)SCRIPT_TYPE::OTHERPLAYERSCRIPT:
		return new COtherPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PROJECTILESCRIPT:
		return new CProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::RAZORBEAKMINISCRIPT:
		return new CRazorBeakMiniScript;
		break;
	case (UINT)SCRIPT_TYPE::RAZORBEAKSCRIPT:
		return new CRazorBeakScript;
		break;
	case (UINT)SCRIPT_TYPE::REDSCRIPT:
		return new CRedScript;
		break;
	case (UINT)SCRIPT_TYPE::SOUNDTESTSCRIPT:
		return new CSoundTestScript;
		break;
	case (UINT)SCRIPT_TYPE::STRUCTURESCRIPT:
		return new CStructureScript;
		break;
	case (UINT)SCRIPT_TYPE::TURRETATTACKSCRIPT:
		return new CTurretAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::TURRETSCRIPT:
		return new CTurretScript;
		break;
	case (UINT)SCRIPT_TYPE::UNITSCRIPT:
		return new CUnitScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTACKRANGESCRIPT:
		return L"CAttackRangeScript";
		break;

	case SCRIPT_TYPE::BASICATTACKSCRIPT:
		return L"CBasicAttackScript";
		break;

	case SCRIPT_TYPE::BLUESCRIPT:
		return L"CBlueScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHAMPIONSCRIPT:
		return L"CChampionScript";
		break;

	case SCRIPT_TYPE::GROMPSCRIPT:
		return L"CGrompScript";
		break;

	case SCRIPT_TYPE::INGAMECAMERASCRIPT:
		return L"CInGameCameraScript";
		break;

	case SCRIPT_TYPE::INHIBITORSCRIPT:
		return L"CInhibitorScript";
		break;

	case SCRIPT_TYPE::JINXSCRIPT:
		return L"CJinxScript";
		break;

	case SCRIPT_TYPE::JINXWSCRIPT:
		return L"CJinxWScript";
		break;

	case SCRIPT_TYPE::JUNGLEMONSTERSCRIPT:
		return L"CJungleMonsterScript";
		break;

	case SCRIPT_TYPE::KRUGMINISCRIPT:
		return L"CKrugMiniScript";
		break;

	case SCRIPT_TYPE::KRUGSCRIPT:
		return L"CKrugScript";
		break;

	case SCRIPT_TYPE::MINIONSCRIPT:
		return L"CMinionScript";
		break;

	case SCRIPT_TYPE::MOBSCRIPT:
		return L"CMobScript";
		break;

	case SCRIPT_TYPE::MURKWOLFMINISCRIPT:
		return L"CMurkWolfMiniScript";
		break;

	case SCRIPT_TYPE::MURKWOLFSCRIPT:
		return L"CMurkWolfScript";
		break;

	case SCRIPT_TYPE::NEXUSSCRIPT:
		return L"CNexusScript";
		break;

	case SCRIPT_TYPE::OTHERPLAYERSCRIPT:
		return L"COtherPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PROJECTILESCRIPT:
		return L"CProjectileScript";
		break;

	case SCRIPT_TYPE::RAZORBEAKMINISCRIPT:
		return L"CRazorBeakMiniScript";
		break;

	case SCRIPT_TYPE::RAZORBEAKSCRIPT:
		return L"CRazorBeakScript";
		break;

	case SCRIPT_TYPE::REDSCRIPT:
		return L"CRedScript";
		break;

	case SCRIPT_TYPE::SOUNDTESTSCRIPT:
		return L"CSoundTestScript";
		break;

	case SCRIPT_TYPE::STRUCTURESCRIPT:
		return L"CStructureScript";
		break;

	case SCRIPT_TYPE::TURRETATTACKSCRIPT:
		return L"CTurretAttackScript";
		break;

	case SCRIPT_TYPE::TURRETSCRIPT:
		return L"CTurretScript";
		break;

	case SCRIPT_TYPE::UNITSCRIPT:
		return L"CUnitScript";
		break;

	}
	return nullptr;
}