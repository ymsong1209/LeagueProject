#include "pch.h"
#include "CPathFinder.h"
#include "CPathFindMgr.h"

#include "CTransform.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

CPathFinder::CPathFinder()
	: CComponent(COMPONENT_TYPE::PATHFINDER)
	, m_vecPath{}
	, m_iPathCount(0)
	, m_iCurPathIdx(-1)
	, m_vNextPos(NaN, NaN, NaN)
{
}

CPathFinder::~CPathFinder()
{
}

void CPathFinder::tick()
{


	//m_vCurPos = Transform()->GetRelativePos();
}

void CPathFinder::FindPath(Vec3 endPos)
{
	// �� ��θ� ã�� ������ ���ο� ��η� ��ü��
	float dist = (endPos - m_vPrevEndPos).Length();

	if (dist > 10.f) //������ ���� �������� ���� ��û�� �������� ���̰� 10�ȼ��ۿ��ȵȴٸ� ��ã�⸦ �������� ���� : �Ź� ����Ѱ���������� ĳ���Ͱ� �������� ���� �����ϱ� ����!
	{
		m_vecPath.clear();
		// ������Ʈ�� ���� ��ġ
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

		// ���� ��θ� �������� ��θ� �޾ƿ�
		m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
		m_iPathCount = m_vecPath.size();
		m_iCurPathIdx = 0;

		m_vNextPos = Vec3(m_vecPath[0].x, m_vecPath[0].y, m_vecPath[0].z);

		m_vPrevEndPos = endPos;
	}

}

void CPathFinder::FindNextPath()
{
	m_iCurPathIdx++;

	// ���� ��ΰ� �ִٸ� �� ��ġ�� ��ȯ�ϰ�, ��� �ε��� ������Ŵ
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
	}
	// ���� ��ΰ� ���ٸ� �� �� ���� ��ġ ��ȯ
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
	}
}

//void CPathFinder::FindPathMousePicking()  //���� ī�޶󿡼� ���̸� ��������, ���ݸ������� �������� endPos�� �����ؼ� FindPath ȣ��.(���콺 ��ŷ �̵�)
//{
//	CGameObject* LoLMap = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
//	/*if (!LayerObj.empty())
//	{
//		if (LayerObj[0] && !LayerObj[0]->IsDead())
//			LoLMap = LayerObj[0];
//	}*/
//
//	if (LoLMap != nullptr && !LoLMap->IsDead())  //�� �ݸ����� ���������� �����Ұ�츸 ȣ��
//	{
//		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
//		tRay ray = MainCam->GetRay();
//		IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, LoLMap);
//		Vec3 EndPos = result.vCrossPoint;
//		FindPath(EndPos);
//	}
//}

void CPathFinder::ClearPath()
{
	m_vNextPos = Vec3(NaN, NaN, NaN);
}

