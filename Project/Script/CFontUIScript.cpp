#include "pch.h"
#include "CFontUIScript.h"
#include <Engine\CFontMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

void CFontUIScript::begin()
{
}

void CFontUIScript::tick()
{
	CCamera* UICam = CRenderMgr::GetInst()->GetCamerafromIdx(1);
	if (CUR_LEVEL->GetState() == LEVEL_STATE::PLAY && UICam)
	{
		//tFont Font = {};
		//Font.wInputText = L"¿ì¸®ÆÀ È­ÀÌ¶ìÀÌÀ×~!";
		//Font.fontType = FONT_TYPE::RIX_KOR_M;
		//Font.fFontSize = 18;
		//Font.vDisplayPos = Vec2(0, 52);
		//Font.iFontColor = FONT_RGBA(245, 242, 228, 255);
		//UICam->AddText(FONT_DOMAIN::TRANS, Font);
	}
}

void CFontUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CFontUIScript::CFontUIScript()
	:CScript((UINT)SCRIPT_TYPE::FONTUISCRIPT)
{

}

CFontUIScript::~CFontUIScript()
{
}
