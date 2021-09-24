#include "..\Headers\Component_Manager.h"
#include "Component.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}


CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	CComponent* pComponent = Find_Component(iSceneIdx, pComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone();
}

HRESULT CComponent_Manager::Add_Component(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent * pComponent)
{
	if (nullptr == m_pMapComponent)	
		return E_FAIL;

	if (iSceneIdx >= m_iReserve_Size)
		return E_FAIL;

	if (nullptr != Find_Component(iSceneIdx, pComponentTag))
		return E_FAIL;

	m_pMapComponent[iSceneIdx].insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	return NOERROR;
}

HRESULT CComponent_Manager::Reserve_ContainerSize(const _uint & iSceneCnt)
{
	if (nullptr != m_pMapComponent)
		return E_FAIL;

	m_pMapComponent = new MAPCOMPONENT[iSceneCnt];

	m_iReserve_Size = iSceneCnt;

	return NOERROR;
}

void CComponent_Manager::Release_Component(const _uint & iSceneIdx)
{
	for (auto& Pair : m_pMapComponent[iSceneIdx])
	{
		Safe_Release(Pair.second);
	}
	m_pMapComponent[iSceneIdx].clear();
}

CComponent * CComponent_Manager::Find_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	auto iter = find_if(m_pMapComponent[iSceneIdx].begin(), m_pMapComponent[iSceneIdx].end(), CFind_Tag(pComponentTag));

	if(iter == m_pMapComponent[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

_ulong CComponent_Manager::Free(void)
{
	_ulong dwRefCnt = 0;

	for (size_t i = 0; i < m_iReserve_Size; ++i)
	{
		for (auto& Pair : m_pMapComponent[i])
		{
			if (dwRefCnt = Safe_Release(Pair.second))
				return dwRefCnt;
		}
		m_pMapComponent[i].clear();
	}

	Safe_Delete_Array(m_pMapComponent);

	return dwRefCnt;
}
