#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
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
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY) {
		return;
	}

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
		vPos.y += EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= EditorDT * fSpeed;
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



	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += EditorDT * vMouseDir.x * 5.f;
		vRot.x -= EditorDT * vMouseDir.y * 5.f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);

}
