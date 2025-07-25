﻿// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "Client.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "CEditorObjMgr.h"
#include <Engine\CDevice.h>
// ImGui
#include "ImGuiMgr.h"

#include "TestLevel.h"

#include <Engine/CEventMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <iostream>
#include <chrono> // for fps
#include "ServerEventMgr.h"

#include <Script/CPlayerScript.h>
#include <Engine/CAnim3D.h>
#include <Script/CChampionScript.h>

#include <Script/CSendServerEventMgr.h>
// 전역 변수:
HINSTANCE   hInst;    // 현재 인스턴스입니다.
HWND        g_hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(968721);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // CEngine 초기화
    if (FAILED(CEngine::GetInst()->init(g_hWnd, 1600, 1000)))
    {
        return 0;
    }

    // Editor 초기화
    CEditorObjMgr::GetInst()->init();
       
    // ImGui 초기화
    ImGuiMgr::GetInst()->init(g_hWnd);

    // 테스트 용 레벨 생성
    //CreateTestLevel();
    //CreateLoginLevel();

    // 메세지 루프
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    MSG msg;

    // 해상도 수정
    SetProcessDPIAware();
    
    AllocConsole();
    //// 표준 출력을 콘솔 창으로 리디렉션
    freopen("CONOUT$", "w", stdout);
    
    this_thread::sleep_for(1s);
    
    ClientServiceRef service = MakeShared<ClientService>(
        //NetAddress(L"221.148.206.199", 40000),  // 다혜집 데탑 IP
        //NetAddress(L"14.35.246.224", 40000),    // snow
        //NetAddress(L"192.168.0.19", 40000), //  내부ip
        NetAddress(L"127.0.0.1", 40000), // 로컬 호스트
        MakeShared<IocpCore>(),
        MakeShared<ServerSession>, // TODO : SessionManager 등
        1);
    
    ASSERT_CRASH(service->Start());
    
    GThreadManager->SetFlags(1);
    for (int32 i = 0; i < 2; i++)
    {
        GThreadManager->Launch([=]()
        {
            while (true)
            {
                service->GetIocpCore()->Dispatch(10);
                if (GThreadManager->GetFlags() == 0)
                {
                    this_thread::sleep_for(500ms);
                    return;
                    //break;
                }
                
            }
        });
    }    

    while (true) 
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (KEY_TAP(KEY::LSHIFT) && service->GetCurrentSessionCount() > 0)
            {
                Send_CLogin(service, L"랄랄라라");
            }
            else if (KEY_TAP(KEY::N))
            {
                Send_CPickFaction(service);
            }
            else if (KEY_TAP(KEY::M))
            { 
                Send_CPickChampion(service,ChampionType::MALPHITE);
               //Send_CPickChampion(service, ChampionType::JINX);
            }
            else if (KEY_TAP(KEY::NUM_4))      
            {
               //std::cout << "Test Pakcet" << endl;
               //
               //wstring _chatTest = L"Hello my chat";
               //PKT_C_CHAT_WRITE  pktWriter(1);
               //PKT_C_CHAT_WRITE::ChatLog chatLog = pktWriter.ReserveChatLog(_chatTest.size());
               //for (int i = 0; i < _chatTest.size(); i++)
               //{
               //    chatLog[i] = { _chatTest[i] };
               //}
               //
               //SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
               //service->Broadcast(sendBuffer);
               //std::cout << "===============================" << endl;
            }

            if (CSendServerEventMgr::GetInst()->GetIsGameStop() == false)
            {
                CEngine::GetInst()->progress();

                if (IsInGame) // C->S 패킷 전송
                    ServerEventMgr::GetInst()->sendtick(service);


                // 랜덤으로 온 서버패킷을 핸들러에서 서버 이벤트 매니저에 등록해둠.

                // Event 처리
                CEventMgr::GetInst()->tick();

                // Server에서 온 패킷 정보를 클라이언트에 반영.
                ServerEventMgr::GetInst()->clienttick();


                CEditorObjMgr::GetInst()->progress();

                if (CEngine::GetInst()->GetImguiActive()) {
                    ImGuiMgr::GetInst()->progress();
                }

                // 렌더 종료
                CDevice::GetInst()->Present();
            }

        }       



    }

     GThreadManager->Join();
    
    //// 콘솔 창 닫기
    ////fclose(stdout);
    FreeConsole();


    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = L"MyWindow";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(L"MyWindow", L"MyGame", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, false);
   UpdateWindow(g_hWnd);

   return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;


    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        GThreadManager->SetFlags(0);
        this_thread::sleep_for(1s);
        PostQuitMessage(0);
        break;



    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
