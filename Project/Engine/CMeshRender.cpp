#include "pch.h"
#include "CMeshRender.h"


#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
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