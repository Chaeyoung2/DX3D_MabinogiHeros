#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CMesh : public CComponent
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;
public:
	virtual const _vec3* Get_Min(const _uint& iMeshIdx = 0) const = 0;
	virtual const _vec3* Get_Max(const _uint& iMeshIdx = 0) const = 0;
public:
	HRESULT Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	virtual void Render_Mesh(LPD3DXEFFECT pEffect = nullptr) = 0;
public:
	virtual CComponent* Clone(void) = 0;
	virtual _ulong Free(void);
};

END