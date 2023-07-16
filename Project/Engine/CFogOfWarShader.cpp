#include "pch.h"
#include "CFogOfWarShader.h"

void CFogOfWarShader::UpdateData()
{

	m_iGroupX = ((UINT)m_iSourceLightCount / m_iGroupPerThreadX) + 1;
	m_iGroupY = ((UINT)m_iSourceLightPerRay / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;
}
	