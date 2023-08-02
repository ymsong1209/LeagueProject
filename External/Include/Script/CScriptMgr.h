#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BASICATTACKSCRIPT,
	CAMERAMOVESCRIPT,
	CHAMPIONSCRIPT,
	CHARACTERUISCRIPT,
	COOLDOWNUISCRIPT,
	EXPRATIOUISCRIPT,
	HPMPRATIOUISCRIPT,
	INGAMECAMERASCRIPT,
	INHIBITORSCRIPT,
	INVENTORYUISCRIPT,
	JINXSCRIPT,
	JUNGLEMONSTERSCRIPT,
	MINIMAPUISCRIPT,
	MINIONSCRIPT,
	MOBSCRIPT,
	NEXUSSCRIPT,
	OTHERPLAYERSCRIPT,
	PLAYERSCRIPT,
	STRUCTURESCRIPT,
	TURRETSCRIPT,
	UISCRIPT,
	UNITSCRIPT,
	WORLDHPSPAWNSCRIPT,
	WORLDHPUISCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
