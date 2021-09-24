// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "../Headers/Defines.h"
#include "../Headers/MainApp.h"
#include "Timer_Manager.h"
#include "Frame_Manager.h"
#include "Component_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Object_manager.h"
#include "Management.h"



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;
int			g_iScene;
bool		g_bShowingCollider = false;
bool		g_bShowingNaviMesh = false;
bool		g_bUsingStaticCamera = true;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
_ulong				Release_Singleton(void);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
	// api에서 콘솔창 띄우기.
	if (AllocConsole())
		freopen("CONOUT$", "w", stdout);
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // TODO: 여기에 코드를 입력합니다.
	CMainApp*			pMainApp = nullptr;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadStringW(hInstance, IDS_APP_TITLE, L"Mabinogi Heros", MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

	// Timer fmf cnrrkgkwk
	Engine::CTimer_Manager*		pTimer_Manager = CTimer_Manager::Get_Instance();
	pTimer_Manager->AddRef();

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default")))
		return FALSE;

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60")))
		return FALSE;

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_200")))
		return FALSE;


	// CFrame dmf aksemfwk.
	Engine::CFrame_Manager*		pFrame_Manager = CFrame_Manager::Get_Instance();
	pFrame_Manager->AddRef();

	if (FAILED(pFrame_Manager->Add_Frame(L"Frame_60", 60.0f)))
		return FALSE;

	if (FAILED(pFrame_Manager->Add_Frame(L"Frame_200", 200.0f)))
		return FALSE;

	CInput_Device::Get_Instance()->Ready_Input_Device(g_hInst, g_hWnd);

    // 기본 메시지 루프입니다.
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
			if (nullptr == pTimer_Manager)
				return FALSE;

			pTimer_Manager->Compute_TimeDelta(L"Timer_Default");

			_float		fTimeDelta_Default = pTimer_Manager->Get_TimeDelta(L"Timer_Default");

			// 무제한			

			// 1초에 60번만 트루리턴
			if (true == pFrame_Manager->Permit_Call(L"Frame_60", fTimeDelta_Default))
			{			

				CInput_Device::Get_Instance()->Invalidate_InputDev_State();
				pTimer_Manager->Compute_TimeDelta(L"Timer_60");

				_float		fTimeDelta_60 = pTimer_Manager->Get_TimeDelta(L"Timer_60");
				
				_int iExitCode = pMainApp->Update_MainApp(fTimeDelta_60);

				if (iExitCode & 0x80000000)
					break;
				pMainApp->Render_MainApp();		
			}

			// 1초에 200번만 트루리턴
			if (true == pFrame_Manager->Permit_Call(L"Frame_200", fTimeDelta_Default))
			{
				pTimer_Manager->Compute_TimeDelta(L"Timer_200");

				_float		fTimeDelta_200 = pTimer_Manager->Get_TimeDelta(L"Timer_200");
			}			
		}
	}

	Engine::Safe_Release(pTimer_Manager);
	Engine::Safe_Release(pFrame_Manager);

	_ulong			dwRefCnt = 0;

	if (dwRefCnt = Engine::Safe_Release(pMainApp))
		MessageBox(g_hWnd, L"CMainApp Release Failed", nullptr, MB_OK);	

	if (dwRefCnt = Release_Singleton())
		MessageBox(g_hWnd, L"Singleton Release Failed", nullptr, MB_OK);

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
	wcex.lpszMenuName = NULL; /*MAKEINTRESOURCEW(IDC_CLIENT);*/
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	RECT		rcWindows = { 0, 0, g_iBackCX, g_iBackCY };

	AdjustWindowRect(&rcWindows, WS_OVERLAPPEDWINDOW, TRUE);	

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, rcWindows.right - rcWindows.left, rcWindows.bottom - rcWindows.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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

_ulong Release_Singleton(void)
{
	_ulong	dwRefCnt = 0;


	if (dwRefCnt = CTimer_Manager::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CTimer_Manager Release Failed");
		return dwRefCnt;
	}
	if (dwRefCnt = CFrame_Manager::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CFrame_Manager Release Failed");
		return dwRefCnt;
	}

	
	if (dwRefCnt = CManagement::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CManagement Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CObject_Manager::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CObject_Manager Release Failed");
		return dwRefCnt;
	}


	if (dwRefCnt = CComponent_Manager::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CComponent_Manager Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CInput_Device::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CInput_Device Release Failed");
		return dwRefCnt;
	}


	if (dwRefCnt = CGraphic_Device::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX("CGraphic_Device Release Failed");
		return dwRefCnt;
	}

	return dwRefCnt;
}
