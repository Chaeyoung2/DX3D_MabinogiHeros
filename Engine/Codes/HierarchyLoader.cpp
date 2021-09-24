#include "..\Headers\HierarchyLoader.h"

USING(Engine)

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
	: m_pGraphicDev(pGraphicDev)
	, m_pFilePath(pFilePath)
{
	m_pGraphicDev->AddRef();
}

// 뼈를 생성한다.
STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*			pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pNewFrame->Name, Name);

	pNewFrame->matCombinedTransformationMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	*ppNewFrame = pNewFrame;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name
	, CONST D3DXMESHDATA * pMeshData
	, CONST D3DXMATERIAL * pMaterials
	, CONST D3DXEFFECTINSTANCE * pEffectInstances
	, DWORD NumMaterials
	, CONST DWORD * pAdjacency
	, LPD3DXSKININFO pSkinInfo
	, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*			pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pNewMeshContainer->Name, Name);

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	pNewMeshContainer->MeshData.pMesh = pMeshData->pMesh;

	LPD3DXMESH			pMesh = pNewMeshContainer->MeshData.pMesh;
	pMesh->AddRef();

	pNewMeshContainer->NumMaterials = NumMaterials;

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
	pNewMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[NumMaterials];

	for (size_t i = 0; i < pNewMeshContainer->NumMaterials; ++i)
	{
		pNewMeshContainer->pMaterials[i] = pMaterials[i];

		_tchar			szFullPath[128] = L"";
		lstrcpy(szFullPath, m_pFilePath);

		_tchar			szFileName[128] = L"";

		MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename)
			, szFileName, 128);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppTextures[i])))
			return E_FAIL;
	}

	_ulong dwNumFace = pMesh->GetNumFaces();

	pNewMeshContainer->pAdjacency = new _ulong[dwNumFace * 3];
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFace * 3);

	// 피부에 대한 정보를 가진다.
	// 피부와 연관되어있는 다양한 기능을 가진 함수를 2ㅈ3ㅔ공한다.
	pNewMeshContainer->pSkinInfo = pSkinInfo;
	pNewMeshContainer->pSkinInfo->AddRef();

	pNewMeshContainer->dwNumBones = pNewMeshContainer->pSkinInfo->GetNumBones();

	pNewMeshContainer->ppCombinedTransformationMatrices = new _matrix*[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->ppCombinedTransformationMatrices, sizeof(_matrix*) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->pOffsetMatrices = new _matrix[pNewMeshContainer->dwNumBones];

	for (_uint i = 0; i < pNewMeshContainer->dwNumBones; ++i)
		pNewMeshContainer->pOffsetMatrices[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

	pNewMeshContainer->pRenderingMatrices = new _matrix[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->pRenderingMatrices, sizeof(_matrix) * pNewMeshContainer->dwNumBones);

	// 원본메시와 똑같은 형태의 복제된 메시를 하나 생성해놓았다.
	pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), m_pGraphicDev, &pNewMeshContainer->pOriginal_Mesh);
	pMesh->AddRef();



	D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);


	// 정점구조체 안에 선언된  FVF의 정보를 얻어온다.
	pMesh->GetDeclaration(Decl);

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

	pMesh->LockVertexBuffer(0, (void**)&pVertex);

	if (FAILED(D3DXComputeBoundingBox((_vec3*)(pVertex + wOffset), pMesh->GetNumVertices(), D3DXGetFVFVertexSize(pMesh->GetFVF()), &pNewMeshContainer->m_vMin, &pNewMeshContainer->m_vMax)))
		return E_FAIL;

	pMesh->UnlockVertexBuffer();

	Safe_Release(pMesh);

	*ppNewMeshContainer = pNewMeshContainer;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	if (nullptr != pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}

	Safe_Delete(pFrameToFree);

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;
	Safe_Release(pMeshContainer->pOriginal_Mesh);

	for (_uint i = 0; i < pMeshContainer->NumMaterials; ++i)
		Safe_Release(pMeshContainer->ppTextures[i]);
	Safe_Delete_Array(pMeshContainer->ppTextures);

	Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrices);

	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete(pMeshContainer);

	return NOERROR;
}

HRESULT CHierarchyLoader::Allocate_Name(char ** ppSour, const char * pDest)
{
	if (nullptr == pDest)
		return E_FAIL;

	_ulong			dwLength = strlen(pDest) + 1;

	*ppSour = new char[dwLength];
	ZeroMemory(*ppSour, sizeof(char) * dwLength);

	strcpy(*ppSour, pDest);

	return NOERROR;
}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CHierarchyLoader *	pInstance = new CHierarchyLoader(pGraphicDev, pFilePath);

	return pInstance;
}
_ulong CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return _ulong();
}
