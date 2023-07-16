#include "pch.h"
#include "CFogOfWarShader.h"
#include "CStructuredBuffer.h"

CFogOfWarShader::CFogOfWarShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_pRayBuffer(nullptr)
	, m_pWallBuffer(nullptr)
	, m_pOutputBuffer(nullptr)
	, m_iSourceLightCount(0)
	, m_iSourceLightPerRay(0)
	, m_iColliderVecCount(0)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}


void CFogOfWarShader::UpdateData()
{

	m_iGroupX = ((UINT)m_iSourceLightCount / m_iGroupPerThreadX) + 1;
	m_iGroupY = ((UINT)m_iSourceLightPerRay / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;

	// 구조화버퍼 전달
	m_pRayBuffer->UpdateData_CS(16, true);
	m_pWallBuffer->UpdateData_CS(17, true);
	m_pOutputBuffer->UpdateData_CS(0, false);
}

void CFogOfWarShader::Clear()
{
	if (nullptr != m_pRayBuffer)
	{
		m_pRayBuffer->Clear_CS(true);
		m_pRayBuffer = nullptr;
	}

	if (nullptr != m_pWallBuffer)
	{
		m_pWallBuffer->Clear_CS(true);
		m_pWallBuffer = nullptr;
	}

	if (nullptr != m_pOutputBuffer)
	{
		m_pOutputBuffer->Clear_CS(false);
		m_pOutputBuffer = nullptr;
	}
	
}

