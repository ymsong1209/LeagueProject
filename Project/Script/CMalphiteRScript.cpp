#include "pch.h"
#include "CMalphiteRScript.h"
#include "CProjectileScript.h"

CMalphiteRScript::CMalphiteRScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::MALPHITERSCRIPT)
{
	m_fProjectileSpeed = 300.f;
	m_fSkillRange = 150.f;
}

CMalphiteRScript::~CMalphiteRScript()
{
}

void CMalphiteRScript::begin()
{
}

void CMalphiteRScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	m_fTime += DT;
	if (m_fTime > 0.1f) {

		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
		m_bUnitDead = true;
	}

}

void CMalphiteRScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_bUnitDead) return;

	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;

	// �����ڿ� �ٸ� ������ ������Ʈ�� �ε�ģ�ٸ�
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// �ǰ����� ���� ���̵�
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::MALPHITE_R);
	}

	// sound
	Vec3 _2dPos = Vec3(0.f, 0.f, 0.f);
	CSound* newSound = new CSound;
	wstring filepath = CPathMgr::GetInst()->GetContentPath();
	filepath += L"sound3d\\malphite\\Malph_R.mp3";
	newSound->Load(filepath);
	CSoundMgr::GetInst()->AddSound(newSound);
	int soundId = newSound->GetSoundIndex();
	CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, _2dPos);
	CSoundMgr::GetInst()->Stop(soundId);
	CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, _2dPos);
	//CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\malphite\\Malph_R.mp3", 1, 0.5f, true, 100.f, TargetUnitScript->GetOwner()->Transform()->GetRelativePos(), Faction::END);
	
}
