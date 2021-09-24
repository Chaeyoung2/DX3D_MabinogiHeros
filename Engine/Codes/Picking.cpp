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
	// ���콺 ��ǥ�� ������.
	POINT			ptMouse;
	GetCursorPos(&ptMouse);

	// ����Ʈ�������� ��ǥ�� ���ߴ�.
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	// ���������� ���� ��ǥ�� ��ȯ�ߴ�.

	_vec3			vMousePos;
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) -1;
	vMousePos.y = ptMouse.y / (ViewPort.Height * 0.5f * -1.f) + 1;
	vMousePos.z = 0.0f;

	// �佺���̽��� ��������.
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRayPos, vRayDir;
	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// ���彺���̽��� ��������.
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

			// ��.��
			// fU : 0��° ������ 1��° ������ �Ÿ��� 1�κ������� �浹�� ������ ��ʰ�
			// fV : 0��° ������ 2��° ������ �Ÿ��� 1�κ������� �浹�� ������ ��ʰ�
			if (TRUE == D3DXIntersectTri(&pVertices[iVertexIdx[1]], &pVertices[iVertexIdx[0]], &pVertices[iVertexIdx[2]]
				, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return iIndex;
				//return pVertices[iVertexIdx[1]] + (pVertices[iVertexIdx[0]] - pVertices[iVertexIdx[1]]) * fU + (pVertices[iVertexIdx[2]] - pVertices[iVertexIdx[1]]) * fV;
			}


			// ��.��
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
	// ���콺 ��ǥ�� ������.
	POINT			ptMouse;
	GetCursorPos(&ptMouse);

	// ����Ʈ�������� ��ǥ�� ���ߴ�.
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	// ���������� ���� ��ǥ�� ��ȯ�ߴ�.

	_vec3			vMousePos;
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1;
	vMousePos.y = ptMouse.y / (ViewPort.Height * 0.5f * -1.f) + 1;
	vMousePos.z = 0.0f;

	// �佺���̽��� ��������.
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRayPos, vRayDir;
	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// ���彺���̽��� ��������.
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

			// ��.��
			// fU : 0��° ������ 1��° ������ �Ÿ��� 1�κ������� �浹�� ������ ��ʰ�
			// fV : 0��° ������ 2��° ������ �Ÿ��� 1�κ������� �浹�� ������ ��ʰ�
			// fDist : vRayPos�κ��� �浹�� ���������� �Ÿ�.
			if (TRUE == D3DXIntersectTri(&pVertices[iVertexIdx[1]], &pVertices[iVertexIdx[0]], &pVertices[iVertexIdx[2]]
				, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return pVertices[iVertexIdx[1]] + (pVertices[iVertexIdx[0]] - pVertices[iVertexIdx[1]]) * fU + (pVertices[iVertexIdx[2]] - pVertices[iVertexIdx[1]]) * fV;
			}


			// ��.��
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
