#include "pch.h"
#include "CFogFilterShader.h"


#include "CStructuredBuffer.h"

CFogFilterShader::CFogFilterShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_pCalcedFogInfo(nullptr)
	, m_pOutputFogFilterMap(nullptr)
	, m_iWidth(0)
	, m_iHeight(0)
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
	m_Const.arrInt[0] = (int)m_iWidth;
	m_Const.arrInt[1] = (int)m_iHeight;
	m_Const.arrInt[2] = m_iCntObjectsWithSight;
	m_Const.arrInt[3] = m_iCntRayPerObject;


	m_pCalcedFogInfo->UpdateData_CS(16, true);
	m_pOutputFogFilterMap->UpdateData_CS(0, false);

	// 그룹 수 계산
	m_iGroupX = (m_iWidth / m_iGroupPerThreadX) + 1;
	m_iGroupY = (m_iHeight / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;
}

void CFogFilterShader::Clear()
{
	if (nullptr != m_pOutputFogFilterMap)
	{
		m_pOutputFogFilterMap->Clear_CS(false);
		m_pOutputFogFilterMap = nullptr;
	}

	if (nullptr != m_pCalcedFogInfo)
	{
		m_pCalcedFogInfo->Clear();
		m_pCalcedFogInfo = nullptr;
	}
}
