#include "pch.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"


#include "CTransform.h"
#include "CAnimator2D.h"
 
#include "CResMgr.h"

 
 
#include "CAnimator3D.h"
 

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{


	SetUsingMovingVec(true);

 
}

void CMeshRender::SetUsingMovingVec(bool _use)
{
	m_bIsUsingMovingVec = _use;


	// ó�� MovingVec�� ������ �ϴ� ���
	if (_use == true && m_vMovingVec.size() == 0)
	{
		// ������ ������ �ִ� Texture�� ���� Vector�ȿ� ����־���.
		MovingStruct Temp;

		Temp.TargetTex = eTargetTexture::OUTPUT;
		Temp.MovingStyle = eTexMovingStyle::NONE;
		Temp.FuncValue = Vec4(1.f, 0.5f, 1.f, 0.f);
		Temp.PreviousPos = Vec2(0.f, 0.f);

		m_vMovingVec.push_back(Temp);


		MovingStruct Temp2;

		Temp2.TargetTex = eTargetTexture::PUNCTURE;
		Temp2.MovingStyle = eTexMovingStyle::COS;
		Temp2.FuncValue = Vec4(1.f, 0.5f, 1.f, 0.f);
		Temp2.PreviousPos = Vec2(0.f, 0.f);

		m_vMovingVec.push_back(Temp2);
	}

	else if (_use == false)
	{
		if (m_vMovingVec.size() != 0)
		{
			m_vMovingVec.clear();
		}
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

	for (int i = 0; i < m_vMovingVec.size(); ++i)
	{
		Vec2 PreviousPos = m_vMovingVec[i].PreviousPos;
		Vec4 FuncValue = m_vMovingVec[i].FuncValue;

		switch (m_vMovingVec[i].MovingStyle)
		{
		case eTexMovingStyle::NONE:
		{
			MovingUse |= (1 << i );
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

		switch (m_vMovingVec[i].TargetTex)
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
		m_vMovingVec[i].PreviousPos = PreviousPos;

	}
	// Texture�� Moving�� Option�� ������ ���� (int_1 �� �����Ǿ� ���� (Std2dMtrl����))

	int a = MovingUse;

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &MovingUse);


}

void CMeshRender::render()
{	
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (UINT i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}


	// ����
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// ����� ���� ������Ʈ
			GetMaterial(i)->UpdateData();

			// ����� �޽� ������Ʈ �� ������
			GetMesh()->render(i);
		}
	}

	// �������� �ڵ�� ������ �� ��ü (note1 start)
	//for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i) {
	//	//���� ������Ʈ
	//	GetMaterial(i)->UpdateData();

	//	//����
	//	GetMesh()->render(i);
	//}
	// (note1 end)


	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}