#include "pch.h"
#include "CKillLogUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include "CUnitScript.h"
#include "CAnnounceTimerUIScript.h"




CKillLogUIScript::CKillLogUIScript()
	:CScript((UINT)SCRIPT_TYPE::KILLLOGUISCRIPT)
{
}

CKillLogUIScript::~CKillLogUIScript()
{
}


void CKillLogUIScript::begin()
{

}

void CKillLogUIScript::tick()
{
	killLogManager.displayTime = 3.f;
	killLogManager.shiftSpeed = 300.f;
	killLogManager.lineHeight = 75.f;

	//if (KEY_TAP(KEY::_7))
	//	AddAnnouncement(AnnounceType::INHIBITOR_DESTROY, Faction::RED,ChampionType::JINX,ChampionType::MALPHITE);

	////�Ʊ��� ������ óġ���Ѱ��
	//else if (KEY_TAP(KEY::_8))
	//	AddAnnouncement(AnnounceType::INHIBITOR_DESTROY, Faction::BLUE, ChampionType::MALPHITE);

	////�Ʊ��̳�, ���� ���� óġ�Ѱ��
	//else if (KEY_TAP(KEY::_9))
	//	AddAnnouncement(AnnounceType::KILLEDENEMY, Faction::BLUE, ChampionType::JINX, ChampionType::MALPHITE);


	vector<tServerEvent> vServerEvent = CSendServerEventMgr::GetInst()->GetUIEvent();
	for (size_t i = 0; i < vServerEvent.size(); ++i)
	{
		if (vServerEvent[i].Type == SERVER_EVENT_TYPE::Kill_LOG_PACKET)
		{
			CGameObject* KillerObj = (CGameObject*)vServerEvent[i].wParam;
			CGameObject* VictimObj = (CGameObject*)vServerEvent[i].lParam;
			if (KillerObj && VictimObj)
			{
				CUnitScript* KillerUnitScript = KillerObj->GetScript<CUnitScript>();
				CUnitScript* VictimUnitScript = VictimObj->GetScript<CUnitScript>();
				UnitType KillerType = KillerUnitScript->GetType();
				UnitType VictimType = VictimUnitScript->GetType();

				//===ū���� Ÿ��====
				SpecificType KillerSpecificType = GetSpecificType(KillerType);
				SpecificType VictimSpecificType = GetSpecificType(VictimType);


				if (KillerSpecificType == SpecificType::CHAMPION && VictimSpecificType == SpecificType::CHAMPION)
				{
					ChampionType KillerChamp = KillerUnitScript->GetChampType();
					Faction KillerFaction = KillerUnitScript->GetFaction();
					ChampionType VictimChamp = VictimObj->GetScript<CUnitScript>()->GetChampType();

					//������ ų�α� ���
					killLogManager.AddKillLog(KillerChamp, VictimChamp, KillerFaction);

					CGameObject* MyPlayer = CSendServerEventMgr::GetInst()->GetMyPlayer();
					CUnitScript* MyPlayerUnitScript = MyPlayer->GetScript<CUnitScript>();

					//���� �Ʊ����� óġ���Ѱ��
					if (VictimObj == MyPlayer)
						AddAnnouncement(AnnounceType::GOTKILLED, KillerFaction, KillerChamp, VictimChamp);

					//�Ʊ��� ������ óġ���Ѱ��
					else if (VictimUnitScript->GetFaction() == MyPlayerUnitScript->GetFaction())
						AddAnnouncement(AnnounceType::ALLY_HASBEENSLAIN, KillerFaction, KillerChamp, VictimChamp);

					//�Ʊ��̳�, ���� ���� óġ�Ѱ��
					else if (KillerUnitScript->GetFaction() == MyPlayerUnitScript->GetFaction())
						AddAnnouncement(AnnounceType::KILLEDENEMY, KillerFaction, KillerChamp, VictimChamp);

					else
					{
					}

				}

				else if (KillerSpecificType == SpecificType::CHAMPION && VictimSpecificType == SpecificType::AllTURRET)
				{
					Faction KillerFaction = KillerUnitScript->GetFaction();
					ChampionType KillerChamp = KillerUnitScript->GetChampType();

					if(VictimType == UnitType::TURRET)
						AddAnnouncement(AnnounceType::TURRET_DESTROY, KillerFaction, KillerChamp);
					else if(VictimType == UnitType::INHIBITOR)
						AddAnnouncement(AnnounceType::INHIBITOR_DESTROY, KillerFaction, KillerChamp);
				}

				else if (KillerSpecificType == SpecificType::MINION && VictimSpecificType == SpecificType::AllTURRET)
				{
					Faction KillerFaction = KillerUnitScript->GetFaction();

					if (VictimType == UnitType::TURRET)
						AddAnnouncement(AnnounceType::TURRET_DESTROY, KillerFaction);
					else if(VictimType == UnitType::INHIBITOR)
						AddAnnouncement(AnnounceType::INHIBITOR_DESTROY, KillerFaction);
				}


			}
		}
	}

	CSendServerEventMgr::GetInst()->ClearUISendEvent();


	// ų�α� ������Ʈ
	killLogManager.Update(DT);
	AnnounceLogUpdate(DT);

	// UI �������� ���� �ڵ�
	for (const auto& log : killLogManager.GetKillLogs())
	{
		log.KillLogObj->Transform()->SetRelativePos(log.position);
	}

	//ó�� �Ϸ� �� ������ ų�α� ���Ͱ� Ŭ����
	//CSendServerEventMgr::GetInst()->ClearUISendEvent();

	 //������==========================
	if (KEY_TAP(KEY::I))
		killLogManager.AddKillLog(ChampionType::MALPHITE, ChampionType::JINX, Faction::RED);
	//=================================

}

void CKillLogUIScript::BeginOverlap(CCollider2D* _Other)
{

}

SpecificType CKillLogUIScript::GetSpecificType(UnitType type)
{
	if (type == UnitType::CHAMPION)
		return SpecificType::CHAMPION;
	if (type >= UnitType::SOUTH_GROMP && type <= UnitType::NORTH_BLUE)
		return SpecificType::JUNGLE_MOB;
	if (type == UnitType::TURRET || type == UnitType::INHIBITOR || type == UnitType::NEXUS)
		return SpecificType::AllTURRET;
	if (type >= UnitType::MELEE_MINION && type <= UnitType::SUPER_MINION)
		return SpecificType::MINION;
	return SpecificType::Other;
}

void CKillLogUIScript::DisplayAnnounce(ChampionType _Killer, ChampionType _Victim, AnnounceType _Type)
{
	CGameObject* AnnouncePanel = SpawnAnnouncePanel(Vec3(645.7f, 123.2f, 1.f), L"AnnounceObj");

	switch (_Type)
	{
	case (AnnounceType::ALLY_HASBEENSLAIN):
		AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\allyhasbeenslain.mtrl"), 0);
		break;
	case (AnnounceType::GOTKILLED):
		AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PlayerDeathPanel.mtrl"), 0);
		break;
	case (AnnounceType::KILLEDENEMY):
		AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\killedenemy.mtrl"), 0);
		break;
	}
	AnnouncePlayerIcon(AnnouncePanel, _Killer, _Victim, _Type);
	SpawnGameObject(AnnouncePanel, Vec3(22.f, 315.f, 150.f), 31);
}

void CKillLogUIScript::AnnouncePlayerIcon(CGameObject* _Parent, ChampionType _Killer, ChampionType _Victim, AnnounceType _Type)
{

	Vec3 IconScale = Vec3(52.f, 52.f, 1.f);
	CGameObject* Killer_AnnouncePanel = SpawnIconPanel(IconScale, L"Killer_AnnouncePlayerIcon");
	IconSetting(Killer_AnnouncePanel, _Killer);
	_Parent->AddChild(Killer_AnnouncePanel);


	CGameObject* Victim_AnnouncePanel = SpawnIconPanel(IconScale, L"Victimc_AnnouncePlayerIcon");
	IconSetting(Victim_AnnouncePanel, _Victim);
	_Parent->AddChild(Victim_AnnouncePanel);

	switch (_Type)
	{
	case (AnnounceType::ALLY_HASBEENSLAIN):
	{
		Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(175.f, 5.f, 200.f));
		Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-165.f, 5.f, 200.f));
	}
	break;
	case (AnnounceType::GOTKILLED):
	{
		Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(143.f, 6.f, 200.f));
		Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-131.f, 5.f, 200.f));

	}
	break;
	case (AnnounceType::KILLEDENEMY):
	{
		Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(147.f, 5.f, 200.f));
		Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-146.f, 5.f, 200.f));
	}
	break;
	}


}

void CKillLogUIScript::IconSetting(CGameObject* _Obj, ChampionType ChampType)
{
	switch (ChampType)
	{
	case (ChampionType::JINX):
		_Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\JINX_CIRCLE.mtrl"), 0);
		break;
	case (ChampionType::MALPHITE):
		_Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MALPHITE_CIRCLE.mtrl"), 0);
		break;
	}
}




//========ų�α� �Ŵ��� (�̱��� �ƴ�!)=========================================
void KillLogManager::SpawnChampRect(ChampionType _Killer, CGameObject* _Parent, Vec3 _Pos)
{
	CGameObject* ChampRect = new CGameObject;
	ChampRect->SetName(L"KillLogRect");
	ChampRect->AddComponent(new CTransform);
	ChampRect->AddComponent(new CMeshRender);
	ChampRect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ChampRect->Transform()->SetAbsolute(true);
	ChampRect->Transform()->SetRelativeScale(Vec3(55.f, 55.f, 1.f));
	ChampRect->Transform()->SetRelativePos(_Pos);

	switch (_Killer)
	{
	case (ChampionType::JINX):
	{
		ChampRect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\JINX_Rect.mtrl"), 0);
		_Parent->AddChild(ChampRect);
	}
	break;
	case (ChampionType::MALPHITE):
	{
		ChampRect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\malphite_Rect.mtrl"), 0);
		_Parent->AddChild(ChampRect);
	}
	break;
	}

}

void KillLogManager::AddKillLog(ChampionType _killer, ChampionType _victim, Faction _Faction)
{
	KillLog log;
	log.killer = _killer;
	log.victim = _victim;
	log.remainingTime = displayTime;
	log.initialX = initialPosition.x - 100; // ���ʿ��� ���� (100�� ���ϴ� �Ÿ��� ���� ����)
	log.position.x = log.initialX; // X ��ǥ ����

	CGameObject* SpawnKillLogObj = new CGameObject;
	SpawnKillLogObj->SetName(L"KillLogObj");
	SpawnKillLogObj->AddComponent(new CTransform);
	SpawnKillLogObj->AddComponent(new CMeshRender);
	SpawnKillLogObj->Transform()->SetRelativeScale(Vec3(194.f, 71.f, 1.f));
	SpawnKillLogObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	if (_Faction == Faction::BLUE)
		SpawnKillLogObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueKillLog.mtrl"), 0);
	else if (_Faction == Faction::RED)
		SpawnKillLogObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedKillLog.mtrl"), 0);
	SpawnKillLogObj->Transform()->SetAbsolute(true);

	SpawnChampRect(log.killer, SpawnKillLogObj, Vec3(-37.f, 0.f, -20.f));
	SpawnChampRect(log.victim, SpawnKillLogObj, Vec3(60.f, 0.f, -14.f));

	log.KillLogObj = SpawnKillLogObj;

	if (killLogs.empty())
	{
		log.position.y = initialPosition.y; // Y ��ǥ�� �ʱ� ��ġ�� ����
		log.targetY = initialPosition.y; // Target position ����
	}
	else
	{
		log.position.y = initialPosition.y + lineHeight * killLogs.size(); // ���� ų�α��� ��ǥ ��ġ���� lineHeight��ŭ ���� ��ġ
		log.targetY = log.position.y; // Target position ����
	}
	log.position.z = initialPosition.z; // Z ��ǥ �ʱ� ��ġ ����

	killLogs.push_back(log); // ���ʿ� �߰�
	needToShift = true;
	UpdateTargetPositions();
	SpawnGameObject(SpawnKillLogObj, Vec3(log.position.x, log.position.y, 150.f), 31);
}



void CKillLogUIScript::AnnounceLogUpdate(float deltaTime)
{
	if (displayTimer > 0) {
		displayTimer -= deltaTime;
	}
	else if (!announcementsQueue.empty())
	{
		AnnounceMessage nextMessage = announcementsQueue.front();
		announcementsQueue.pop();
		DisplayAnnounceAll(nextMessage.type, nextMessage.KillerFaction, nextMessage.KillerChampType, nextMessage.VictimChampType, nextMessage.KillerUnitType, nextMessage.VictimUnitType);
		displayTimer = displayDuration;
	}
}



void CKillLogUIScript::DisplayAnnounceAll(AnnounceType type, Faction _KillerFaction, ChampionType _KillerChamp, ChampionType _VictimChamp, UnitType _KillerUnitType, UnitType _VictimUnitType)
{
	if (type <= AnnounceType::ALLY_HASBEENSLAIN)
		DisplayAnnounce(_KillerChamp, _VictimChamp, type);
	else
	{
		DisplayAnnounceTurret(_KillerChamp, _KillerFaction, type);
	}
}




void CKillLogUIScript::DisplayAnnounceTurret(ChampionType _Killer, Faction _KillerFaction, AnnounceType _Type) //�̴Ͼ��� óġ�ߴٸ� ChampionType �� NONE���� ����
{
	CGameObject* AnnouncePanel = SpawnAnnouncePanel(Vec3(645.7f, 123.2f, 1.f), L"AnnounceObj");
	Vec3 IconScale = Vec3(52.f, 52.f, 1.f);
	CGameObject* Killer_AnnouncePanel = SpawnIconPanel(IconScale, L"Killer_AnnouncePlayerIcon");
	AnnouncePanel->AddChild(Killer_AnnouncePanel);
	CGameObject* Victim_AnnouncePanel = SpawnIconPanel(IconScale, L"Victimc_AnnouncePlayerIcon");
	AnnouncePanel->AddChild(Victim_AnnouncePanel);


	if (_KillerFaction == Faction::BLUE) 
	{

		if (_Type == AnnounceType::TURRET_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_red_circle.mtrl"), 0);
			AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Redturretisdestroyed.mtrl"), 0);
			Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-216.f, 7.f, 200.f));
			Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(219.f, 4.f, 200.f));

		}
		else if (_Type == AnnounceType::INHIBITOR_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_circle.mtrl"), 0);

			if (_KillerFaction == CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>()->GetFaction()) //�����⸦ �ı��߽��ϴ�
			{
				AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\InhibitorKill.mtrl"), 0);
				Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-171.f, 6.f, 200.f));
				Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(172.f, 5.72f, 200.f));

			}
			else // �����Ⱑ �ı��Ǿ����ϴ�
			{
				AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\InhibitorDeath.mtrl"), 0);
				Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-178.f, 4.f, 200.f));
				Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(180.f, 6.f, 200.f));
			}
		}

		if (_Killer == ChampionType::NONE) // �̴Ͼ��� ���ΰ��
			Killer_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\bluemelee_circle.mtrl"), 0);
		else
			IconSetting(Killer_AnnouncePanel, _Killer);
	}


	else if (_KillerFaction == Faction::RED)
	{
		if (_Type == AnnounceType::TURRET_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_blue_circle.mtrl"), 0);
			AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turretisdestroyed.mtrl"), 0);
			Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-216.f, 7.f, 200.f));
			Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(219.f, 4.f, 200.f));
		}
		else if (_Type == AnnounceType::INHIBITOR_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_circle.mtrl"), 0);

			if (_KillerFaction == CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>()->GetFaction())
			{
				AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\InhibitorKill.mtrl"), 0);
				Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-171.f, 6.f, 200.f));
				Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(172.f, 5.72f, 200.f));
			}
			else
			{
				AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\InhibitorDeath.mtrl"), 0);
				Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-178.f, 4.f, 200.f));
				Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(180.f, 6.f, 200.f));
			}
		}

		if (_Killer == ChampionType::NONE) // �̴Ͼ��� ���ΰ��
			Killer_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\redmelee_circle.mtrl"), 0);
		else
			IconSetting(Killer_AnnouncePanel, _Killer);
	}

	SpawnGameObject(AnnouncePanel, Vec3(22.f, 315.f, 150.f), 31);
}





CGameObject* CKillLogUIScript::SpawnAnnouncePanel(Vec3 _Scale, wstring _Name)
{
	CGameObject* AnnouncePanel = new CGameObject;
	AnnouncePanel->SetName(_Name);
	AnnouncePanel->AddComponent(new CTransform);
	AnnouncePanel->AddComponent(new CMeshRender);
	AnnouncePanel->AddComponent(new CAnnounceTimerUIScript);
	AnnouncePanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	AnnouncePanel->Transform()->SetAbsolute(true);
	AnnouncePanel->Transform()->SetRelativeScale(_Scale);
	return AnnouncePanel;
}


CGameObject* CKillLogUIScript::SpawnIconPanel(Vec3 _Scale, wstring _Name)
{
	CGameObject* AnnouncePanel = new CGameObject;
	AnnouncePanel->SetName(_Name);
	AnnouncePanel->AddComponent(new CTransform);
	AnnouncePanel->AddComponent(new CMeshRender);
	AnnouncePanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	AnnouncePanel->Transform()->SetAbsolute(true);
	AnnouncePanel->Transform()->SetRelativeScale(_Scale);
	return AnnouncePanel;
}