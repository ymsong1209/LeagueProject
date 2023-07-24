#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>

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
	
	//에디터 상에서(play모드가아닐때) 에디터 카메라 이동하면 메인카메라도 같이 움직여버리는 현상때문에 아래 코드도 추가합니다.
	//나중에 카메라 스크립트를 에디터랑, 메인카메라 따로 만들어야할것같아요! 인게임 카메라랑 저희가 편집할때 카메라랑 많이달라서..
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() != LEVEL_STATE::PLAY &&
		CRenderMgr::GetInst()->GetMainCam() != this->GetOwner()->Camera()) {
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

	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
	{
		if (KEY_PRESSED(KEY::LSHIFT))
			fSpeed *= 5.f;

		if (KEY_PRESSED(KEY::W))
		{
			vPos.z += EditorDT * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.z -= EditorDT  * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= EditorDT * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += EditorDT * fSpeed;
		}
	}

	else
	{
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
