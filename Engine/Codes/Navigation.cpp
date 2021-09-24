#include "..\Headers\Navigation.h"
#include "Cell.h"

USING(Engine)

CNavigation::CNavigation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_iNumCell(rhs.m_iNumCell)
{
	for (auto& pCell : m_vecCell)
		pCell->AddRef();
}

HRESULT CNavigation::Ready_Navigation(const _tchar * pFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_uint		iIndex = 0;

	while (true)
	{
		_vec3	vPoint[3];
		CELLPOINT* pCellPoint = new CELLPOINT;

//		ReadFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
		ReadFile(hFile, pCellPoint, sizeof(CELLPOINT), &dwByte, NULL);


		if (0 == dwByte)
			break;

//		CCell*	 pCell = CCell::Create(m_pGraphicDev, vPoint, vPoint + 1, vPoint + 2, iIndex++);
		CCell*	 pCell = CCell::Create(m_pGraphicDev, 
			pCellPoint->vPoint_First, pCellPoint->vPoint_Second, pCellPoint->vPoint_Third, iIndex++);
		if (nullptr == pCell)
			return E_FAIL;

		m_vecCell.push_back(pCell);
	}

	m_iNumCell = m_vecCell.size();

	Make_Neighbor();

	return NOERROR;
}

CCell::MOVETYPE CNavigation::isMoving(const _vec3 * pPosition, const _vec3 * pDirection)
{
	_vec3			vMovedPos = *pPosition + *pDirection;

	return m_vecCell[m_iCurrentCell]->GetMoveType(vMovedPos, &m_iCurrentCell);
}

_vec3 CNavigation::GetWallNormal(const _vec3 * pPosition, const _vec3 * pDirection)
{
	_vec3			vMovedPos = *pPosition + *pDirection;

	return m_vecCell[m_iCurrentCell]->GetWallNormal(vMovedPos, &m_iCurrentCell);
}

void CNavigation::Render_Navigation(void)
{
	for (auto& pCell : m_vecCell)
	{
		pCell->Render_Cell();
	}
}

HRESULT CNavigation::Make_Neighbor(void)
{
	// 이웃들을 셋팅하는 작업을 하자
	for (size_t i = 0; i < m_iNumCell; i++)
	{
		for (size_t j = 0; j < m_iNumCell; j++)
		{
			if (m_vecCell[i] == m_vecCell[j])
				continue;

			if (true == m_vecCell[j]->Check_Neighbor(m_vecCell[i]->Get_Point(CCell::POINT_A), m_vecCell[i]->Get_Point(CCell::POINT_B), m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
			}

			if (true == m_vecCell[j]->Check_Neighbor(m_vecCell[i]->Get_Point(CCell::POINT_B), m_vecCell[i]->Get_Point(CCell::POINT_C), m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
			}

			if (true == m_vecCell[j]->Check_Neighbor(m_vecCell[i]->Get_Point(CCell::POINT_C), m_vecCell[i]->Get_Point(CCell::POINT_A), m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
			}
		}
	}

	return NOERROR;
}

CComponent * CNavigation::Clone(void)
{
	return new CNavigation(*this);
}

CNavigation * CNavigation::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath)
{
	CNavigation *	pInstance = new CNavigation(pGraphicDev);

	if (FAILED(pInstance->Ready_Navigation(pFilePath)))
	{
		MSG_BOX("CNavigation Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CNavigation::Free(void)
{
	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);
	m_vecCell.clear();

	return CComponent::Free();
}
