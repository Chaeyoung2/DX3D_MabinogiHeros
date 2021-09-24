#pragma once

#include "Defines.h"
#include "Camera.h"
#include "Input_Device.h"

class CDynamic_Camera final : public CCamera
{
private:
	explicit CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamic_Camera() = default;
public:
	virtual HRESULT Ready_GameObject(CAMERAINFO * pCamInfo, PROJINFO * pProjInfo);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CInput_Device*				m_pInput_Device = nullptr;
private:
	_float						m_fSpeed = 0.f;
public:
	static CDynamic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev, CAMERAINFO* pCamInfo, PROJINFO* pProjInfo = nullptr );
	virtual _ulong Free(void);

};

