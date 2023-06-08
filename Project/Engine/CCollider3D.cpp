#include "pch.h"
#include "CCollider3D.h"
#include "CScript.h"
#include "components.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_Shape(COLLIDER3D_TYPE::SPHERE)
	, m_bAbsolute(false)
	, m_iCollisionCount(0)
{
}

CCollider3D::CCollider3D(const CCollider3D& _other)
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vOffsetScale(_other.m_vOffsetScale)
	, m_bAbsolute(_other.m_bAbsolute)
	, m_Shape(_other.m_Shape)
{

}

CCollider3D::~CCollider3D()
{
}


void CCollider3D::finaltick()
{
	// �浹 ȸ���� ������ ���
	assert(0 <= m_iCollisionCount);
	m_matColliderScale = XMMatrixIdentity();
	m_matColliderScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matColliderPos = XMMatrixIdentity();
	m_matColliderPos = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);


	m_matCollider3D = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matCollider3D *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);
	//ũ�� X ȸ�� X �̵� (ȸ���� ����)
	const Matrix& matWorld = Transform()->GetWorldMat(); //���� ���� ���

	if (m_bAbsolute)
	{
		//�θ��� Scale���ֱ�
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		//Collider�� Offset * �θ� ũ���� ����� * �θ�
		m_matCollider3D = m_matCollider3D * matParentScaleInv * matWorld;

	}
	else
	{
		// �浹ü ���� * ������Ʈ ����
		m_matCollider3D *= matWorld;

		// �浹ü scale update
		m_matColliderScale *= Transform()->GetWorldScaleMat();
		m_matColliderPos *= Transform()->GetWorldPosMat();
	}


	// DebugShape ��û
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	// �������� 0.f�� ������ GameObject�� ������� Collider�� �������.
	if (COLLIDER3D_TYPE::SPHERE == m_Shape)
		DrawDebugSphere(m_matCollider3D, vColor, 0.f);
	else
		DrawDebugCube(m_matCollider3D, vColor, 0.f);
}

void CCollider3D::BeginOverlap(CCollider3D* _Other)
{
	m_iCollisionCount += 1;

	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider3D::OnOverlap(CCollider3D* _Other)
{
	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider3D::EndOverlap(CCollider3D* _Other)
{
	m_iCollisionCount -= 1;

	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

void CCollider3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_Shape, sizeof(UINT), 1, _File);
}

void CCollider3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_Shape, sizeof(UINT), 1, _File);
}