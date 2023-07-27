#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CCharacterUIScript.h"
#include "CCoolDownUIScript.h"
#include "CGravityScript.h"
#include "CInGameCameraScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "COtherPlayerScript.h"
#include "CPlayerScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCharacterUIScript");
	_vec.push_back(L"CCoolDownUIScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"COtherPlayerScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCharacterUIScript" == _strScriptName)
		return new CCharacterUIScript;
	if (L"CCoolDownUIScript" == _strScriptName)
		return new CCoolDownUIScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
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
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
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

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
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

	}
	return nullptr;
}