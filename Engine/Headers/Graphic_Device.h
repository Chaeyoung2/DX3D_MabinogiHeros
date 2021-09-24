#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CGraphic_Device : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	explicit CGraphic_Device(void);
	virtual ~CGraphic_Device(void) = default;

public: // getter
	LPDIRECT3DDEVICE9 Get_GraphicDev(void) {
		return m_pGraphic_Device;}
	LPD3DXFONT			Get_Font() {
		return m_pFont;
	}
public:
	HRESULT Ready_GraphicDev(WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY);
private:
	LPDIRECT3D9					m_pSDK = nullptr; // �� ��ġ��ü�� ������ ����
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr; // ��ġ��ü�� ��ǥ�ϴ� ��ü
	LPD3DXFONT			m_pFont = nullptr; // ��Ʈ ��ü
private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY);
public:
	virtual _ulong Free(void);
};

END	

