#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogOfWarShader :
    public CComputeShader
{
private:
	CStructuredBuffer* m_WallBuffer;				// 충돌 검사를 진행해야하는 Object의 개수가 곧 Buffer의 개수임
	CStructuredBuffer* m_RayBuffer;					// 레이를 쏘는 Object의 개수가 곧 Buffer의 개수임
	CStructuredBuffer* m_pOutputBuffer;				// u0

	int				   m_iSourceLightCount;			// Ray가 나오는 Source 의 개수
	int				   m_iSourceLightPerRay;		// 아마 360개 쓸거임 ( Source하나당 뻗어 나오는 Ray의 개수)


public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogOfWarShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogOfWarShader();
};

 

 