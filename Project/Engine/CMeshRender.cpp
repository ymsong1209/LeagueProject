#include "pch.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{

	// 움직일 여지가 있는 Texture들 전부 Vector안에 집어넣어줌.
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
	// vector에 있는 Texture들에 대해서
	// 어떤 움직임을 취하고 있는지 찾아서
	// 참조해야할 UV Offset 값을 계산해준다.
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
			// FuncValue.y : x 계수
			// FuncValue.z : y 절편
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * PreviousPos.x + FuncValue.z;
		}
			break;
		case eTexMovingStyle::PARABOLA:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : x^2 계수
			// FuncValye.z : x 계수
			// FuncValue.w : y 절편
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * PreviousPos.x * PreviousPos.x + FuncValue.z * PreviousPos.x + FuncValue.w;
		}
			break;
		case eTexMovingStyle::SIN:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : sin 계수
			// FuncValue.z : 주파수
			// FuncValue.w : y절편
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * sin(FuncValue.z * PreviousPos.x) + FuncValue.w;
		}
			break;
		case eTexMovingStyle::COS:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : cos 계수
			// FuncValue.z : 주파수
			// FuncValue.w : y절편
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
	// Texture가 Moving을 Option을 쓰는지 전달 (int_1 에 지정되어 있음 (Std2dMtrl기준))
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &MovingUse);


}

void CMeshRender::render()
{	
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// 재질 업데이트
	GetMaterial()->UpdateData();

	// 렌더
	GetMesh()->render();

	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();
}