#include "pch.h"
#include "CKillLogUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include "CUnitScript.h"
#include "CAnnounceTimerUIScript.h"
#include <Engine\CEventMgr.h>
#include "CPlayerCSUIScript.h"
#include "CIconTimerUIScript.h"
#include "CEndOfGameUIScript.h"
#include <thread>

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

	//==============디버깅용=========================
	/*
	if (KEY_TAP(KEY::_7))
	{
		CEventMgr::GetInst()->SetIsGameOver(true);
		Announce_EndofGame(Faction::BLUE);
	}

	if (KEY_TAP(KEY::_8))
	{
		CEventMgr::GetInst()->SetIsGameOver(true);
		Announce_EndofGame(Faction::RED);
	}
	*/

	//===============================디버깅용====================================
	//if (KEY_TAP(KEY::_8))
	//	DisplayCS(CSendServerEventMgr::GetInst()->GetMyPlayer(), UnitType::MELEE_MINION);

	//////아군이나, 내가 적을 처치한경우
	//else if (KEY_TAP(KEY::_9))
	//	AddAnnouncement(AnnounceType::KILLEDENEMY, Faction::BLUE, ChampionType::JINX, ChampionType::MALPHITE);

	//else if (KEY_TAP(KEY::_7))
	//	AddAnnouncement(AnnounceType::INHIBITOR_DESTROY, Faction::BLUE);

	//else if (KEY_TAP(KEY::_6))
	//	AddAnnouncement(AnnounceType::ALLY_HASBEENSLAIN, Faction::RED, ChampionType::MALPHITE, ChampionType::MALPHITE);

	//else if (KEY_TAP(KEY::_5))
	//	AddAnnouncement(AnnounceType::TURRET_DESTROY, Faction::BLUE,ChampionType::MALPHITE);

	//else if (KEY_TAP(KEY::_4))
	//	AddAnnouncement(AnnounceType::TURRET_DESTROY, Faction::RED);

	//else if (KEY_TAP(KEY::_3))
	//{
	//	//오른쪽 킬로그 출력
	//	killLogManager.AddKillLog(ChampionType::JINX, ChampionType::MALPHITE, Faction::RED);
	//}

	//else if (KEY_TAP(KEY::_2))
	//{
	//	Announce_EndofGame(Faction::RED);
	//}
	//================================================================================================

	vector<tServerEvent> vServerEvent = CSendServerEventMgr::GetInst()->GetUIEvent();
	for (size_t i = 0; i < vServerEvent.size(); ++i)
	{
		if (vServerEvent[i].Type == SERVER_EVENT_TYPE::Kill_LOG_PACKET)
		{
			CGameObject* KillerObj = (CGameObject*)vServerEvent[i].wParam;
			CGameObject* VictimObj = (CGameObject*)vServerEvent[i].lParam;

			if (VictimObj != nullptr)
			{
				CUnitScript* VictimUnitScript = VictimObj->GetScript<CUnitScript>();
				UnitType VictimType = VictimUnitScript->GetType();
				if (VictimType == UnitType::NEXUS)
				{
					Faction VictimFaction = VictimUnitScript->GetFaction();
					if (CEventMgr::GetInst()->GetIsGameOver() == false) //게임이 끝난상태가 아니었을때 들어오도록
					{
						Announce_EndofGame(VictimFaction);
					}
				}
			}
			

			if (KillerObj && VictimObj)
			{
				CUnitScript* KillerUnitScript = KillerObj->GetScript<CUnitScript>();
				CUnitScript* VictimUnitScript = VictimObj->GetScript<CUnitScript>();
				UnitType KillerType = KillerUnitScript->GetType();
				UnitType VictimType = VictimUnitScript->GetType();

				//===큰범주 타입====
				SpecificType KillerSpecificType = GetSpecificType(KillerType);
				SpecificType VictimSpecificType = GetSpecificType(VictimType);

				if (KillerSpecificType == SpecificType::CHAMPION && VictimSpecificType == SpecificType::CHAMPION)
				{
					ChampionType KillerChamp = KillerUnitScript->GetChampType();
					Faction KillerFaction = KillerUnitScript->GetFaction();
					ChampionType VictimChamp = VictimObj->GetScript<CUnitScript>()->GetChampType();

					//오른쪽 킬로그 출력
					killLogManager.AddKillLog(KillerChamp, VictimChamp, KillerFaction);

					CGameObject* MyPlayer = CSendServerEventMgr::GetInst()->GetMyPlayer();
					CUnitScript* MyPlayerUnitScript = MyPlayer->GetScript<CUnitScript>();

					//내가 아군한테 처치당한경우
					if (VictimObj == MyPlayer)
						AddAnnouncement(AnnounceType::GOTKILLED, KillerFaction, KillerChamp, VictimChamp);

					//아군이 적한테 처치당한경우
					else if (VictimUnitScript->GetFaction() == MyPlayerUnitScript->GetFaction())
						AddAnnouncement(AnnounceType::ALLY_HASBEENSLAIN, KillerFaction, KillerChamp, VictimChamp);

					//아군이나, 내가 적을 처치한경우
					else if (KillerUnitScript->GetFaction() == MyPlayerUnitScript->GetFaction())
						AddAnnouncement(AnnounceType::KILLEDENEMY, KillerFaction, KillerChamp, VictimChamp);

					else
					{

					}

				}

				//cs 획득
				else if (KillerSpecificType == SpecificType::CHAMPION && VictimSpecificType == SpecificType::MINION)
				{
					if(KillerObj == CSendServerEventMgr::GetInst()->GetMyPlayer())
						DisplayCS(KillerObj, VictimType);
				}

				else if (KillerSpecificType == SpecificType::CHAMPION && VictimSpecificType == SpecificType::JUNGLE_MOB)
				{
					if (KillerObj == CSendServerEventMgr::GetInst()->GetMyPlayer())
						DisplayCS(KillerObj, VictimType);
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
	// 킬로그 업데이트
	killLogManager.Update(DT);
	AnnounceLogUpdate(DT);

	if (CEventMgr::GetInst()->GetIsGameOver() == true)
	{
		vector<CGameObject*> UIObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(31)->GetObjects();
		for (size_t i = 0; i < UIObj.size(); ++i)
		{
			if (UIObj[i]->GetRenderComponent())
			{
				if(UIObj[i] != EndOfGameObj) // 승리 패배 오브젝트는 보여줘야함
					UIObj[i]->GetRenderComponent()->SetSortExcept(true);
			}
		}
	}

	// UI 렌더링을 위한 코드
	for (const auto& log : killLogManager.GetKillLogs())
	{
		log.KillLogObj->Transform()->SetRelativePos(log.position);
	}

	//처리 완료 후 서버의 킬로그 벡터값 클리어
	//CSendServerEventMgr::GetInst()->ClearUISendEvent();

	 //디버깅용==========================
	/*if (KEY_TAP(KEY::I))
		killLogManager.AddKillLog(ChampionType::MALPHITE, ChampionType::JINX, Faction::RED);*/
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
		_Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_jinx_circle.mtrl"), 0);
		break;
	case (ChampionType::MALPHITE):
		_Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_malphite_circle.mtrl"), 0);
		break;
	}
}




//========킬로그 매니저 (싱글톤 아님!)=========================================
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
	log.initialX = initialPosition.x - 70; // 왼쪽에서 시작 (100은 원하는 거리에 따라 조절)
	log.position.x = log.initialX; // X 좌표 설정

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
		log.position.y = initialPosition.y; // Y 좌표만 초기 위치로 지정
		log.targetY = initialPosition.y; // Target position 지정
	}
	else
	{
		log.position.y = initialPosition.y + lineHeight * killLogs.size(); // 이전 킬로그의 목표 위치보다 lineHeight만큼 위에 위치
		log.targetY = log.position.y; // Target position 지정
	}
	log.position.z = initialPosition.z; // Z 좌표 초기 위치 지정

	killLogs.push_back(log); // 뒤쪽에 추가
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




void CKillLogUIScript::DisplayAnnounceTurret(ChampionType _Killer, Faction _KillerFaction, AnnounceType _Type) //미니언이 처치했다면 ChampionType 은 NONE으로 들어옴
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
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_turret_red_circle.mtrl"), 0);
			AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Redturretisdestroyed.mtrl"), 0);
			Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-216.f, 7.f, 200.f));
			Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(219.f, 4.f, 200.f));

		}
		else if (_Type == AnnounceType::INHIBITOR_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_inhibitor_red_circle.mtrl"), 0);

			if (_KillerFaction == CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>()->GetFaction()) //억제기를 파괴했습니다
			{
				AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\InhibitorKill.mtrl"), 0);
				Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-171.f, 6.f, 200.f));
				Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(172.f, 5.72f, 200.f));

			}
			else // 억제기가 파괴되었습니다
			{
				AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\InhibitorDeath.mtrl"), 0);
				Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-178.f, 4.f, 200.f));
				Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(180.f, 6.f, 200.f));
			}
		}

		if (_Killer == ChampionType::NONE) // 미니언이 죽인경우
			Killer_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_bluemelee_circle.mtrl"), 0);
		else
			IconSetting(Killer_AnnouncePanel, _Killer);
	}


	else if (_KillerFaction == Faction::RED)
	{
		if (_Type == AnnounceType::TURRET_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_turret_blue_circle.mtrl"), 0);
			AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turretisdestroyed.mtrl"), 0);
			Killer_AnnouncePanel->Transform()->SetRelativePos(Vec3(-216.f, 7.f, 200.f));
			Victim_AnnouncePanel->Transform()->SetRelativePos(Vec3(219.f, 4.f, 200.f));
		}
		else if (_Type == AnnounceType::INHIBITOR_DESTROY)
		{
			Victim_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_inhibitor_blue_circle.mtrl"), 0);

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

		if (_Killer == ChampionType::NONE) // 미니언이 죽인경우
			Killer_AnnouncePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\announce_redmelee_circle.mtrl"), 0);
		else
			IconSetting(Killer_AnnouncePanel, _Killer);
	}

	SpawnGameObject(AnnouncePanel, Vec3(22.f, 315.f, 150.f), 31);
}

void CKillLogUIScript::DisplayCS(CGameObject* _KillerChamp, UnitType _UnitType)
{
	Vec3 Pos = UICamera->GetMainPlayerUICamPos();
	Vec3 PlayerPos = Vec3(Pos.x, Pos.y, 1.f);
	Vec2 OffsetPos = Vec2(0.f, 0.f);

	CGameObject* CSImage = new CGameObject;
	CSImage->SetName(L"CsImage");
	CSImage->AddComponent(new CTransform);
	CSImage->AddComponent(new CMeshRender);
	CSImage->AddComponent(new CPlayerCSUIScript);
	CSImage->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CSImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\20CSGold.mtrl"), 0);
	CSImage->Transform()->SetAbsolute(true);
	CSImage->Transform()->SetRelativeScale(Vec3(66.f,29.f,1.f));
	CSImage->Transform()->SetRelativePos(PlayerPos);
	SpawnGameObject(CSImage, PlayerPos, 31);
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
	AnnouncePanel->AddComponent(new CIconTimerUIScript);
	AnnouncePanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	AnnouncePanel->Transform()->SetAbsolute(true);
	AnnouncePanel->Transform()->SetRelativeScale(_Scale);
	return AnnouncePanel;
}

void CKillLogUIScript::Announce_EndofGame(Faction _VictimFaction)
{
	CEventMgr::GetInst()->SetIsGameOver(true);

	CGameObject* EndOfGamePanel = new CGameObject;
	EndOfGamePanel->SetName(L"EndOfGame");
	EndOfGamePanel->AddComponent(new CTransform);
	EndOfGamePanel->AddComponent(new CMeshRender);
	EndOfGamePanel->AddComponent(new CEndOfGameUIScript);
	EndOfGamePanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	EndOfGamePanel->Transform()->SetAbsolute(true);

	if (_VictimFaction == CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>()->GetFaction())//파괴된 팀과 같은팀이라면 패배
	{
		EndOfGamePanel->Transform()->SetRelativeScale(Vec3(403.2f, 447.3f, 1.f));
		EndOfGamePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EndOfDefeat.mtrl"), 0);
		
		// sound
		thread t([=]() {
			Sleep(4000);
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\announce_defeat.mp3", 1, 0.5f, true, 0.f, Vec3(0, 0, 0), _VictimFaction);
			});
		t.detach();

		thread t1([=]() {
			Sleep(4000);
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\bgm_ummoners_rift_defeat.mp3", 1, 0.15f, true, 0.f, Vec3(0, 0, 0), _VictimFaction);
			});
		t1.detach();
	}
	else
	{
		EndOfGamePanel->Transform()->SetRelativeScale(Vec3(402.f, 447.f, 1.f));
		EndOfGamePanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EndOfVictory.mtrl"), 0);

		// sound
		if (_VictimFaction == Faction::RED) {

			// sound
			thread t([=]() {
				Sleep(4000);
			CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\announce_victory.mp3", 1, 0.5f, true, 0.f, Vec3(0, 0, 0), Faction::BLUE);
				});
			t.detach();

			thread t1([=]() {
				Sleep(2000);
			CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\bgm_summoners_rift_victory.mp3", 1, 0.3f, true, 0.f, Vec3(0, 0, 0), Faction::BLUE);
				});
			t1.detach();
		}
		else
		{
			// sound
			thread t([=]() {
				Sleep(4000);
			CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\announce_victory.mp3", 1, 0.5f, true, 0.f, Vec3(0, 0, 0), Faction::RED);
				});
			t.detach();
			
			thread t1([=]() {
				Sleep(2000);
			CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\bgm_summoners_rift_victory.mp3", 1, 0.f, true, 0.f, Vec3(0, 0, 0), Faction::RED);
				});
			t1.detach();
		}
	}

	SpawnGameObject(EndOfGamePanel, Vec3(0.f, 0.f, 1.f), 31);
	EndOfGameObj = EndOfGamePanel;
}
