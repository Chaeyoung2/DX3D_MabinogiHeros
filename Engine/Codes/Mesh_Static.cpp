#include "..\Headers\Mesh_Static.h"

USING(Engine)

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pAdjacency(nullptr)
	, m_pMaterials(nullptr)
	, m_dwNumMaterials(0)
	, m_pMesh(nullptr)
	, m_pTextures(nullptr)
{

}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CMesh(rhs)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pMaterials(rhs.m_pMaterials)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_pMesh(rhs.m_pMesh)
	, m_pSubsetInfo(rhs.m_pSubsetInfo)
	, m_pTextures(rhs.m_pTextures)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
{
	m_pAdjacency->AddRef();
	m_pMaterials->AddRef();
	m_pMesh->AddRef();

	for (size_t i = 0; i < m_dwNumMaterials; i++)
	{
		m_pTextures[i]->AddRef();
	}
}


CMesh_Static::RENDERTYPE CMesh_Static::Get_RenderType(const char * pFileName)
{
	_uint iLength = strlen(pFileName);

	for (size_t i = iLength - 1; i >= 0; --i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return RENDER_ALPHATEST;

			if (pFileName[i - 1] == 'D')
				return RENDER_NORMAL;
		}
	}

	return RENDERTYPE();
}

HRESULT CMesh_Static::Ready_Mesh_Static(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar	szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, &m_pAdjacency, &m_pMaterials, nullptr, &m_dwNumMaterials, &m_pMesh)))
		return E_FAIL;

	m_pSubsetInfo = (D3DXMATERIAL*)m_pMaterials->GetBufferPointer();

	m_pTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for (size_t i = 0; i < m_dwNumMaterials; i++)
	{
		_tchar			szFileName[128] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pSubsetInfo[i].pTextureFilename, strlen(m_pSubsetInfo[i].pTextureFilename), szFileName, 128);

		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_pTextures[i])))
			return E_FAIL;
	}

	D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);


	// 정점구조체 안에 선언된  FVF의 정보를 얻어온다.
	m_pMesh->GetDeclaration(Decl);

	_ushort			wOffset = 0;

	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (D3DDECLUSAGE_POSITION == Decl[i].Usage)
		{
			wOffset = Decl[i].Offset;
			break;
		}
	}

	//void*		pVertex = nullptr;
	_byte*		pVertex = nullptr;

	m_pMesh->LockVertexBuffer(0, (void**)&pVertex);

	if (FAILED(D3DXComputeBoundingBox((_vec3*)(pVertex + wOffset), m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax)))
		return E_FAIL;

	m_pMesh->UnlockVertexBuffer();

	return NOERROR;
}

void CMesh_Static::Render_Mesh(LPD3DXEFFECT pEffect/* = nullptr*/)
{
	if (nullptr == m_pMesh)
		return;

	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		if (nullptr != pEffect)
		{
			_uint		iPass = 0;

			if (RENDER_NORMAL == Get_RenderType(m_pSubsetInfo[i].pTextureFilename))
				iPass = 0;

			if (RENDER_ALPHATEST == Get_RenderType(m_pSubsetInfo[i].pTextureFilename))
				iPass = 1;

			pEffect->BeginPass(iPass);
		}

		m_pGraphicDev->SetMaterial(&m_pSubsetInfo[i].MatD3D);

		if (nullptr == pEffect)
			m_pGraphicDev->SetTexture(0, m_pTextures[i]);
		else
		{
			pEffect->SetTexture("g_BaseTexture", m_pTextures[i]);
			pEffect->CommitChanges();
		}

		m_pMesh->DrawSubset(i);

		if (nullptr != pEffect)
			pEffect->EndPass();

	}
}

CMesh_Static * CMesh_Static::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Static *	pInstance = new CMesh_Static(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh_Static(pFilePath, pFileName)))
	{
		MSG_BOX("CMesh_Static Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Static::Clone(void)
{
	return new CMesh_Static(*this);
}

_ulong CMesh_Static::Free(void)
{
	/*
	D3DXMATERIAL*			m_pSubsetInfo = nullptr;
	LPDIRECT3DTEXTURE9*		m_pTextures = nullptr;
	*/
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMaterials);
	Safe_Release(m_pMesh);

	for (size_t i = 0; i < m_dwNumMaterials; i++)
	{
		Safe_Release(m_pTextures[i]);
	}

	if (false == m_isClone)
		Safe_Delete_Array(m_pTextures);

	return CComponent::Free();
}
