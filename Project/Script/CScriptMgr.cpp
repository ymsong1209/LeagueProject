#include "pch.h"
#include "CScriptMgr.h"

#include "CBasicAttackScript.h"
#include "CCameraMoveScript.h"
#include "CChampionScript.h"
#include "CCharacterUIScript.h"
#include "CCoolDownUIScript.h"
#include "CExpRatioUIScript.h"
#include "CFontUIScript.h"
#include "CGrompScript.h"
#include "CHpMpRatioUIScript.h"
#include "CInGameCameraScript.h"
#include "CInhibitorScript.h"
#include "CInventoryUIScript.h"
#include "CJinxScript.h"
#include "CJungleMonsterScript.h"
#include "CMinimapUIScript.h"
#include "CMinionScript.h"
#include "CMobScript.h"
#include "CMouseCursorUIScript.h"
#include "CMoveCursorUIScript.h"
#include "CNexusScript.h"
#include "COtherPlayerScript.h"
#include "CPlayerScript.h"
#include "CScorePanelScript.h"
#include "CSoundTestScript.h"
#include "CStructureScript.h"
#include "CTurretAttackScript.h"
#include "CTurretScript.h"
#include "CUIScript.h"
#include "CUnitScript.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBasicAttackScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChampionScript");
	_vec.push_back(L"CCharacterUIScript");
	_vec.push_back(L"CCoolDownUIScript");
	_vec.push_back(L"CExpRatioUIScript");
	_vec.push_back(L"CFontUIScript");
	_vec.push_back(L"CGrompScript");
	_vec.push_back(L"CHpMpRatioUIScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CInhibitorScript");
	_vec.push_back(L"CInventoryUIScript");
	_vec.push_back(L"CJinxScript");
	_vec.push_back(L"CJungleMonsterScript");
	_vec.push_back(L"CMinimapUIScript");
	_vec.push_back(L"CMinionScript");
	_vec.push_back(L"CMobScript");
	_vec.push_back(L"CMouseCursorUIScript");
	_vec.push_back(L"CMoveCursorUIScript");
	_vec.push_back(L"CNexusScript");
	_vec.push_back(L"COtherPlayerScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CScorePanelScript");
	_vec.push_back(L"CSoundTestScript");
	_vec.push_back(L"CStructureScript");
	_vec.push_back(L"CTurretAttackScript");
	_vec.push_back(L"CTurretScript");
	_vec.push_back(L"CUIScript");
	_vec.push_back(L"CUnitScript");
	_vec.push_back(L"CWorldHPSpawnScript");
	_vec.push_back(L"CWorldHPUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBasicAttackScript" == _strScriptName)
		return new CBasicAttackScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CChampionScript" == _strScriptName)
		return new CChampionScript;
	if (L"CCharacterUIScript" == _strScriptName)
		return new CCharacterUIScript;
	if (L"CCoolDownUIScript" == _strScriptName)
		return new CCoolDownUIScript;
	if (L"CExpRatioUIScript" == _strScriptName)
		return new CExpRatioUIScript;
	if (L"CFontUIScript" == _strScriptName)
		return new CFontUIScript;
	if (L"CGrompScript" == _strScriptName)
		return new CGrompScript;
	if (L"CHpMpRatioUIScript" == _strScriptName)
		return new CHpMpRatioUIScript;
	if (L"CInGameCameraScript" == _strScriptName)
		return new CInGameCameraScript;
	if (L"CInhibitorScript" == _strScriptName)
		return new CInhibitorScript;
	if (L"CInventoryUIScript" == _strScriptName)
		return new CInventoryUIScript;
	if (L"CJinxScript" == _strScriptName)
		return new CJinxScript;
	if (L"CJungleMonsterScript" == _strScriptName)
		return new CJungleMonsterScript;
	if (L"CMinimapUIScript" == _strScriptName)
		return new CMinimapUIScript;
	if (L"CMinionScript" == _strScriptName)
		return new CMinionScript;
	if (L"CMobScript" == _strScriptName)
		return new CMobScript;
	if (L"CMouseCursorUIScript" == _strScriptName)
		return new CMouseCursorUIScript;
	if (L"CMoveCursorUIScript" == _strScriptName)
		return new CMoveCursorUIScript;
	if (L"CNexusScript" == _strScriptName)
		return new CNexusScript;
	if (L"COtherPlayerScript" == _strScriptName)
		return new COtherPlayerScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CScorePanelScript" == _strScriptName)
		return new CScorePanelScript;
	if (L"CSoundTestScript" == _strScriptName)
		return new CSoundTestScript;
	if (L"CStructureScript" == _strScriptName)
		return new CStructureScript;
	if (L"CTurretAttackScript" == _strScriptName)
		return new CTurretAttackScript;
	if (L"CTurretScript" == _strScriptName)
		return new CTurretScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	if (L"CUnitScript" == _strScriptName)
		return new CUnitScript;
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
	case (UINT)SCRIPT_TYPE::BASICATTACKSCRIPT:
		return new CBasicAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHAMPIONSCRIPT:
		return new CChampionScript;
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
	case (UINT)SCRIPT_TYPE::FONTUISCRIPT:
		return new CFontUIScript;
		break;
	case (UINT)SCRIPT_TYPE::GROMPSCRIPT:
		return new CGrompScript;
		break;
	case (UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT:
		return new CHpMpRatioUIScript;
		break;
	case (UINT)SCRIPT_TYPE::INGAMECAMERASCRIPT:
		return new CInGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::INHIBITORSCRIPT:
		return new CInhibitorScript;
		break;
	case (UINT)SCRIPT_TYPE::INVENTORYUISCRIPT:
		return new CInventoryUIScript;
		break;
	case (UINT)SCRIPT_TYPE::JINXSCRIPT:
		return new CJinxScript;
		break;
	case (UINT)SCRIPT_TYPE::JUNGLEMONSTERSCRIPT:
		return new CJungleMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIMAPUISCRIPT:
		return new CMinimapUIScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIONSCRIPT:
		return new CMinionScript;
		break;
	case (UINT)SCRIPT_TYPE::MOBSCRIPT:
		return new CMobScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSECURSORUISCRIPT:
		return new CMouseCursorUIScript;
		break;
	case (UINT)SCRIPT_TYPE::MOVECURSORUISCRIPT:
		return new CMoveCursorUIScript;
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
	case (UINT)SCRIPT_TYPE::SCOREPANELSCRIPT:
		return new CScorePanelScript;
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
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUIScript;
		break;
	case (UINT)SCRIPT_TYPE::UNITSCRIPT:
		return new CUnitScript;
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
	case SCRIPT_TYPE::BASICATTACKSCRIPT:
		return L"CBasicAttackScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHAMPIONSCRIPT:
		return L"CChampionScript";
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

	case SCRIPT_TYPE::FONTUISCRIPT:
		return L"CFontUIScript";
		break;

	case SCRIPT_TYPE::GROMPSCRIPT:
		return L"CGrompScript";
		break;

	case SCRIPT_TYPE::HPMPRATIOUISCRIPT:
		return L"CHpMpRatioUIScript";
		break;

	case SCRIPT_TYPE::INGAMECAMERASCRIPT:
		return L"CInGameCameraScript";
		break;

	case SCRIPT_TYPE::INHIBITORSCRIPT:
		return L"CInhibitorScript";
		break;

	case SCRIPT_TYPE::INVENTORYUISCRIPT:
		return L"CInventoryUIScript";
		break;

	case SCRIPT_TYPE::JINXSCRIPT:
		return L"CJinxScript";
		break;

	case SCRIPT_TYPE::JUNGLEMONSTERSCRIPT:
		return L"CJungleMonsterScript";
		break;

	case SCRIPT_TYPE::MINIMAPUISCRIPT:
		return L"CMinimapUIScript";
		break;

	case SCRIPT_TYPE::MINIONSCRIPT:
		return L"CMinionScript";
		break;

	case SCRIPT_TYPE::MOBSCRIPT:
		return L"CMobScript";
		break;

	case SCRIPT_TYPE::MOUSECURSORUISCRIPT:
		return L"CMouseCursorUIScript";
		break;

	case SCRIPT_TYPE::MOVECURSORUISCRIPT:
		return L"CMoveCursorUIScript";
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

	case SCRIPT_TYPE::SCOREPANELSCRIPT:
		return L"CScorePanelScript";
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

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	case SCRIPT_TYPE::UNITSCRIPT:
		return L"CUnitScript";
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