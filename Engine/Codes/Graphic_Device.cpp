#include "..\Headers\Graphic_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(Engine::CGraphic_Device)

Engine::CGraphic_Device::CGraphic_Device(void)
: m_pSDK(nullptr)
, m_pGraphic_Device(nullptr)
{
 
}


HRESULT Engine::CGraphic_Device::Ready_GraphicDev(WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY)
{
	if (nullptr != m_pSDK 
		|| nullptr != m_pGraphic_Device)
		return E_FAIL;

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	// 장치의 정보(성능) 조사한다.
	D3DCAPS9		devicecaps; // 장치의 정보를 보관하기위한 구조체
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	// 장치의 정보를 얻어온다.
	if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		&devicecaps)))
	{
		MSG_BOX("GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp;

	if(devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, Mode, hWnd, wSizeX, wSizeY);

	if(FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, hWnd, vp, &d3dpp, &m_pGraphic_Device)))
	{
		MSG_BOX("CreateDevice Failed");
		return E_FAIL;
	}


	//폰트 객체를 만들어 주자
	//그전에 폰트 객체를 먼저 초기화(설정값지정).
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(FontInfo));


	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_BOLD;
	FontInfo.CharSet = HANGEUL_CHARSET;

	lstrcpy(FontInfo.FaceName, L"돋움체");


	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &FontInfo, &m_pFont)))
	{
		MSG_BOX("폰트 객체 생성 실패!");
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CGraphic_Device::SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY)
{
	d3dpp.BackBufferWidth = wSizeX;
	d3dpp.BackBufferHeight = wSizeY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = Mode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}


_ulong Engine::CGraphic_Device::Free(void)
{
	_ulong dwRefCnt = 0;


	if (dwRefCnt = Engine::Safe_Release(m_pFont))
	{
		MSG_BOX("m_pFont Release Failed");
		return dwRefCnt;
	}
	
	if (dwRefCnt = Engine::Safe_Release(m_pGraphic_Device))
	{
		MSG_BOX("m_pGraphic_Device Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = Engine::Safe_Release(m_pSDK))
	{
		MSG_BOX("m_pSDK Release Failed");
		return dwRefCnt;
	}


	return dwRefCnt;
}
