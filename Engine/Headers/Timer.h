#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	inline _float Get_TimeDelta(void) const {
		return m_fTimeDelta;}
public:
	void Compute_TimeDelta(void);
public:
	HRESULT Ready_Timer(void);
private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;
private:
	_float					m_fTimeDelta = 0.f;
public:
	static CTimer* Create(void);
	virtual _ulong Free(void)final;
};

END