#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CHierarchyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual ~CHierarchyLoader() = default;
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
public:
	HRESULT Allocate_Name(char** ppSour, const char* pDest);
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	const _tchar*				m_pFilePath = nullptr;
public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	_ulong Release(void);
};

END