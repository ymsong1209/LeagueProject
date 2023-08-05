#pragma once
#include "CSingleton.h"
#include "define.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper"))
#endif



class CFontMgr
	: public CSingleton<CFontMgr>
{
private:
	IFW1Factory* m_pFW1Factory;
	IFW1FontWrapper* m_pFontWrapper;

	//=========·Ñ ÆùÆ®==================
	IFW1FontWrapper* m_pRixM;
	IFW1FontWrapper* m_pRixL;
	IFW1FontWrapper* m_pRixB;
	IFW1FontWrapper* m_pRixEB;
	//==================================

public:
	void init();
	void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
	void DrawRixKorM(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
	void DrawRixKorL(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
	void DrawRixKorB(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
	void DrawRixKorEB(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
public:
	CFontMgr();
	~CFontMgr();
};
