#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CFrustum;
class DLL_EXPORT CTerrain_Buffer : public CVIBuffer
{
private:
	explicit CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain_Buffer(const CTerrain_Buffer& rhs);
	virtual ~CTerrain_Buffer(void) = default;
public:
	virtual HRESULT Ready_Component(const _tchar* pHeightmap, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ, const _ulong& dwInterval,
		vector<TERRAIN_DATA_SOLE>* pVecTerrainDataSole = nullptr);
	virtual void Render_Buffer(void);
	HRESULT Optimize_Buffer(CFrustum* pFrustum);
private:
	INDEX32*				m_pIndices = nullptr;
public:
	virtual CComponent* Clone(void);
	static CTerrain_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightmap, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ, const _ulong& dwInterval,
		vector<TERRAIN_DATA_SOLE>* pVecTerrainDataSole = nullptr);
	virtual _ulong Free(void);

};

END