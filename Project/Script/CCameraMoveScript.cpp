#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSound.h>
#include <Engine/CResMgr.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_fCamSpeed(100.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{

	// Editor Camera에서의 Play 상태에서의 Tick을 주지 않기 위함 
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY &&
		CRenderMgr::GetInst()->GetMainCam() != this->GetOwner()->Camera()) {
		return;
	}

	Vec3 CamPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 CamForward = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 CamUp = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP);
	CSound::UpdateListenerAttributes(CamPos, CamForward, CamUp);
	

	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		Camera2DMove();
	else
		Camera3DMove();
}

void CCameraMoveScript::Camera2DMove()
{
	// 키 입력에 따른 카메라 이동
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fCamSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos.z += EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.z -= EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += EditorDT * fSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::Camera3DMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = m_fCamSpeed;




	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos += EditorDT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos -= EditorDT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos -= EditorDT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += EditorDT * vRight * fSpeed;
	}



	if (KEY_PRESSED(KEY::MBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += EditorDT * vMouseDir.x * 5.f;
		vRot.x -= EditorDT * vMouseDir.y * 5.f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);

}
