#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogOfWarShader :
    public CComputeShader
{
private:
	CStructuredBuffer* m_WallBuffer;				// �浹 �˻縦 �����ؾ��ϴ� Object�� ������ �� Buffer�� ������
	CStructuredBuffer* m_RayBuffer;					// ���̸� ��� Object�� ������ �� Buffer�� ������
	CStructuredBuffer* m_pOutputBuffer;				// u0

	int				   m_iSourceLightCount;			// Ray�� ������ Source �� ����
	int				   m_iSourceLightPerRay;		// �Ƹ� 360�� ������ ( Source�ϳ��� ���� ������ Ray�� ����)


public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogOfWarShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogOfWarShader();
};

 

 