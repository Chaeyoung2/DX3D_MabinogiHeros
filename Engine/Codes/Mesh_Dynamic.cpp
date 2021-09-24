#include "..\Headers\Mesh_Dynamic.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"

USING(Engine)

CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pLoader(nullptr)
	, m_pAnimationCtrl(nullptr)
{

}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic & rhs)
	: CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAnimationCtrl = CAnimationCtrl::Create(*rhs.m_pAnimationCtrl);

}

const _vec3 * CMesh_Dynamic::Get_Min(const _uint & iMeshIdx) const
{
	auto	iter = m_MeshContainerList.begin();

	for (size_t i = 0; i < iMeshIdx; ++i)
		++iter;

	return &(*iter)->m_vMin;
}

const _vec3 * CMesh_Dynamic::Get_Max(const _uint & iMeshIdx) const
{
	auto	iter = m_MeshContainerList.begin();

	for (size_t i = 0; i < iMeshIdx; ++i)
		++iter;

	return &(*iter)->m_vMax;
}

_double CMesh_Dynamic::Get_TrackPosition()
{
	if (m_pAnimationCtrl != nullptr)
		return (m_pAnimationCtrl->Get_TrackPosition());
	else
		return 0.0;
}

_double CMesh_Dynamic::Get_Period()
{
	if (m_pAnimationCtrl != nullptr)
		return (m_pAnimationCtrl->Get_Period());
	else
		return 0.0;
}

_uint CMesh_Dynamic::Get_AnimationIdx()
{
	if (m_pAnimationCtrl != nullptr)
		return (m_pAnimationCtrl->Get_AnimationIdx());
	else
		return 999;
}

void CMesh_Dynamic::SetTrackPos(_double dTrackPos)
{

		if (m_pAnimationCtrl != nullptr)
			m_pAnimationCtrl->SetTrackPos(dTrackPos);

}

HRESULT CMesh_Dynamic::Ready_Mesh_Dynamic(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar	szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER			pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, m_pLoader, nullptr, &m_pRootFrame, &pAniCtrl)))
		return E_FAIL;

	m_pAnimationCtrl = CAnimationCtrl::Create(m_pGraphicDev, pAniCtrl);
	if (nullptr == m_pAnimationCtrl)
		return E_FAIL;

	Engine::Safe_Release(pAniCtrl);



	_matrix			matPivot;
	Update_CombiendTransformationMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, *D3DXMatrixRotationY(&matPivot, D3DXToRadian(0.0f)));

	SetUp_CombinedMatricesPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return NOERROR;
}

void CMesh_Dynamic::Render_Mesh(LPD3DXEFFECT pEffect/* = nullptr*/)
{
	for (auto& pMeshContainer : m_MeshContainerList)
	{
		for (_ulong j = 0; j < pMeshContainer->dwNumBones; ++j)
		{
			pMeshContainer->pRenderingMatrices[j]
				= pMeshContainer->pOffsetMatrices[j] * *pMeshContainer->ppCombinedTransformationMatrices[j];
		}

		void		*pVerticesSrc = nullptr, *pVerticesDst = nullptr;

		pMeshContainer->pOriginal_Mesh->LockVertexBuffer(0, &pVerticesSrc);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pVerticesDst);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrices, nullptr, pVerticesSrc, pVerticesDst);

		pMeshContainer->pOriginal_Mesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();


		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			//m_pGraphicDev->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);

			// 뼈대들의 행렬정보대로 메시들의 정점 위치를 변경한다. + 피부를 늘리는 작업도 수행(스키닝)

			if (nullptr == pEffect)
				m_pGraphicDev->SetTexture(0, pMeshContainer->ppTextures[i]);
			else
			{
				pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTextures[i]);
				pEffect->CommitChanges();
			}

			pMeshContainer->MeshData.pMesh->DrawSubset(i);

		}
	
	}
}

void CMesh_Dynamic::Set_AnimationSet(const _uint & iIndex, _double dMagicNumber)
{
	m_iAnimationIdx = iIndex;
	if (nullptr == m_pAnimationCtrl)
		return;
	m_pAnimationCtrl->Set_AnimationSet(iIndex, dMagicNumber);
}

void CMesh_Dynamic::Play_AnimationSet(const _float & fTimeDelta)
{
	if (nullptr == m_pAnimationCtrl)
		return;

	m_pAnimationCtrl->Play_AnimationSet(fTimeDelta);

	_matrix			matPivot;
	D3DXMatrixIdentity(&matPivot);

	Update_CombiendTransformationMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, *D3DXMatrixRotationY(&matPivot, D3DXToRadian(0.0f)));

}

// 뼈들이 가지고 있는 CombiendTransformationMatrix값을 채운다.
void CMesh_Dynamic::Update_CombiendTransformationMatrices(D3DXFRAME_DERIVED* pFrame, _matrix matParent)
{
	pFrame->matCombinedTransformationMatrix = pFrame->TransformationMatrix * matParent;

	if (nullptr != pFrame->pFrameSibling)
		Update_CombiendTransformationMatrices(((D3DXFRAME_DERIVED*)pFrame->pFrameSibling), matParent);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_CombiendTransformationMatrices(((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild), pFrame->matCombinedTransformationMatrix);
}

void CMesh_Dynamic::SetUp_CombinedMatricesPointer(D3DXFRAME_DERIVED * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		m_MeshContainerList.push_back(pMeshContainer);

		for (_uint i = 0; i < pMeshContainer->dwNumBones; ++i)
		{

			const char* pName = pMeshContainer->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED*	pFrame_Find = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pName);

			pMeshContainer->ppCombinedTransformationMatrices[i] = &pFrame_Find->matCombinedTransformationMatrix;
		}
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CombinedMatricesPointer(((D3DXFRAME_DERIVED*)pFrame->pFrameSibling));

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CombinedMatricesPointer(((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild));
}

CMesh_Dynamic * CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Dynamic *	pInstance = new CMesh_Dynamic(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh_Dynamic(pFilePath, pFileName)))
	{
		MSG_BOX("CMesh_Dynamic Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Dynamic::Clone(void)
{
	return new CMesh_Dynamic(*this);
}

_ulong CMesh_Dynamic::Free(void)
{
	if (false == m_isClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
		m_MeshContainerList.clear();
	}

	Safe_Release(m_pAnimationCtrl);

	return CComponent::Free();
}
