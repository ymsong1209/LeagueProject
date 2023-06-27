#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"

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

	
	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i) {
		//���� ������Ʈ
		GetMaterial(i)->UpdateData();

		//����
		GetMesh()->render(i);
	}


	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();
}