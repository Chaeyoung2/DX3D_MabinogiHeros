#pragma once

#include "Engine_Defines.h"
#include "Base.h"

// 1�ʿ� ���� ������ Ƚ����ŭ�� Ʈ�縦 �����Ѵ�.

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