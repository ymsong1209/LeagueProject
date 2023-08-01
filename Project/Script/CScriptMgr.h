#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BASICATTACKSCRIPT,
	CAMERAMOVESCRIPT,
	CHAMPIONSCRIPT,
	GROMPSCRIPT,
	INGAMECAMERASCRIPT,
	INHIBITORSCRIPT,
	JINXSCRIPT,
	JUNGLEMONSTERSCRIPT,
	MINIONSCRIPT,
	MOBSCRIPT,
	NEXUSSCRIPT,
	OTHERPLAYERSCRIPT,
	PLAYERSCRIPT,
	STRUCTURESCRIPT,
	TURRETATTACKSCRIPT,
	TURRETSCRIPT,
	UNITSCRIPT,
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
