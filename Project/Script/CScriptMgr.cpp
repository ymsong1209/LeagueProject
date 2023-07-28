#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CCharacterUIScript.h"
#include "CCoolDownUIScript.h"
#include "CExpRatioUIScript.h"
#include "CGravityScript.h"
#include "CHpMpRatioUIScript.h"
#include "CInGameCameraScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "COtherPlayerScript.h"
#include "CPlayerScript.h"
#include "CUIScript.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCharacterUIScript");
	_vec.push_back(L"CCoolDownUIScript");
	_vec.push_back(L"CExpRatioUIScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CHpMpRatioUIScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"COtherPlayerScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CUIScript");
	_vec.push_back(L"CWorldHPSpawnScript");
	_vec.push_back(L"CWorldHPUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCharacterUIScript" == _strScriptName)
		return new CCharacterUIScript;
	if (L"CCoolDownUIScript" == _strScriptName)
		return new CCoolDownUIScript;
	if (L"CExpRatioUIScript" == _strScriptName)
		return new CExpRatioUIScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CHpMpRatioUIScript" == _strScriptName)
		return new CHpMpRatioUIScript;
	if (L"CInGameCameraScript" == _strScriptName)
		return new CInGameCameraScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"COtherPlayerScript" == _strScriptName)
		return new COtherPlayerScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	if (L"CWorldHPSpawnScript" == _strScriptName)
		return new CWorldHPSpawnScript;
	if (L"CWorldHPUIScript" == _strScriptName)
		return new CWorldHPUIScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERUISCRIPT:
		return new CCharacterUIScript;
		break;
	case (UINT)SCRIPT_TYPE::COOLDOWNUISCRIPT:
		return new CCoolDownUIScript;
		break;
	case (UINT)SCRIPT_TYPE::EXPRATIOUISCRIPT:
		return new CExpRatioUIScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT:
		return new CHpMpRatioUIScript;
		break;
	case (UINT)SCRIPT_TYPE::INGAMECAMERASCRIPT:
		return new CInGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::OTHERPLAYERSCRIPT:
		return new COtherPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUIScript;
		break;
	case (UINT)SCRIPT_TYPE::WORLDHPSPAWNSCRIPT:
		return new CWorldHPSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::WORLDHPUISCRIPT:
		return new CWorldHPUIScript;
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

	case SCRIPT_TYPE::CHARACTERUISCRIPT:
		return L"CCharacterUIScript";
		break;

	case SCRIPT_TYPE::COOLDOWNUISCRIPT:
		return L"CCoolDownUIScript";
		break;

	case SCRIPT_TYPE::EXPRATIOUISCRIPT:
		return L"CExpRatioUIScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::HPMPRATIOUISCRIPT:
		return L"CHpMpRatioUIScript";
		break;

	case SCRIPT_TYPE::INGAMECAMERASCRIPT:
		return L"CInGameCameraScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::OTHERPLAYERSCRIPT:
		return L"COtherPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	case SCRIPT_TYPE::WORLDHPSPAWNSCRIPT:
		return L"CWorldHPSpawnScript";
		break;

	case SCRIPT_TYPE::WORLDHPUISCRIPT:
		return L"CWorldHPUIScript";
		break;

	}
	return nullptr;
}