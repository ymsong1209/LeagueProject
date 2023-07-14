#include "pch.h"
#include "CPathFinder.h"
#include "CPathFindMgr.h"

#include "CTransform.h"

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
	m_vecPath.clear();

	// ������Ʈ�� ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// ���� ��θ� �������� ��θ� �޾ƿ�
	m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
	m_iPathCount = m_vecPath.size();
	m_iCurPathIdx = 0;

	m_vNextPos = Vec3(m_vecPath[0].x, m_vecPath[0].y, m_vecPath[0].z);
}

void CPathFinder::FindNextPath()
{
	m_iCurPathIdx++;

	// ���� ��ΰ� �ִٸ� �� ��ġ�� ��ȯ�ϰ�, ��� �ε��� ������Ŵ
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount) 
	{	
		m_vNextPos =  Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
	}
	// ���� ��ΰ� ���ٸ� �� �� ���� ��ġ ��ȯ
	else 
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
	}
}
