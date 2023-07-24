#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CChampionScript.h"
#include "CInGameCameraScript.h"
#include "CJinxScript.h"
#include "CPlayerScript.h"
#include "CUnitScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChampionScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CJinxScript");
	_vec.push_back(L"CPlayerScript");
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
	if (L"CJinxScript" == _strScriptName)
		return new CJinxScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
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
	case (UINT)SCRIPT_TYPE::JINXSCRIPT:
		return new CJinxScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
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

	case SCRIPT_TYPE::JINXSCRIPT:
		return L"CJinxScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::UNITSCRIPT:
		return L"CUnitScript";
		break;

	}
	return nullptr;
}