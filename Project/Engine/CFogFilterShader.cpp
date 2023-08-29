#include "pch.h"
#include "CFogFilterShader.h"


#include "CStructuredBuffer.h"

CFogFilterShader::CFogFilterShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_pCalcedFogInfo(nullptr)
	, m_pFogFilterMap(nullptr)
	, m_iCntObjectsWithSight(0)
	, m_iCntRayPerObject(360)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CFogFilterShader::~CFogFilterShader()
{
}


void CFogFilterShader::UpdateData()
{
	// 0 : 필터맵의 넓이
	// 1 : 필터맵의 높이
	// 2 : 시야를 가진 오브젝트 개수
	// 3 : 오브젝트가 쏠 레이의 개수
	m_Const.arrInt[0] = (int)m_pFogFilterMap->Width();
	m_Const.arrInt[1] = (int)m_pFogFilterMap->Height();
	m_Const.arrInt[2] = m_iCntObjectsWithSight;
	m_Const.arrInt[3] = m_iCntRayPerObject;

	// u1 : 계산에 필요한 정보, u0 : 계산된 안개필터맵
	m_pCalcedFogInfo->UpdateData_CS(1, false);
	m_pFogFilterMap->UpdateData_CS(0, false);

	// 실행 시킬 스레드 그룹 수 지정
	m_iGroupX = ((UINT)m_pFogFilterMap->Width() / m_iGroupPerThreadX) + 1;
	m_iGroupY = ((UINT)m_pFogFilterMap->Height() / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;
}

void CFogFilterShader::Clear()
{
	if (nullptr != m_pFogFilterMap)
	{
		m_pFogFilterMap->Clear_CS(false);
		m_pFogFilterMap = nullptr;
	}

	if (nullptr != m_pCalcedFogInfo)
	{
		m_pCalcedFogInfo->Clear_CS(false);
		m_pCalcedFogInfo = nullptr;
	}
}
