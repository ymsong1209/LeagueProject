#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CChampionScript.h"
#include "CInGameCameraScript.h"
#include "CInhibitorScript.h"
#include "CJinxScript.h"
#include "CNexusScript.h"
#include "CPlayerScript.h"
#include "CStructureScript.h"
#include "CTurretScript.h"
#include "CUnitScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChampionScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CInhibitorScript");
	_vec.push_back(L"CJinxScript");
	_vec.push_back(L"CNexusScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CStructureScript");
	_vec.push_back(L"CTurretScript");
	_vec.push_back(L"CUnitScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CChampionScript" == _strScriptName)
		return new CChampionScript;
	if (L"CInGameCameraScript" == _strScriptName)
		return new CInGameCameraScript;
	if (L"CInhibitorScript" == _strScriptName)
		return new CInhibitorScript;
	if (L"CJinxScript" == _strScriptName)
		return new CJinxScript;
	if (L"CNexusScript" == _strScriptName)
		return new CNexusScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CStructureScript" == _strScriptName)
		return new CStructureScript;
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
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHAMPIONSCRIPT:
		return new CChampionScript;
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
	case (UINT)SCRIPT_TYPE::NEXUSSCRIPT:
		return new CNexusScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::STRUCTURESCRIPT:
		return new CStructureScript;
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
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHAMPIONSCRIPT:
		return L"CChampionScript";
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

	case SCRIPT_TYPE::NEXUSSCRIPT:
		return L"CNexusScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::STRUCTURESCRIPT:
		return L"CStructureScript";
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