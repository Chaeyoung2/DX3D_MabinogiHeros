#include "..\Headers\Input_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
	: m_pInputSDK(nullptr)
	, m_pKeyBoard(nullptr)
	, m_pMouse(nullptr)
{
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8
		, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(Ready_KeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(Ready_Mouse(hWnd)))
		return E_FAIL;

	return NOERROR;
}

void CInput_Device::Invalidate_InputDev_State(void)
{
	if (nullptr == m_pKeyBoard)
		return;

	if (nullptr == m_pMouse)
		return;

	m_pKeyBoard->GetDeviceState(256, m_byKeyBoard_State);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

HRESULT CInput_Device::Ready_KeyBoard(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CInput_Device::Ready_Mouse(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return NOERROR;
}

_ulong CInput_Device::Free(void)
{
	_ulong dwRefCnt = 0;

	if(dwRefCnt = Safe_Release(m_pKeyBoard))
	{
		MSG_BOX("KeyBoard Release Failed");
		return dwRefCnt;
	}

	if(dwRefCnt = Safe_Release(m_pMouse))
	{
		MSG_BOX("Mouse Release Failed");
		return dwRefCnt;
	}

	if(dwRefCnt = Safe_Release(m_pInputSDK))
	{
		MSG_BOX("m_pInputSDK Release Failed");
		return dwRefCnt;
	}

	return dwRefCnt;
}
