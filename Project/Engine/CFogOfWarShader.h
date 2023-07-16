#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogOfWarShader :
	public CComputeShader
{
private:
	CStructuredBuffer* m_pRayBuffer;				// 레이를 쏘는 Object의 개수가 곧 Buffer의 개수임                (t16)
	CStructuredBuffer* m_pWallBuffer;				// 충돌 검사를 진행해야하는 Object의 개수가 곧 Buffer의 개수임   (t17)
	CStructuredBuffer* m_pOutputBuffer;				// OutputBuffer / ReadWrite                                      (u0)

	int				   m_iSourceLightCount;			// Ray가 나오는 Source 의 개수                                   (g_int_0)
	int				   m_iSourceLightPerRay;		// 아마 360개 쓸거임 ( Source하나당 뻗어 나오는 Ray의 개수)      (g_int_1)
	int                m_iColliderVecCount;         // Compute Shader에 전달해주는 Collider들의 총 개수              (g_int_2)

public:
	void SetSourceLightCount(int _iSourceLightCount) { m_iSourceLightCount = _iSourceLightCount; m_Const.arrInt[0] = _iSourceLightCount; }
	void SetSourceLightPerRay(int _iSourceLightPerRay) { m_iSourceLightPerRay = _iSourceLightPerRay; m_Const.arrInt[1] = _iSourceLightPerRay; }
	void SetColliderVecCount(int _iColliderVecCount) { m_iColliderVecCount = _iColliderVecCount; m_Const.arrInt[2] = _iColliderVecCount; }

	void SetRayBuffer(CStructuredBuffer* _buffer) { m_pRayBuffer = _buffer; }
	void SetWallBuffer(CStructuredBuffer* _buffer) { m_pWallBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer;  }



public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogOfWarShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogOfWarShader();
};

 

 