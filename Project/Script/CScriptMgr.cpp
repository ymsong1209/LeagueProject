#include "pch.h"
#include "CScriptMgr.h"

#include "CAttackRangeScript.h"
#include "CBasicAttackScript.h"
#include "CBlueScript.h"
#include "CBushScript.h"
#include "CCameraMoveScript.h"
#include "CChampionScript.h"
#include "CCharacterUIScript.h"
#include "CCoolDownUIScript.h"
#include "CDragonHPUIScript.h"
#include "CDragonScript.h"
#include "CExpRatioUIScript.h"
#include "CFontUIScript.h"
#include "CGrompScript.h"
#include "CHpMpRatioUIScript.h"
#include "CInGameCameraScript.h"
#include "CInhibitorScript.h"
#include "CInventoryUIScript.h"
#include "CJinxEScript.h"
#include "CJinxRScript.h"
#include "CJinxScript.h"
#include "CJinxWScript.h"
#include "CJungleMINIHPScript.h"
#include "CJungleMobHPScript.h"
#include "CJungleMonsterScript.h"
#include "CKrugMiniScript.h"
#include "CKrugScript.h"
#include "CMalphiteEDecalScript.h"
#include "CMalphiteEScript.h"
#include "CMalphiteQDecalScript.h"
#include "CMalphiteQModelScript.h"
#include "CMalphiteQScript.h"
#include "CMalphiteRDecalScript.h"
#include "CMalphiteRScript.h"
#include "CMalphiteScript.h"
#include "CMinimapUIScript.h"
#include "CMinionHPBarPosScript.h"
#include "CMinionHPRatioScript.h"
#include "CMinionScript.h"
#include "CMobScript.h"
#include "CMouseCursorUIScript.h"
#include "CMoveCursorUIScript.h"
#include "CMurkWolfMiniScript.h"
#include "CMurkWolfScript.h"
#include "CNexusScript.h"
#include "COtherPlayerScript.h"
#include "CPlayerScript.h"
#include "CProjectileScript.h"
#include "CRazorBeakMiniScript.h"
#include "CRazorBeakScript.h"
#include "CRedScript.h"
#include "CScorePanelScript.h"
#include "CSoundTestScript.h"
#include "CStructureScript.h"
#include "CTurretAttackScript.h"
#include "CTurretHPUIScript.h"
#include "CTurretScript.h"
#include "CUIScript.h"
#include "CUnitScript.h"
#include "CWorldHPSpawnScript.h"
#include "CWorldHPUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackRangeScript");
	_vec.push_back(L"CBasicAttackScript");
	_vec.push_back(L"CBlueScript");
	_vec.push_back(L"CBushScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChampionScript");
	_vec.push_back(L"CCharacterUIScript");
	_vec.push_back(L"CCoolDownUIScript");
	_vec.push_back(L"CDragonHPUIScript");
	_vec.push_back(L"CDragonScript");
	_vec.push_back(L"CExpRatioUIScript");
	_vec.push_back(L"CFontUIScript");
	_vec.push_back(L"CGrompScript");
	_vec.push_back(L"CHpMpRatioUIScript");
	_vec.push_back(L"CInGameCameraScript");
	_vec.push_back(L"CInhibitorScript");
	_vec.push_back(L"CInventoryUIScript");
	_vec.push_back(L"CJinxEScript");
	_vec.push_back(L"CJinxRScript");
	_vec.push_back(L"CJinxScript");
	_vec.push_back(L"CJinxWScript");
	_vec.push_back(L"CJungleMINIHPScript");
	_vec.push_back(L"CJungleMobHPScript");
	_vec.push_back(L"CJungleMonsterScript");
	_vec.push_back(L"CKrugMiniScript");
	_vec.push_back(L"CKrugScript");
	_vec.push_back(L"CMalphiteEDecalScript");
	_vec.push_back(L"CMalphiteEScript");
	_vec.push_back(L"CMalphiteQDecalScript");
	_vec.push_back(L"CMalphiteQModelScript");
	_vec.push_back(L"CMalphiteQScript");
	_vec.push_back(L"CMalphiteRDecalScript");
	_vec.push_back(L"CMalphiteRScript");
	_vec.push_back(L"CMalphiteScript");
	_vec.push_back(L"CMinimapUIScript");
	_vec.push_back(L"CMinionHPBarPosScript");
	_vec.push_back(L"CMinionHPRatioScript");
	_vec.push_back(L"CMinionScript");
	_vec.push_back(L"CMobScript");
	_vec.push_back(L"CMouseCursorUIScript");
	_vec.push_back(L"CMoveCursorUIScript");
	_vec.push_back(L"CMurkWolfMiniScript");
	_vec.push_back(L"CMurkWolfScript");
	_vec.push_back(L"CNexusScript");
	_vec.push_back(L"COtherPlayerScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectileScript");
	_vec.push_back(L"CRazorBeakMiniScript");
	_vec.push_back(L"CRazorBeakScript");
	_vec.push_back(L"CRedScript");
	_vec.push_back(L"CScorePanelScript");
	_vec.push_back(L"CSoundTestScript");
	_vec.push_back(L"CStructureScript");
	_vec.push_back(L"CTurretAttackScript");
	_vec.push_back(L"CTurretHPUIScript");
	_vec.push_back(L"CTurretScript");
	_vec.push_back(L"CUIScript");
	_vec.push_back(L"CUnitScript");
	_vec.push_back(L"CWorldHPSpawnScript");
	_vec.push_back(L"CWorldHPUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttackRangeScript" == _strScriptName)
		return new CAttackRangeScript;
	if (L"CBasicAttackScript" == _strScriptName)
		return new CBasicAttackScript;
	if (L"CBlueScript" == _strScriptName)
		return new CBlueScript;
	if (L"CBushScript" == _strScriptName)
		return new CBushScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CChampionScript" == _strScriptName)
		return new CChampionScript;
	if (L"CCharacterUIScript" == _strScriptName)
		return new CCharacterUIScript;
	if (L"CCoolDownUIScript" == _strScriptName)
		return new CCoolDownUIScript;
	if (L"CDragonHPUIScript" == _strScriptName)
		return new CDragonHPUIScript;
	if (L"CDragonScript" == _strScriptName)
		return new CDragonScript;
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
	if (L"CJinxEScript" == _strScriptName)
		return new CJinxEScript;
	if (L"CJinxRScript" == _strScriptName)
		return new CJinxRScript;
	if (L"CJinxScript" == _strScriptName)
		return new CJinxScript;
	if (L"CJinxWScript" == _strScriptName)
		return new CJinxWScript;
	if (L"CJungleMINIHPScript" == _strScriptName)
		return new CJungleMINIHPScript;
	if (L"CJungleMobHPScript" == _strScriptName)
		return new CJungleMobHPScript;
	if (L"CJungleMonsterScript" == _strScriptName)
		return new CJungleMonsterScript;
	if (L"CKrugMiniScript" == _strScriptName)
		return new CKrugMiniScript;
	if (L"CKrugScript" == _strScriptName)
		return new CKrugScript;
	if (L"CMalphiteEDecalScript" == _strScriptName)
		return new CMalphiteEDecalScript;
	if (L"CMalphiteEScript" == _strScriptName)
		return new CMalphiteEScript;
	if (L"CMalphiteQDecalScript" == _strScriptName)
		return new CMalphiteQDecalScript;
	if (L"CMalphiteQModelScript" == _strScriptName)
		return new CMalphiteQModelScript;
	if (L"CMalphiteQScript" == _strScriptName)
		return new CMalphiteQScript;
	if (L"CMalphiteRDecalScript" == _strScriptName)
		return new CMalphiteRDecalScript;
	if (L"CMalphiteRScript" == _strScriptName)
		return new CMalphiteRScript;
	if (L"CMalphiteScript" == _strScriptName)
		return new CMalphiteScript;
	if (L"CMinimapUIScript" == _strScriptName)
		return new CMinimapUIScript;
	if (L"CMinionHPBarPosScript" == _strScriptName)
		return new CMinionHPBarPosScript;
	if (L"CMinionHPRatioScript" == _strScriptName)
		return new CMinionHPRatioScript;
	if (L"CMinionScript" == _strScriptName)
		return new CMinionScript;
	if (L"CMobScript" == _strScriptName)
		return new CMobScript;
	if (L"CMouseCursorUIScript" == _strScriptName)
		return new CMouseCursorUIScript;
	if (L"CMoveCursorUIScript" == _strScriptName)
		return new CMoveCursorUIScript;
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
	if (L"CScorePanelScript" == _strScriptName)
		return new CScorePanelScript;
	if (L"CSoundTestScript" == _strScriptName)
		return new CSoundTestScript;
	if (L"CStructureScript" == _strScriptName)
		return new CStructureScript;
	if (L"CTurretAttackScript" == _strScriptName)
		return new CTurretAttackScript;
	if (L"CTurretHPUIScript" == _strScriptName)
		return new CTurretHPUIScript;
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
	case (UINT)SCRIPT_TYPE::ATTACKRANGESCRIPT:
		return new CAttackRangeScript;
		break;
	case (UINT)SCRIPT_TYPE::BASICATTACKSCRIPT:
		return new CBasicAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::BLUESCRIPT:
		return new CBlueScript;
		break;
	case (UINT)SCRIPT_TYPE::BUSHSCRIPT:
		return new CBushScript;
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
	case (UINT)SCRIPT_TYPE::DRAGONHPUISCRIPT:
		return new CDragonHPUIScript;
		break;
	case (UINT)SCRIPT_TYPE::DRAGONSCRIPT:
		return new CDragonScript;
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
	case (UINT)SCRIPT_TYPE::JINXESCRIPT:
		return new CJinxEScript;
		break;
	case (UINT)SCRIPT_TYPE::JINXRSCRIPT:
		return new CJinxRScript;
		break;
	case (UINT)SCRIPT_TYPE::JINXSCRIPT:
		return new CJinxScript;
		break;
	case (UINT)SCRIPT_TYPE::JINXWSCRIPT:
		return new CJinxWScript;
		break;
	case (UINT)SCRIPT_TYPE::JUNGLEMINIHPSCRIPT:
		return new CJungleMINIHPScript;
		break;
	case (UINT)SCRIPT_TYPE::JUNGLEMOBHPSCRIPT:
		return new CJungleMobHPScript;
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
	case (UINT)SCRIPT_TYPE::MALPHITEEDECALSCRIPT:
		return new CMalphiteEDecalScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITEESCRIPT:
		return new CMalphiteEScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITEQDECALSCRIPT:
		return new CMalphiteQDecalScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITEQMODELSCRIPT:
		return new CMalphiteQModelScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITEQSCRIPT:
		return new CMalphiteQScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITERDECALSCRIPT:
		return new CMalphiteRDecalScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITERSCRIPT:
		return new CMalphiteRScript;
		break;
	case (UINT)SCRIPT_TYPE::MALPHITESCRIPT:
		return new CMalphiteScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIMAPUISCRIPT:
		return new CMinimapUIScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIONHPBARPOSSCRIPT:
		return new CMinionHPBarPosScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIONHPRATIOSCRIPT:
		return new CMinionHPRatioScript;
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
	case (UINT)SCRIPT_TYPE::TURRETHPUISCRIPT:
		return new CTurretHPUIScript;
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
	case SCRIPT_TYPE::ATTACKRANGESCRIPT:
		return L"CAttackRangeScript";
		break;

	case SCRIPT_TYPE::BASICATTACKSCRIPT:
		return L"CBasicAttackScript";
		break;

	case SCRIPT_TYPE::BLUESCRIPT:
		return L"CBlueScript";
		break;

	case SCRIPT_TYPE::BUSHSCRIPT:
		return L"CBushScript";
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

	case SCRIPT_TYPE::DRAGONHPUISCRIPT:
		return L"CDragonHPUIScript";
		break;

	case SCRIPT_TYPE::DRAGONSCRIPT:
		return L"CDragonScript";
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

	case SCRIPT_TYPE::JINXESCRIPT:
		return L"CJinxEScript";
		break;

	case SCRIPT_TYPE::JINXRSCRIPT:
		return L"CJinxRScript";
		break;

	case SCRIPT_TYPE::JINXSCRIPT:
		return L"CJinxScript";
		break;

	case SCRIPT_TYPE::JINXWSCRIPT:
		return L"CJinxWScript";
		break;

	case SCRIPT_TYPE::JUNGLEMINIHPSCRIPT:
		return L"CJungleMINIHPScript";
		break;

	case SCRIPT_TYPE::JUNGLEMOBHPSCRIPT:
		return L"CJungleMobHPScript";
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

	case SCRIPT_TYPE::MALPHITEEDECALSCRIPT:
		return L"CMalphiteEDecalScript";
		break;

	case SCRIPT_TYPE::MALPHITEESCRIPT:
		return L"CMalphiteEScript";
		break;

	case SCRIPT_TYPE::MALPHITEQDECALSCRIPT:
		return L"CMalphiteQDecalScript";
		break;

	case SCRIPT_TYPE::MALPHITEQMODELSCRIPT:
		return L"CMalphiteQModelScript";
		break;

	case SCRIPT_TYPE::MALPHITEQSCRIPT:
		return L"CMalphiteQScript";
		break;

	case SCRIPT_TYPE::MALPHITERDECALSCRIPT:
		return L"CMalphiteRDecalScript";
		break;

	case SCRIPT_TYPE::MALPHITERSCRIPT:
		return L"CMalphiteRScript";
		break;

	case SCRIPT_TYPE::MALPHITESCRIPT:
		return L"CMalphiteScript";
		break;

	case SCRIPT_TYPE::MINIMAPUISCRIPT:
		return L"CMinimapUIScript";
		break;

	case SCRIPT_TYPE::MINIONHPBARPOSSCRIPT:
		return L"CMinionHPBarPosScript";
		break;

	case SCRIPT_TYPE::MINIONHPRATIOSCRIPT:
		return L"CMinionHPRatioScript";
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

	case SCRIPT_TYPE::TURRETHPUISCRIPT:
		return L"CTurretHPUIScript";
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