#pragma once

#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CAnimationCtrl : public CBase
{
public:
	explicit CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXANIMATIONCONTROLLER	pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl(void) = default;
public:
	HRESULT Ready_AnimationCtrl(void);
	void Set_AnimationSet(const _uint& iIndex, _double dMagicNumber);
	void Play_AnimationSet(const _float& fTimeDelta);
public:
	_double Get_Period(){
		return m_Period;
	}
	_double Get_TrackPosition();
	_uint Get_AnimationIdx() {
		return m_iAnimationIdx;
	}

public:
	void SetTrackPos(_double dTrackPos)
	{
		m_pAniCtrl->SetTrackPosition(m_iCurrentTrack, dTrackPos);
	} 
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl = nullptr;
private:
	_uint						m_iCurrentTrack = 0;
	_uint						m_iNewTrack = 1;
	_uint						m_iCurrentIdx = 0;
	_double						m_TimeDeltaAcc = 0.0;
private:
	_double					m_Period = 0.0;
	_double			m_TrackPosition = 0.0;
	_uint						m_iAnimationIdx = 0;
public:
	static CAnimationCtrl* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXANIMATIONCONTROLLER	m_pAniCtrl);
	static CAnimationCtrl* Create(const CAnimationCtrl& rhs);
	virtual _ulong Free(void);
};

END