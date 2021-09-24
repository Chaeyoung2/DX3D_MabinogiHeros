#pragma once

#include "Component.h"

BEGIN(Engine)

// Vertex, Index
class DLL_EXPORT CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;
public:
	const _vec3* Get_VertexPos(void) const {
		return m_pVertexPos;}

	tuple<_ulong, _ulong, _ulong> Get_BufferInfomation(void) {
		return make_tuple(m_dwCntX, m_dwCntZ, m_dwInterval); }
public:
	virtual HRESULT Ready_Component(void);
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void Render_Buffer(void);
protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	_ulong							m_dwVtxCnt = 0;
	_ulong							m_dwVtxSize = 0;
	_ulong							m_dwVtxFVF = 0;
	_ulong							m_dwCntX, m_dwCntZ, m_dwInterval;
protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_ulong							m_dwIdxSize = 0;
	_ulong							m_dwTriCnt = 0;
	D3DFORMAT						m_IdxFmt;
protected:
	_vec3*							m_pVertexPos = nullptr;
	VTXNORTEX*					m_pVertexNormalTexture = nullptr;
public:
	virtual CComponent* Clone(void) = 0;
	virtual _ulong Free(void);
};

END