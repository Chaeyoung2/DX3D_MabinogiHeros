#include "..\Headers\Cell.h"

USING(Engine)

CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CCell::MOVETYPE CCell::GetMoveType(const _vec3 & vMovedPos, _uint* pCurrentIdx)
{
	for (size_t i = 0; i < LINE_END; ++i)
	{
		_vec3 vTmp = vMovedPos - m_vPoint[i];
		_vec2 vSrcDir = _vec2(vTmp.x, vTmp.z);

		D3DXVec2Normalize(&vSrcDir, &vSrcDir);

		// 빨간거랑 노말에 각이 0~90사이거나 270~360도 사이다.
		if (0 < D3DXVec2Dot(&vSrcDir, &m_vNormal[i]))
		{
			if (nullptr != m_pNeighbor[i])
			{
				*pCurrentIdx = m_pNeighbor[i]->m_iIndex;
				return MOVE_TO_NEIGHBOR;
			}
			else
				return MOVE_OUT;
		}
	}
	return MOVE_IN;
}

_vec3 CCell::GetWallNormal(const _vec3 & vMovedPos, _uint * pCurrentIdx)
{
	for (size_t i = 0; i < LINE_END; ++i)
	{
		_vec3 vTmp = vMovedPos - m_vPoint[i]; // 플레이어가 들어온 방향 벡터.
		_vec2 vSrcDir = _vec2(vTmp.x, vTmp.z);

		D3DXVec2Normalize(&vSrcDir, &vSrcDir);


		// 빨간거랑 노말에 각이 0~90사이거나 270~360도 사이다.
		if (0 < D3DXVec2Dot(&vSrcDir, &m_vNormal[i]))
		{
			return m_vNormal[i];
		}
	}
	return _vec3(0.f, 0.f, 0.f);
}

HRESULT CCell::Ready_Cell( _vec3& pPointA,  _vec3& pPointB,  _vec3& pPointC, const _uint& iIndex)
{
	m_iIndex = iIndex;

	m_vPoint[POINT_A] = pPointA;
	m_vPoint[POINT_B] = pPointB;
	m_vPoint[POINT_C] = pPointC;

	m_vDir[LINE_AB] = m_vPoint[POINT_B] - m_vPoint[POINT_A];
	m_vDir[LINE_BC] = m_vPoint[POINT_C] - m_vPoint[POINT_B];
	m_vDir[LINE_CA] = m_vPoint[POINT_A] - m_vPoint[POINT_C];

	m_vNormal[LINE_AB] = _vec2(m_vDir[LINE_AB].z * -1.f, m_vDir[LINE_AB].x);
	m_vNormal[LINE_BC] = _vec2(m_vDir[LINE_BC].z * -1.f, m_vDir[LINE_BC].x);
	m_vNormal[LINE_CA] = _vec2(m_vDir[LINE_CA].z * -1.f, m_vDir[LINE_CA].x);

	for (size_t i = 0; i < LINE_END; ++i)
		D3DXVec2Normalize(&m_vNormal[i], &m_vNormal[i]);

	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))
		return E_FAIL;

	return NOERROR;
}

void CCell::Render_Cell(void)
{
	_vec3		vPoint[4] =
	{
		m_vPoint[0],
		m_vPoint[1],
		m_vPoint[2],
		m_vPoint[0],
	};

	m_pLine->SetWidth(3.f);

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (size_t i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if (vPoint[i].z <= 0)
			vPoint[i].z = 0;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	_matrix			matTmp;

	m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTmp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

_bool CCell::Check_Neighbor(_vec3 vSrcPoint, _vec3 vDestPoint, CCell* pCell)
{
	if (m_vPoint[POINT_A] == vSrcPoint)
	{
		if (m_vPoint[POINT_B] == vDestPoint)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (m_vPoint[POINT_C] == vDestPoint)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == vSrcPoint)
	{
		if (m_vPoint[POINT_A] == vDestPoint)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (m_vPoint[POINT_C] == vDestPoint)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == vSrcPoint)
	{
		if (m_vPoint[POINT_A] == vDestPoint)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}

		if (m_vPoint[POINT_B] == vDestPoint)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	return _bool(false);
}

CCell * CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& pPointA, _vec3&  pPointB, _vec3& pPointC, const _uint& iIndex)
{
	CCell *	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(pPointA, pPointB, pPointC, iIndex)))
	{
		MSG_BOX("CCell Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CCell::Free(void)
{
	Safe_Release(m_pLine);
	Safe_Release(m_pGraphicDev);

	return _ulong();
}
