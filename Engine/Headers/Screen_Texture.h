#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CScreen_Texture final : public CVIBuffer
{
private:
	explicit CScreen_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScreen_Texture(const CScreen_Texture& rhs);
	virtual ~CScreen_Texture() = default;
public:
	virtual HRESULT Ready_Component(void);
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void Render_Buffer(const _matrix* pWorldMatrix);
private:
	void Transform(const _matrix* pWorldMatrix);
public:
	virtual CComponent* Clone(void);
	static CScreen_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END