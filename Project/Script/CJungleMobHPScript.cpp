#include "pch.h"
#include "CJungleMobHPScript.h"
#include "CUnitScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>

CJungleMobHPScript::CJungleMobHPScript()
	:CScript((UINT)SCRIPT_TYPE::JUNGLEMOBHPSCRIPT)
{
}

CJungleMobHPScript::~CJungleMobHPScript()
{
}

void CJungleMobHPScript::begin()
{
	GetOwner()->ChangeLayer(31);
	Transform()->SetNoParentaffected(true);
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\JungleMobBar.mtrl"), 0);

	if (GetOwner()->GetParent() && !GetOwner()->GetParent()->IsDead())
	{
		CGameObject* ParentObj = GetOwner()->GetParent();
		CUnitScript* UnitScript = ParentObj->GetScript<CUnitScript>();
		UnitType UnitType = UnitScript->GetUnitType();
		if (UnitType == UnitType::NORTH_GROMP || UnitType == UnitType::SOUTH_GROMP)
		{
			OffsetBarPos = Vec2(0.f, 110.f);
			OffsetFontPos = Vec2(0.f, -135.3f);
		}
		else if (UnitType == UnitType::NORTH_MURKWOLF || UnitType == UnitType::SOUTH_MURKWOLF)
		{
			OffsetBarPos = Vec2(0.f, 85.f);
			OffsetFontPos = Vec2(0.f, -111.3f);
		}
		else if(UnitType == UnitType::NORTH_KRUG || UnitType == UnitType::SOUTH_KRUG)
		{
			OffsetBarPos = Vec2(0.f, 110.f);
			OffsetFontPos = Vec2(0.f, -135.3f);
		}
		else
		{
			OffsetBarPos = Vec2(0.f, 110.f);
			OffsetFontPos = Vec2(0.f, -135.3f);
		}
	}
}

void CJungleMobHPScript::tick()
{
	if (GetOwner()->GetParent() && !GetOwner()->GetParent()->IsDead())
	{
		CGameObject* ParentObj = GetOwner()->GetParent();
		bool IsCulling = ParentObj->GetRenderComponent()->IsCulled();
		bool IsRaySightCulling = ParentObj->GetRenderComponent()->IsUsingRaySightCulling();
		if (IsCulling && IsRaySightCulling)
		{
			GetOwner()->GetRenderComponent()->SetSortExcept(true);
			return;
		}

		CUnitScript* UnitScript = ParentObj->GetScript<CUnitScript>();
		UnitType UnitType = UnitScript->GetUnitType();

		m_fCurHP = ParentObj->GetScript<CUnitScript>()->GetCurHP();
		m_fTotalHP = ParentObj->GetScript<CUnitScript>()->GetMaxHP();

		Transform()->SetAbsolute(false);
		Transform()->SetRelativeScale(Vec3(157.04f, 43.12f, 1.f));
		//Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));

		//----------HP------------
		//======디버깅용=========
		//m_fCurHP = 570.f;
		//m_fTotalHP = 1000.f;
		//====================
		m_fRatio = m_fCurHP / m_fTotalHP;
		if (m_fCurHP >= m_fTotalHP)
			m_fCurHP = m_fTotalHP;

		if (m_fCurHP <= 0.f)
			m_fCurHP = 0.f;
		//-------------------------
		Vec3 Pos = ParentObj->Transform()->GetRelativePos();
		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		Matrix viewmat = MainCam->GetViewMat();
		Matrix projmat = MainCam->GetProjMat();
		Matrix MatViewProj = viewmat * projmat;

		// 4D 벡터로 변환 (w 요소를 1로 설정)
		Vec4 Pos4 = Vec4(Pos.x, Pos.y, Pos.z, 1.0f);
		// MatViewProj에 곱해줌
		Vec4 ProjPos = Pos4.Transform(Pos4, MatViewProj);
		// w로 나눠서 클리핑(ndc) 좌표계로 변환
		ProjPos /= ProjPos.w;
		Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();  //화면 해상도를 가져옴
		Vec2 ObjscreenPos = ((Vec2(ProjPos.x, ProjPos.y) + Vec2(1.f, 1.f)) / Vec2(2.f, 2.f)) * Resolution;

		Vec4 ndcVec = Vec4((2.0f * ObjscreenPos.x) / Resolution.x - 1.0f, 1.0f - (2.0f * ObjscreenPos.y) / Resolution.y, 1.f, 1.f);

		Matrix viewInvMatrix = UICamera->GetViewMatInv();
		Matrix projInvMatrix = UICamera->GetProjMatInv();
		Matrix invViewProjMatrix = viewInvMatrix * projInvMatrix;

		Vec3 worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);
		Vec3 FinalPos = Vec3(worldVec.x + OffsetBarPos.x, -worldVec.y + OffsetBarPos.y, 700.f);
		Transform()->SetRelativePos(FinalPos);

		//==========닉네임, 레벨 폰트 출력==============
		Vec2 FontDefaultPos = Vec2(worldVec.x + (Resolution.x / 2), worldVec.y + (Resolution.y / 2));

		tFont Font2 = {};
		Font2.wInputText = to_wstring((int)m_fCurHP); // 원래 여기에 닉네임 가져와야함
		Font2.fontType = FONT_TYPE::RIX_KOR_L;
		Font2.fFontSize = 13.5;
		Font2.vDisplayPos = Vec2(FontDefaultPos.x + OffsetFontPos.x, FontDefaultPos.y + OffsetFontPos.y);
		Font2.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::OPAQE, Font2);

		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fRatio);
	}
}

void CJungleMobHPScript::BeginOverlap(CCollider2D* _Other)
{
}

