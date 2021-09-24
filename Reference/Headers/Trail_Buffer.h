#pragma once

#include "VIBuffer.h"

BEGIN(Engine)


class DLL_EXPORT CTrail_Buffer : public CVIBuffer
{
private:
	explicit CTrail_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrail_Buffer(const CTrail_Buffer& rhs);
	virtual ~CTrail_Buffer(void) = default;
public:
	virtual HRESULT Ready_Component();
	virtual void Render_Buffer(const _matrix* pMatWorld);
public:
	void Set_VertexTrail(const _vec3* pPos, const _vec3* pDir, const _float& fLength, const DWORD& dwCnt);
public:
	virtual CComponent* Clone(void);
	static CTrail_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
private:
	VTXTRAIL*		m_pVertex;
};

END