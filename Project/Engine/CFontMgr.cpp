#include "pch.h"
#include "CFontMgr.h"
#include "CDevice.h"


CFontMgr::CFontMgr()
	: m_pFW1Factory(nullptr)
	, m_pFontWrapper(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	if (nullptr != m_pFontWrapper)
		m_pFontWrapper->Release();

	if (nullptr != m_pRixM)
		m_pRixM->Release();

	if (nullptr != m_pRixL)
		m_pRixL->Release();

	if (nullptr != m_pRixB)
		m_pRixB->Release();

	if (nullptr != m_pRixEB)
		m_pRixEB->Release();
}

void CFontMgr::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Rix��� M", &m_pRixM))) //���ο� ��Ʈ �߰�
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"���������� �׿� Light", &m_pRixL))) //���ο� ��Ʈ �߰�
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Rix��� B", &m_pRixB))) //���ο� ��Ʈ �߰�
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Rix��� EB", &m_pRixEB))) //���ο� ��Ʈ �߰�
	{
		assert(NULL);
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawRixKorM(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pRixM->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | FW1_CENTER      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawRixKorL(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pRixL->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | FW1_CENTER       // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawRixKorB(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pRixB->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | FW1_CENTER       // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawRixKorEB(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pRixEB->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | FW1_CENTER       // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
