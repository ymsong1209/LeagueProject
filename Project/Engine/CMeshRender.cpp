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

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	
	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i) {
		//재질 업데이트
		GetMaterial(i)->UpdateData();

		//렌더
		GetMesh()->render(i);
	}


	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();
}