#include "..\Headers\Picking.h"

USING(Engine)

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CPicking::CPicking(const CPicking & rhs)
	: CComponent(rhs)
{

}

HRESULT CPicking::Ready_Picking(void)
{


	return NOERROR;
}

_int CPicking::Picking_OnRectBuffer(HWND hWnd, const _vec3* pVertices, const _uint& iCntX, const _uint& iCntZ)
{
	// 마우스 좌표를 구하자.
	POINT			ptMouse;
	GetCursorPos(&ptMouse);

	// 뷰포트영역상의 좌표를 구했다.
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	// 투영윈도우 상의 좌표로 변환했다.

	_vec3			vMousePos;
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) -1;
	vMousePos.y = ptMouse.y / (ViewPort.Height * 0.5f * -1.f) + 1;
	vMousePos.z = 0.0f;

	// 뷰스페이스로 데려간다.
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRayPos, vRayDir;
	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// 월드스페이스로 데려간다.
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_float		fU, fV, fDist;

	for (size_t i = 0; i < iCntZ - 1; i++)
	{
		for (size_t j = 0; j < iCntX - 1; j++)
		{
			_uint		iIndex = i * iCntX + j;

			_uint		iVertexIdx[] =
			{
				iIndex + iCntX,
				iIndex + iCntX + 1,
				iIndex + 1,
				iIndex
			};

			// 오.위
			// fU : 0번째 정점과 1번째 정점의 거리를 1로보았을때 충돌된 지점의 비례값
			// fV : 0번째 정점과 2번째 정점의 거리를 1로보았을때 충돌된 지점의 비례값
			if (TRUE == D3DXIntersectTri(&pVertices[iVertexIdx[1]], &pVertices[iVertexIdx[0]], &pVertices[iVertexIdx[2]]
				, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return iIndex;
				//return pVertices[iVertexIdx[1]] + (pVertices[iVertexIdx[0]] - pVertices[iVertexIdx[1]]) * fU + (pVertices[iVertexIdx[2]] - pVertices[iVertexIdx[1]]) * fV;
			}


			// 왼.아
			if (TRUE == D3DXIntersectTri(&pVertices[iVertexIdx[3]], &pVertices[iVertexIdx[2]], &pVertices[iVertexIdx[0]]
				, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				//return pVertices[iVertexIdx[3]] + (pVertices[iVertexIdx[2]] - pVertices[iVertexIdx[3]]) * fU + (pVertices[iVertexIdx[0]] - pVertices[iVertexIdx[3]]) * fV;
				return iIndex;
			}
		}

	}
	
	return -1;
}

_vec3 CPicking::Picking_OnRectBuffer_ReturnVec3(HWND hWnd, const _vec3 * pVertices, const _uint & iCntX, const _uint & iCntZ)
{
	// 마우스 좌표를 구하자.
	POINT			ptMouse;
	GetCursorPos(&ptMouse);

	// 뷰포트영역상의 좌표를 구했다.
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	// 투영윈도우 상의 좌표로 변환했다.

	_vec3			vMousePos;
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1;
	vMousePos.y = ptMouse.y / (ViewPort.Height * 0.5f * -1.f) + 1;
	vMousePos.z = 0.0f;

	// 뷰스페이스로 데려간다.
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRayPos, vRayDir;
	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// 월드스페이스로 데려간다.
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_float		fU, fV, fDist;

	for (size_t i = 0; i < iCntZ - 1; i++)
	{
		for (size_t j = 0; j < iCntX - 1; j++)
		{
			_uint		iIndex = i * iCntX + j;

			_uint		iVertexIdx[] =
			{
				iIndex + iCntX,
				iIndex + iCntX + 1,
				iIndex + 1,
				iIndex
			};

			// 오.위
			// fU : 0번째 정점과 1번째 정점의 거리를 1로보았을때 충돌된 지점의 비례값
			// fV : 0번째 정점과 2번째 정점의 거리를 1로보았을때 충돌된 지점의 비례값
			// fDist : vRayPos로부터 충돌된 지점까지의 거리.
			if (TRUE == D3DXIntersectTri(&pVertices[iVertexIdx[1]], &pVertices[iVertexIdx[0]], &pVertices[iVertexIdx[2]]
				, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return pVertices[iVertexIdx[1]] + (pVertices[iVertexIdx[0]] - pVertices[iVertexIdx[1]]) * fU + (pVertices[iVertexIdx[2]] - pVertices[iVertexIdx[1]]) * fV;
			}


			// 왼.아
			if (TRUE == D3DXIntersectTri(&pVertices[iVertexIdx[3]], &pVertices[iVertexIdx[2]], &pVertices[iVertexIdx[0]]
				, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return pVertices[iVertexIdx[3]] + (pVertices[iVertexIdx[2]] - pVertices[iVertexIdx[3]]) * fU + (pVertices[iVertexIdx[0]] - pVertices[iVertexIdx[3]]) * fV;
			}

		}

	}

	return _vec3(0.0f, 0.0f, 0.0f);
}

CComponent * CPicking::Clone(void)
{
	return new CPicking(*this);
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPicking *	pInstance = new CPicking(pGraphicDev);

	if (FAILED(pInstance->Ready_Picking()))
	{
		MSG_BOX("CPicking Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CPicking::Free(void)
{
	return CComponent::Free();
}
