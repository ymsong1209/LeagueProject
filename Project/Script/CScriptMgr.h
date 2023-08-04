#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BASICATTACKSCRIPT,
	BLUESCRIPT,
	CAMERAMOVESCRIPT,
	CHAMPIONSCRIPT,
	GROMPSCRIPT,
	INGAMECAMERASCRIPT,
	INHIBITORSCRIPT,
	JINXSCRIPT,
	JUNGLEMONSTERSCRIPT,
	KRUGMINISCRIPT,
	KRUGSCRIPT,
	MINIONSCRIPT,
	MOBSCRIPT,
	MURKWOLFMINISCRIPT,
	MURKWOLFSCRIPT,
	NEXUSSCRIPT,
	OTHERPLAYERSCRIPT,
	PLAYERSCRIPT,
	RAZORBEAKMINISCRIPT,
	RAZORBEAKSCRIPT,
	SOUNDTESTSCRIPT,
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
