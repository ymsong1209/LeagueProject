#include "pch.h"
#include "CMRT.h"

#include "CDevice.h"

CMRT::CMRT()
	: m_RT{}
	, m_RTCount(0)
	, m_tViewPort{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>(&_RTArr)[8], Vec4(&_Clear)[8], Ptr<CTexture> _DSTex)
{
	for (UINT i = 0; i < 8; ++i)
	{
		if (nullptr == _RTArr[i])
		{
			m_RTCount = i;
			break;
		}

		m_RT[i] = _RTArr[i];
	}

	memcpy(m_Clear, _Clear, sizeof(Vec4) * 8);

	m_DSTex = _DSTex;

	//// ViewPort 설정	
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;

	m_tViewPort.Width = m_RT[0]->Width();
	m_tViewPort.Height = m_RT[0]->Height();

	m_tViewPort.MinDepth = 0;
	m_tViewPort.MaxDepth = 1;
}

void CMRT::OMSet()
{
	ID3D11RenderTargetView* RTView[8] = {};
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		RTView[i] = m_RT[i]->GetRTV().Get();
	}

	if (nullptr != m_DSTex)
		CONTEXT->OMSetRenderTargets(m_RTCount, RTView, m_DSTex->GetDSV().Get());
	else
		CONTEXT->OMSetRenderTargets(m_RTCount, RTView, nullptr);

	// 뷰포트 설정
	CONTEXT->RSSetViewports(1, &m_tViewPort);
}

void CMRT::Clear()
{
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_RT[i]->GetRTV().Get(), m_Clear[i]);
	}

	if (nullptr != m_DSTex)
	{
		CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}