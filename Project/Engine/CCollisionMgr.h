#pragma once
#include "CSingleton.h"

class CLayer;
class CCollider2D;
class CCollider3D;


union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class CCollisionMgr :
	public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_matrix[MAX_LAYER];
	map<UINT_PTR, bool>		m_mapColID;

public:
	//��� Layer�� ���� �浹�ϴ��� �����ִ� �Լ�
	void LayerCheck(UINT _left, UINT _right);
	void LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}
	const UINT(&GetMatrix())[MAX_LAYER] {
		return m_matrix;
	}
	void SetMatrix(UINT _matrix[MAX_LAYER]);

public:
	void tick();

private:
	//Layer���� object�� �浹�˻縦 ���ش�.
	//Object���� �浹�˻縦 ������ BeginOverlap, OnOverLap, EndOverlap �Ǻ�
	void CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer);

	//Collider���� �浹�ߴ��� Ȯ��
	void CollisionBtw2DObject(CGameObject* _LeftObject, CGameObject* _RightObject);
	void CollisionBtw3DObject(CGameObject* _LeftObject, CGameObject* _RightObject);

	bool CollisionBtw2DCollider(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool CollisionBtw3DCollider(CCollider3D* _pLeft, CCollider3D* _pRight);
};
