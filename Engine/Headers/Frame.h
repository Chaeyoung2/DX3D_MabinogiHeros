#pragma once

#include "Engine_Defines.h"
#include "Base.h"

// 1초에 내가 지정한 횟수만큼만 트루를 리턴한다.

BEGIN(Engine)

class CFrame : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame() = default;
public:
	HRESULT Ready_Frame(const _float& fCallCnt);
	_bool Permit_Call(const _float& fTimeDelta);
private:
	_float		m_fCallPerSec = 0.f;
	_float		m_fTimeAcc = 0.f;
public:
	static CFrame* Create(const _float& fCallCnt);
	virtual _ulong Free(void);
};

END