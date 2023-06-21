#include "pch.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{

	// ������ ������ �ִ� Texture�� ���� Vector�ȿ� ����־���.
	MovingStruct Temp;

	Temp.MovingStyle = eTexMovingStyle::COS;
	Temp.FuncValue = Vec4(1.f, 0.5f, 1.f, 0.f);
	Temp.PreviousPos = Vec2(0.f, 0.f);

	for (int i = 0; i < (int)eTargetTexture::END; ++i)
	{
		Temp.TargetTex = (eTargetTexture)i;
		MovingVec.push_back(Temp);
	}
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
	// vector�� �ִ� Texture�鿡 ���ؼ�
	// � �������� ���ϰ� �ִ��� ã�Ƽ�
	// �����ؾ��� UV Offset ���� ������ش�.
	int MovingUse = 0;

	for (int i = 0; i < MovingVec.size(); ++i)
	{
		Vec2 PreviousPos = MovingVec[i].PreviousPos;
		Vec4 FuncValue = MovingVec[i].FuncValue;

		switch (MovingVec[i].MovingStyle)
		{
		case eTexMovingStyle::NONE:
		{
			MovingUse |= (1 << i);
		}
			break;
		case eTexMovingStyle::HORIZONTAL:
		{
			// FuncValue.x : dx / dt
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
		}
			break;
		case eTexMovingStyle::VERTICAL:
		{
			// FuncValue.x : dy / dt
			PreviousPos.y += GlobalData.tEditDT * FuncValue.x;
		}
			break;
		case eTexMovingStyle::LINEAR:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : x ���
			// FuncValue.z : y ����
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * PreviousPos.x + FuncValue.z;
		}
			break;
		case eTexMovingStyle::PARABOLA:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : x^2 ���
			// FuncValye.z : x ���
			// FuncValue.w : y ����
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * PreviousPos.x * PreviousPos.x + FuncValue.z * PreviousPos.x + FuncValue.w;
		}
			break;
		case eTexMovingStyle::SIN:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : sin ���
			// FuncValue.z : ���ļ�
			// FuncValue.w : y����
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * sin(FuncValue.z * PreviousPos.x) + FuncValue.w;
		}
			break;
		case eTexMovingStyle::COS:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : cos ���
			// FuncValue.z : ���ļ�
			// FuncValue.w : y����
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * cos(FuncValue.z * PreviousPos.x) + FuncValue.w;
		}
			break;
		case eTexMovingStyle::END:
			break;
		default:
			break;
		}

		switch (MovingVec[i].TargetTex)
		{
		case eTargetTexture::OUTPUT:
		{
			GetMaterial()->SetScalarParam(VEC2_2, &PreviousPos);
		}
			break;
		case eTargetTexture::PUNCTURE:
		{
			GetMaterial()->SetScalarParam(VEC2_3, &PreviousPos);
		}
			break;
		case eTargetTexture::END:
			break;
		default:
			break;
		}

		// Previous Postion Update
		MovingVec[i].PreviousPos = PreviousPos;

	}
	// Texture�� Moving�� Option�� ������ ���� (int_1 �� �����Ǿ� ���� (Std2dMtrl����))
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &MovingUse);


}

void CMeshRender::render()
{	
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// ���� ������Ʈ
	GetMaterial()->UpdateData();

	// ����
	GetMesh()->render();

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();
}