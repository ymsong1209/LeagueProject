#include "pch.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"


#include "CTransform.h"
#include "CAnimator2D.h"
 
#include "CResMgr.h"

 
 
#include "CAnimator3D.h"
 

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)	
	, m_bIsUsingMovingVec(false)
{


}

void CMeshRender::SetUsingMovingVec(bool _use)
{
	m_bIsUsingMovingVec = _use;


	// 처음 MovingVec을 만들어야 하는 경우
	if (_use == true && m_vMovingVec.size() == 0)
	{
		// 움직일 여지가 있는 Texture들 전부 Vector안에 집어넣어줌.
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
	// vector에 있는 Texture들에 대해서
	// 어떤 움직임을 취하고 있는지 찾아서
	// 참조해야할 UV Offset 값을 계산해준다.
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
			// FuncValue.y : x 계수
			// FuncValue.z : y 절편
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * PreviousPos.x + FuncValue.z;
		}
			break;
		case eTexMovingStyle::PARABOLA:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : x^2 계수
			// FuncValye.z : x 계수
			// FuncValue.w : y 절편
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * PreviousPos.x * PreviousPos.x + FuncValue.z * PreviousPos.x + FuncValue.w;
		}
			break;
		case eTexMovingStyle::SIN:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : sin 계수
			// FuncValue.z : 주파수
			// FuncValue.w : y절편
			PreviousPos.x += GlobalData.tEditDT * FuncValue.x;
			PreviousPos.y = FuncValue.y * sin(FuncValue.z * PreviousPos.x) + FuncValue.w;
		}
			break;
		case eTexMovingStyle::COS:
		{
			// FuncValue.x : dx / dt
			// FuncValue.y : cos 계수
			// FuncValue.z : 주파수
			// FuncValue.w : y절편
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
			GetMaterial(0)->SetScalarParam(VEC2_2, &PreviousPos);
		}
			break;
		case eTargetTexture::PUNCTURE:
		{
			GetMaterial(0)->SetScalarParam(VEC2_3, &PreviousPos);
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
	// Texture가 Moving을 Option을 쓰는지 전달 (int_1 에 지정되어 있음 (Std2dMtrl기준))

	int a = MovingUse;

	if (GetMaterial(0) != nullptr)
	{
		GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &MovingUse);
	}


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

	// Animator3D 업데이트
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (UINT i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}


	// 렌더
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// 사용할 재질 업데이트
			GetMaterial(i)->UpdateData();

			// 사용할 메쉬 업데이트 및 렌더링
			GetMesh()->render(i);
		}
	}

	// 선생님의 코드로 기존의 것 대체 (note1 start)
	//for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i) {
	//	//재질 업데이트
	//	GetMaterial(i)->UpdateData();

	//	//렌더
	//	GetMesh()->render(i);
	//}
	// (note1 end)


	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::SaveToLevelFile(FILE* _File)
{	
	CRenderComponent::SaveToLevelFile(_File);

	
	fwrite(&m_bIsUsingMovingVec, sizeof(bool), 1, _File);

	// 만약에 MovingVec을 사용하고 있었다면 이에 대한 정보도 저장해줘야 하낟.
	if (m_bIsUsingMovingVec)
	{
		for (int i = 0; i < m_vMovingVec.size(); ++i)
		{
			fwrite(&m_vMovingVec[i], sizeof(MovingStruct), 1, _File);
		}
	}

	 
}

void CMeshRender::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_bIsUsingMovingVec, sizeof(bool), 1, _File);

	if (m_bIsUsingMovingVec)
	{
		SetUsingMovingVec(true);

		for (int i = 0; i < m_vMovingVec.size(); ++i)
		{
			fread(&m_vMovingVec[i], sizeof(MovingStruct), 1, _File);
		}		
	}
	else
	{
		SetUsingMovingVec(false);
	}
	 
}

void CMeshRender::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CRenderComponent::SaveToLevelJsonFile(_objValue, allocator);

	_objValue.AddMember("bIsUsingMovingVec", m_bIsUsingMovingVec, allocator);

	// 만약에 MovingVec을 사용하고 있었다면 이에 대한 정보도 저장해줘야 한다.
	if (m_bIsUsingMovingVec)
	{
		Value vMovingVecArray(kArrayType);
		for (int i = 0; i < m_vMovingVec.size(); ++i)
		{
			// Struct MovingStruct
			Value MovingStructValue(kObjectType);

			// 가독성용
			string key = "m_vMovingVec[" + std::to_string(i) + "]";
			Value keyName(kStringType);
			keyName.SetString(key.c_str(), key.length(), allocator);
			MovingStructValue.AddMember(keyName, Value(kNullType), allocator);

			MovingStructValue.AddMember("TargetTex", (UINT)m_vMovingVec[i].TargetTex, allocator);
			MovingStructValue.AddMember("MovingStyle", (UINT)m_vMovingVec[i].MovingStyle, allocator);
			MovingStructValue.AddMember("FuncValue", SaveVec4Json(m_vMovingVec[i].FuncValue,allocator), allocator);
			MovingStructValue.AddMember("PreviousPos", SaveVec2Json(m_vMovingVec[i].PreviousPos, allocator), allocator);

			vMovingVecArray.PushBack(MovingStructValue, allocator);
		}
		_objValue.AddMember("vMovingVec", vMovingVecArray, allocator);
	}

}

void CMeshRender::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CRenderComponent::LoadFromLevelJsonFile(_componentValue);

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
	}
}
