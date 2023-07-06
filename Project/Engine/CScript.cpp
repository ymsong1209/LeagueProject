#include "pch.h"
#include "CScript.h"


CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)	
	, m_iScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}

void CScript::AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc)
{
	m_vecParam.push_back(tScriptParam{ eParam , _pData , _Desc });
}

void CScript::AddScriptTexParam(Ptr<CTexture>& _tex, const string& _Desc)
{
	m_vecTexParam.push_back(tScriptTexParam{ _tex,_Desc });
}



void CScript::SetScriptTexParam(Ptr<CTexture> pCurTex, const string& _Desc)
{
	for (size_t i = 0; i < m_vecTexParam.size(); ++i) {
		if (m_vecTexParam[i].strDesc == _Desc) {
			m_vecTexParam[i].tex = pCurTex;
		}
	}
}