#include "pch.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"


#include "CTransform.h"
#include "CAnimator2D.h"
 
#include "CResMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
 
#include "CAnimator3D.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
 
#include "CResMgr.h"
#include "CTexture.h"


CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)	
{
	memset(&m_tMeshMoveData, 0, sizeof(struct tMeshMoveData));

	// Test Code
	//SetPunctureTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Dots.jpg"));
	//SetTexMovingStyle(MovTexType::PUNCTURE, MovTexMoveType::VERTICAL);
	//SetFuncValue(MovTexType::PUNCTURE, Vec4(0.5f, 0.f, 0.f, 0.f));
 
}

CMeshRender::CMeshRender(const CMeshRender& _other)
	: CRenderComponent(_other)
	, m_tMeshMoveData(_other.m_tMeshMoveData)
{
	for (int i = 0; i < (UINT)MESH_TEX_PARAM::MESH_TEX_END; ++i)
	{
		m_arrMeshTex[i] = _other.m_arrMeshTex[i];
	}
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::SetOutputTexture(Ptr<CTexture> _Tex)
{
	if (GetMaterial(0) != nullptr)
	{
		GetMaterial(0)->SetTexParam(TEX_0, _Tex);
	}
}

Ptr<CTexture> CMeshRender::GetOutputTexture(Ptr<CTexture> _Tex)
{
	if (GetMaterial(0) != nullptr)
	{
		return GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
	}

	else
	{
		return nullptr;
	}
}

void CMeshRender::SetFuncValue(MovTexType _TexType, Vec4 _Value)
{
	switch (_TexType)
	{
	case MovTexType::OUTPUT:
	{
		m_tMeshMoveData.OutputTexFuncValue = _Value;
	}
		break;
	case MovTexType::PUNCTURE:
	{
		m_tMeshMoveData.PunctureTexFuncValue = _Value;
	}
		break;
	case MovTexType::ADDITIVE:
		break;
	case MovTexType::END:
		break;
	default:
		break;
	}
}

void CMeshRender::SetTexMovingStyle(MovTexType _TexType, MovTexMoveType _Type)
{
	if (_TexType == MovTexType::PUNCTURE)
		m_tMeshMoveData.PunctureTexMovingStyle = (int)_Type;
	else if (_TexType == MovTexType::OUTPUT)
		m_tMeshMoveData.OutputTexMovingStyle = (int)_Type;
}

void CMeshRender::SetOffsetValue(MovTexType _TexType, Vec2 _OffsetValue)
{
	if (_TexType == MovTexType::OUTPUT)
		m_tMeshMoveData.OutputTexPreviousPos = _OffsetValue;
	else if (_TexType == MovTexType::PUNCTURE)
		m_tMeshMoveData.PunctureTexPreviousPos = _OffsetValue;
}

Vec4 CMeshRender::GetFuncValue(MovTexType _TexType)
{
	if (_TexType == MovTexType::OUTPUT)
		return m_tMeshMoveData.OutputTexFuncValue;
	else if (_TexType == MovTexType::PUNCTURE)
		return m_tMeshMoveData.PunctureTexFuncValue;
	else
		return Vec4(0.f, 0.f, 0.f, 0.f); // �ǹ̾��� ��. �� �б�� ������ �Լ� �߸� ������� ����.
}

MovTexMoveType CMeshRender::GetTexMovingStyle(MovTexType _TexType)
{
	if (_TexType == MovTexType::OUTPUT)
		return (MovTexMoveType)m_tMeshMoveData.OutputTexMovingStyle;

	else if (_TexType == MovTexType::PUNCTURE)
		return (MovTexMoveType)m_tMeshMoveData.PunctureTexMovingStyle;

	else
		return (MovTexMoveType::END); // �� �б�� ������ �ڵ� �߸����� �ִ� ����.
}

Vec2 CMeshRender::GetOffsetValue(MovTexType _TexType)
{
	switch (_TexType)
	{
	case MovTexType::OUTPUT:
	{
		return m_tMeshMoveData.OutputTexPreviousPos;
	}
		break;
	case MovTexType::PUNCTURE:
	{
		return m_tMeshMoveData.PunctureTexPreviousPos;
	}
		break;
	case MovTexType::ADDITIVE:
	case MovTexType::END:
	{
		return Vec2(0.f, 0.f); // ����� ������ �ǵ���� �Լ��� ���� ���� ���� ����.
	}
	}
}

void CMeshRender::CalculateNextOffset(int _MoveStyle, Vec2& _PreviousPos, Vec4 _FunctionValue, float _DT)
{
	// 1. None, 2. Vertical, 3. Linear, 4. Parabola, 5. Sin, 6. Cos
	if (_MoveStyle == 0) // None
	{

	}
	else if (_MoveStyle == 1)  // Horizontal
	{
		// FunctionValue.x : dx / dt
		_PreviousPos.x += _DT * _FunctionValue.x;
	}
	else if (_MoveStyle == 2) // Vertical
	{
		// FunctionValue.x : dy / dt
		_PreviousPos.y += _DT * _FunctionValue.x;
	}
	else if (_MoveStyle == 3) // Linear
	{
		// FunctionValue.x : dx / dt;
		// FunctionValue.y : x ���
		// FunctionValue.z : y ����
		_PreviousPos.x += _DT * _FunctionValue.x;
		_PreviousPos.y = _FunctionValue.y * _PreviousPos.x + _FunctionValue.z;
	}
	else if (_MoveStyle == 4) // Parabola
	{
		// FuncValue.x : dx / dt
		// FuncValue.y : x^2 ���
		// FuncValye.z : x ���
		// FuncValue.w : y ����
		_PreviousPos.x += _DT * _FunctionValue.x;
		_PreviousPos.y = _FunctionValue.y * _PreviousPos.x * _PreviousPos.x + _FunctionValue.z * _PreviousPos.x + _FunctionValue.w;

	}
	else if (_MoveStyle == 5) // Sin
	{
		// FuncValue.x : dx / dt
		// FuncValue.y : sin ���
		// FuncValue.z : ���ļ�
		// FuncValue.w : y����
		_PreviousPos.x += _DT * _FunctionValue.x;
		_PreviousPos.y = _FunctionValue.y + sin(_FunctionValue.z * _PreviousPos.x) + _FunctionValue.w;
	}
	else if (_MoveStyle == 6) // Cos
	{
		// FuncValue.x : dx / dt
		// FuncValue.y : cos ���
		// FuncValue.z : ���ļ�
		// FuncValue.w : y����
		_PreviousPos.x += _DT * _FunctionValue.x;
		_PreviousPos.y = _FunctionValue.y * cos(_FunctionValue.z * _PreviousPos.x) + _FunctionValue.w;
	}
}

void CMeshRender::finaltick()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	LEVEL_STATE CurLevelState = CLevelMgr::GetInst()->GetCurLevel()->GetState();
	float SmallTime = 0.f;

	if (CurLevelState == LEVEL_STATE::STOP)
		SmallTime = GlobalData.tEditDT;
	else
		SmallTime = GlobalData.tDT;
	
	// Output Texture
	CalculateNextOffset(m_tMeshMoveData.OutputTexMovingStyle, m_tMeshMoveData.OutputTexPreviousPos,
		m_tMeshMoveData.OutputTexFuncValue, SmallTime);

	//// Puncture Texture
	CalculateNextOffset(m_tMeshMoveData.PunctureTexMovingStyle, m_tMeshMoveData.PunctureTexPreviousPos,
		m_tMeshMoveData.PunctureTexFuncValue, SmallTime);
}

void CMeshRender::render()
{	
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();


	// MeshMoveData Update
	CConstBuffer* pMeshMoveBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MESHRENDER);

	// Mesh Texture Update (PunctureTex, AdditiveTex�� ���ε��Ѵ�. OutputTex�� Material���� ���õ�)
	if (m_arrMeshTex[MESH_TEX_PARAM::PUNCTURE] == nullptr)
	{
		m_tMeshMoveData.isPunctureTextureUsed = 0;
		CTexture::Clear(14); 
	}

	else
	{
		m_tMeshMoveData.isPunctureTextureUsed = 1;
		m_arrMeshTex[MESH_TEX_PARAM::PUNCTURE]->UpdateData(14, PS_ALL_STAGES);
	}

	if (m_arrMeshTex[MESH_TEX_PARAM::ADDITIVE] == nullptr)
	{
		m_tMeshMoveData.isAdditiveTextureUsed = 0;
		CTexture::Clear(15);
	}

	else
	{
		m_tMeshMoveData.isAdditiveTextureUsed = 1;
		m_arrMeshTex[MESH_TEX_PARAM::ADDITIVE]->UpdateData(15, PS_ALL_STAGES);
	}

	pMeshMoveBuffer->SetData(&m_tMeshMoveData);
	pMeshMoveBuffer->UpdateData();


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

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::SaveToLevelFile(FILE* _File)
{	
	//CRenderComponent::SaveToLevelFile(_File);

	//
	//fwrite(&m_bIsUsingMovingVec, sizeof(bool), 1, _File);

	//// ���࿡ MovingVec�� ����ϰ� �־��ٸ� �̿� ���� ������ ��������� �ϳ�.
	//if (m_bIsUsingMovingVec)
	//{
	//	//movingvec size ����
	//	int vecsize = m_vMovingVec.size();
	//	fwrite(&vecsize, sizeof(int), 1, _File);

	//	for (int i = 0; i < m_vMovingVec.size(); ++i)
	//	{
	//		fwrite(&m_vMovingVec[i], sizeof(MovingStruct), 1, _File);
	//	}
	//}	 
}

void CMeshRender::LoadFromLevelFile(FILE* _File)
{
	//CRenderComponent::LoadFromLevelFile(_File);

	//fread(&m_bIsUsingMovingVec, sizeof(bool), 1, _File);

	//if (m_bIsUsingMovingVec)
	//{
	//	SetUsingMovingVec(true);

	//	//movingvec size �б�
	//	int vecsize;
	//	fread(&vecsize, sizeof(int), 1, _File);

	//	for (int i = 0; i < vecsize; ++i)
	//	{
	//		MovingStruct movestruct;
	//		fread(&movestruct, sizeof(MovingStruct), 1, _File);
	//		m_vMovingVec.push_back(movestruct);
	//	}		
	//}
	//else
	//{
	//	SetUsingMovingVec(false);
	//}
}

void CMeshRender::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	//CRenderComponent::SaveToLevelJsonFile(_objValue, allocator);

	//_objValue.AddMember("bIsUsingMovingVec", m_bIsUsingMovingVec, allocator);

	//// ���࿡ MovingVec�� ����ϰ� �־��ٸ� �̿� ���� ������ ��������� �Ѵ�.
	//if (m_bIsUsingMovingVec)
	//{
	//	Value vMovingVecArray(kArrayType);
	//	for (int i = 0; i < m_vMovingVec.size(); ++i)
	//	{
	//		// Struct MovingStruct
	//		Value MovingStructValue(kObjectType);

	//		// ��������
	//		string key = "m_vMovingVec[" + std::to_string(i) + "]";
	//		Value keyName(kStringType);
	//		keyName.SetString(key.c_str(), key.length(), allocator);
	//		MovingStructValue.AddMember(keyName, Value(kNullType), allocator);

	//		MovingStructValue.AddMember("TargetTex", (UINT)m_vMovingVec[i].TargetTex, allocator);
	//		MovingStructValue.AddMember("MovingStyle", (UINT)m_vMovingVec[i].MovingStyle, allocator);
	//		MovingStructValue.AddMember("FuncValue", SaveVec4Json(m_vMovingVec[i].FuncValue,allocator), allocator);
	//		MovingStructValue.AddMember("PreviousPos", SaveVec2Json(m_vMovingVec[i].PreviousPos, allocator), allocator);

	//		vMovingVecArray.PushBack(MovingStructValue, allocator);
	//	}
	//	_objValue.AddMember("vMovingVec", vMovingVecArray, allocator);
	//}
}

void CMeshRender::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CRenderComponent::LoadFromLevelJsonFile(_componentValue);

	/*
	m_bIsUsingMovingVec = _componentValue["bIsUsingMovingVec"].GetBool();
	
	if (m_bIsUsingMovingVec)
	{
		SetUsingMovingVec(true);
		int a = m_vMovingVec.size();
		a;
		const Value& vMovingVecArray = _componentValue["vMovingVec"];
		for (int i = 0; i < vMovingVecArray.Size(); ++i)
		{
			MovingStruct newStruct = {};
			newStruct.TargetTex = (eTargetTexture)vMovingVecArray[i]["TargetTex"].GetUint();
			newStruct.MovingStyle = (eTexMovingStyle)vMovingVecArray[i]["MovingStyle"].GetUint();
			newStruct.FuncValue = LoadVec4Json(vMovingVecArray[i]["FuncValue"]);
			newStruct.PreviousPos = LoadVec2Json(vMovingVecArray[i]["PreviousPos"]);
	
			m_vMovingVec.push_back(newStruct);
		}
	}
	else
	{
		SetUsingMovingVec(false);
	}*/
}
