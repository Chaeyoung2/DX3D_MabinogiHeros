#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CRect_Texture final : public CVIBuffer
{
private:
	explicit CRect_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRect_Texture(const CRect_Texture& rhs);
	virtual ~CRect_Texture() = default;
public:
	virtual HRESULT Ready_Component(void);
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void Render_Buffer(void)final;
public:
	virtual CComponent* Clone(void);
	static CRect_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END