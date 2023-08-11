#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CFontMgr.h"
#include "CKeyMgr.h"

CTimeMgr::CTimeMgr()
	: m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_iCallCount(0)
	, m_fDeltaTime(0.f)
	, m_fTime(0.f)
	
{

}

CTimeMgr::~CTimeMgr()
{

}



void CTimeMgr::init()
{
	// 1�ʴ� ī���� ������
	QueryPerformanceFrequency(&m_llFrequency);
		
	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{	
	QueryPerformanceCounter(&m_llCurCount);

	// tick ���� �ð�
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;
	m_fEditorDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// ���� �ð�
	m_fTime += m_fDeltaTime;
	m_fAccTime += m_fDeltaTime;
	// �Լ� ȣ�� Ƚ��
	++m_iCallCount;
		
	// ���� ī��Ʈ ���� ���� ī��Ʈ�� ����
	m_llPrevCount = m_llCurCount;

	// Deltatime ������/������/������Ű��
	m_fDeltaTime = m_fDeltaTime * m_fTimeScale;

	//saturate
	if (m_fDeltaTime >= 1.f) m_fDeltaTime = 1.f;
	if (m_fDeltaTime < 0.f) m_fDeltaTime = 0.f;

	// GlobalData ����
	GlobalData.tDT = m_fDeltaTime;
	GlobalData.tEditDT = m_fEditorDeltaTime;
	GlobalData.tAccTime += m_fDeltaTime;

}

void CTimeMgr::render()
{
	// 1�ʿ� �ѹ�
	static wchar_t szBuff[256] = {};

	if (1.f <= m_fTime)
	{		
		swprintf_s(szBuff, L"FPS : %d, DT : %f, EditDT : %f", m_iCallCount, m_fDeltaTime, m_fEditorDeltaTime);
		//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);	

		m_fTime = 0.f;
		m_iCallCount = 0;
	}

	CFontMgr::GetInst()->DrawFont(szBuff, 10, 20, 16, FONT_RGBA(255, 0, 0, 255));
	static wchar_t szBuff2[256] = {};
	swprintf_s(szBuff2, L"Press F11 to progress IMGUI");
	CFontMgr::GetInst()->DrawFont(szBuff2, 10, 36, 16, FONT_RGBA(255, 0, 0, 255));
	static wchar_t szBuff3[256] = {};
	swprintf_s(szBuff3, L"Press F7 to progress DebugShapes");
	CFontMgr::GetInst()->DrawFont(szBuff3, 10, 52, 16, FONT_RGBA(255, 0, 0, 255));
}
