#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CCollisionMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CFontMgr.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
{
}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// 메인 윈도우 핸들
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// 해상도에 맞는 작업영역 크기 조정
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);


	// Device 초기화
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
		return E_FAIL;
	}


	// Manager 초기화
	CPathMgr::GetInst()->init();

	CKeyMgr::GetInst()->init();

	CTimeMgr::GetInst()->init();

	CResMgr::GetInst()->init();

	CRenderMgr::GetInst()->init();

	CFontMgr::GetInst()->init();

	CLevelMgr::GetInst()->init();		
	


	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();

	// Event 처리
	CEventMgr::GetInst()->tick();
}

void CEngine::tick()
{
	// Manager Tick
	CResMgr::GetInst()->tick();
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();	

	// FMOD Update
	CSound::g_pFMOD->update();

	// Level Update
	CLevelMgr::GetInst()->tick();
	CCollisionMgr::GetInst()->tick();
}

void CEngine::render()
{	
	CRenderMgr::GetInst()->render();	

	CTimeMgr::GetInst()->render();
}
