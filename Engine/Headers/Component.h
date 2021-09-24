#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent(void) = default;
public:
	virtual HRESULT Ready_Component(void) { return NOERROR; }
	virtual _int Update_Component(const _float& fTimeDelta);
protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
protected:
	_bool				m_isClone = false;
public:
	virtual CComponent* Clone(void) = 0;
	virtual _ulong Free(void) override;
};

END