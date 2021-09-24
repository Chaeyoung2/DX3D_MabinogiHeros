#pragma once

#include "Defines.h"
#include "Camera.h"
#include "Input_Device.h"
#include "Component_Manager.h"

class CKeyMgr;
class CStatic_Camera final : public CCamera
{
private:
	explicit CStatic_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStatic_Camera() = default;
public:
	virtual HRESULT Ready_GameObject(CAMERAINFO * pCamInfo, PROJINFO * pProjInfo);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CInput_Device*				m_pInput_Device = nullptr;
	CKeyMgr*					m_pKey_Manager = nullptr;
private:
	_bool						m_bIsZecallion_StandUpFaceDown = false;
	_bool						m_bIsZecallion_Run = false;
	_bool						m_bIsZecallion_AttackCombo1 = false;
	_bool						m_bIsZecallion_DoubleMash = false;
	_bool						m_bIsZecallion_GaurdBegin = false;
	_bool						m_bIsZecallion_JumpThrow = false;
	_bool						m_bIsZecallion_Spawn = false;

	_bool						m_bIsPermitted_Shaking = true;

	_bool						m_bIsCutSceneMode_InitZecallion = false;

	_bool						m_bIsSettingCompleted_InitZecallionPos = false;
	_vec3						m_vZecallionPos;
private:
	_bool						m_bIsPlusX = true;
	_bool						m_bIsPlusY = false;
	_int							m_iPreZecallionAniIdx = 0;
	_int							m_iCurZecallionAniIdx = 0;
	_vec3						m_vOriAtPos;
	_vec3						m_vOriEyePos;
	_float						m_fSpeed = 0.f;
	_float						m_fShakePower = 0.0f;
	_float						m_fShakeTime = 0.0f;
	_float						m_fTimeDelta_Shaking = 0.f;
public:
	static CStatic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev, CAMERAINFO* pCamInfo, PROJINFO* pProjInfo = nullptr);
	virtual _ulong Free(void);

};

