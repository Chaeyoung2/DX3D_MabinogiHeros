#include "..\Headers\Mesh.h"

USING(Engine)

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CMesh::CMesh(const CMesh & rhs)
	: CComponent(rhs)
{

}

HRESULT CMesh::Ready_Mesh(const _tchar * pFilePath, const _tchar * pFileName)
{
	return E_NOTIMPL;
}

void CMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
}

CComponent * CMesh::Clone(void)
{
	return nullptr;
}

_ulong CMesh::Free(void)
{
	return _ulong();
}
