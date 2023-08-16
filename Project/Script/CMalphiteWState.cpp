#include "pch.h"
#include "CMalphiteWState.h"

#include "CMalphiteScript.h"
#include "CSkill.h"

CMalphiteWState::CMalphiteWState()
{
	SetName(L"W");
}

CMalphiteWState::~CMalphiteWState()
{
}

void CMalphiteWState::tick()
{
	CChampionSkillState::tick();
}

void CMalphiteWState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* MalphiteW = GetOwner()->GetScript<CChampionScript>()->GetSkill(2);
	MalphiteW->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());
	MalphiteW->Use();

	wstring animName = L"Malphite\\Spell1";
	GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, true, 0.1f);

	//Ptr<CTexture> glowtex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\malphite_glow.dds");
	//if (glowtex == nullptr) assert(nullptr);
	//GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, glowtex);
	//CSendServerEventMgr::GetInst()->SendSetTexParamPacket(targetId, 0, TEX_0, L"texture\\FBXTexture\\malphite_glow.dds");
	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MAT_malphite_glow.mtrl"), 0);
	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(targetId, 0, L"material\\MAT_malphite_glow.mtrl");

	CMalphiteScript* malphscript = GetOwner()->GetScript<CMalphiteScript>();
	malphscript->SetAttackPower(20.f);
	

	//재질을 바꿉니다.

	// 사운드
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\malphite\\Malph_W_Active.mp3", 1, 0.5f, true, 100.f, vPos, Faction::NONE);

}

void CMalphiteWState::Exit()
{
	CChampionSkillState::Exit();
}

void CMalphiteWState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
