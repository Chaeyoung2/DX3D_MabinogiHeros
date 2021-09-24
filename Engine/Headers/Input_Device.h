#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CInput_Device : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSESTATE { DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON, DIM_END };
	enum MOUSEMOVESTATE { DIMMS_X, DIMMS_Y, DIMMS_WHEEL };
private:
	explicit CInput_Device(void);
	virtual ~CInput_Device() = default;
public:
	_byte Get_DIKState(const _ubyte& byKey) {
		return m_byKeyBoard_State[byKey];}

	_byte Get_DIMState(const MOUSESTATE& eMouseState) {
		return m_MouseState.rgbButtons[eMouseState];}

	_long Get_DIMMoveState(const MOUSEMOVESTATE& eMouseMoveState) {
		return *((_long*)&m_MouseState + eMouseMoveState);}

public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	void Invalidate_InputDev_State(void);
private:
	LPDIRECTINPUT8				m_pInputSDK; // 내 입력장치에 대한 정보를 조사한다.
private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	_byte						m_byKeyBoard_State[256];

private:
	LPDIRECTINPUTDEVICE8		m_pMouse;
	DIMOUSESTATE				m_MouseState;
private:
	HRESULT Ready_KeyBoard(HWND hWnd);
	HRESULT Ready_Mouse(HWND hWnd);

public:
	virtual _ulong Free(void);
};

END