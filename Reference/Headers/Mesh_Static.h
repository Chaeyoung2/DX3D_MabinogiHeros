#pragma once

#include "Mesh.h"

BEGIN(Engine)

class DLL_EXPORT CMesh_Static : public CMesh
{
public:
	enum RENDERTYPE { RENDER_ALPHATEST, RENDER_ALPHABLEND, RENDER_NORMAL, RENDER_END };
private:
	explicit CMesh_Static(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh_Static(const CMesh_Static& rhs);
	virtual ~CMesh_Static() = default;
public:
	virtual const _vec3* Get_Min(const _uint& iMeshIdx = 0) const {
		return &m_vMin; }
	virtual const _vec3* Get_Max(const _uint& iMeshIdx = 0) const {
		return &m_vMax; 	}
	RENDERTYPE Get_RenderType(const char* pFileName);
public:
	HRESULT Ready_Mesh_Static(const _tchar* pFilePath, const _tchar* pFileName);
	virtual void Render_Mesh(LPD3DXEFFECT pEffect = nullptr);
private:
	LPD3DXBUFFER			m_pAdjacency = nullptr;
	LPD3DXBUFFER			m_pMaterials = nullptr;
	_ulong					m_dwNumMaterials = 0;
	LPD3DXMESH				m_pMesh = nullptr;
	_vec3					m_vMin, m_vMax;
private: // For.SubSet
	D3DXMATERIAL*			m_pSubsetInfo = nullptr;
	LPDIRECT3DTEXTURE9*		m_pTextures = nullptr;
public:
	static CMesh_Static* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void);
	virtual _ulong Free(void);
};

END

