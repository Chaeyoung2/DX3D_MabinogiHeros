#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CCube_Texture final : public CVIBuffer
{
private:
	explicit CCube_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Texture(const CCube_Texture& rhs);
	virtual ~CCube_Texture() = default;
public:
	virtual HRESULT Ready_Component(void);
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void Render_Buffer(void)final;
public:
	virtual CComponent* Clone(void);
	static CCube_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END