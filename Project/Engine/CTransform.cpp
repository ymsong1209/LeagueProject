#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)	
	, m_vRelativeDir{
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)}	
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}


void CTransform::finaltick()
{
	m_matWorldScale = XMMatrixIdentity(); //������� �����
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);


	m_matWorldRot = XMMatrixIdentity();
	m_matWorldRot = XMMatrixRotationX(m_vRelativeRot.x);
	m_matWorldRot *= XMMatrixRotationY(m_vRelativeRot.y);
	m_matWorldRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	m_matWorldPos = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = m_matWorldScale * m_matWorldRot * m_matWorldPos;


	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	//���⺤�Ϳ� ȸ������� ���ϸ� ���� ���� �ִ� ������ ���´�.
	for (int i = 0; i < 3; ++i)
	{
		m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorldRot);
	}

	// �θ� ������Ʈ Ȯ��
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale); //ũ�� ���Լ�,nullptr:�Ǻ���

			// ���� = ���ÿ��� * �θ�ũ�� �� * �θ� ����(ũ��/ȸ��/�̵�)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorldScale *= pParent->Transform()->m_matWorldScale;
			m_matWorldRot *= pParent->Transform()->m_matWorldRot;
			m_matWorldPos *= pParent->Transform()->m_matWorldPos;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld); //transformNormal�� m_matWorld�� �̵��� ����
		m_vWorldDir[i].Normalize(); //Scale�� �����������Ƿ� ����ȭ ����
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	// ��ġ���� ������ۿ� ���� �� ���ε�		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}


Matrix CTransform::GetWorldRotMat()
{
	Matrix matWorldRot = m_matWorldRot;

	CGameObject* pParent = GetOwner()->GetParent();

	while (pParent)
	{
		matWorldRot *= pParent->Transform()->m_matWorldRot;
		pParent = pParent->GetParent();
	}

	return matWorldRot;
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos	, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot	, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);	    
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{	
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
}
