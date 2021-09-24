#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CFrustum : public CComponent
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum() = default;
public:
	HRESULT Ready_Frustum(void);
	_bool Culling_toFrustum(const _vec3* pPositionInWorld, const _float& fRadius);

	_bool isIn_Frustum(const D3DXPLANE* pPlane, const _vec3* pPositionInWorld, const _float & fRadius);
private:
	_vec3				m_vPoint[8];
public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
	virtual _ulong Free(void);
};

END