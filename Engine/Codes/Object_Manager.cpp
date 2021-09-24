#include "..\Headers\Object_Manager.h"
#include "Layer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}


CComponent * CObject_Manager::Get_Component(const _uint& iSceneIdx, const _tchar * pLayerTag, const _tchar * pComponentTag, const _uint & iIndex)
{
	if (iSceneIdx >= m_iMaxSize)
		return nullptr;

	CLayer*	pLayer = Find_Layer(iSceneIdx, pLayerTag);

	return pLayer->Get_Component(pComponentTag, iIndex);	
}

HRESULT CObject_Manager::Reserve_ContainerSize(const _uint & iMaxSize)
{
	if (nullptr != m_pMapLayer)
		return E_FAIL;

	m_pMapLayer = new MAPLAYER[iMaxSize];

	m_iMaxSize = iMaxSize;

	return NOERROR;
}

HRESULT CObject_Manager::Add_GameObject(const _uint& iSceneIdx, const _tchar * pLayerTag, CGameObject * pGameObject)
{
	if (m_iMaxSize <= iSceneIdx
		|| nullptr == m_pMapLayer)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iSceneIdx, pLayerTag);

	if (nullptr == pLayer)
	{
		CLayer*	pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);

		m_pMapLayer[iSceneIdx].insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}
	else	
		pLayer->Add_GameObject(pGameObject);
	
	return NOERROR;
}

_int CObject_Manager::Update_ObjectMgr(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	for (size_t i = 0; i < m_iMaxSize; ++i)
	{
		auto iter_begin = m_pMapLayer[i].begin();
		auto iter_end = m_pMapLayer[i].end();

		for (; iter_begin != iter_end; )
		{
			iExitCode = (*iter_begin).second->Update_Layer(fTimeDelta);
			if (iExitCode == 1)
			{
				iter_begin = m_pMapLayer[i].erase(iter_begin);

				if (iExitCode == 0)
				{
					iter_begin = m_pMapLayer[i].erase(iter_begin);
				}
			}
			else
				++iter_begin;
		}
	}

	return iExitCode;

// 	_int		iExitCode = 0;
// 
// 	for (size_t i = 0; i < m_iMaxSize; ++i)
// 	{
// 		for (auto& Pair : m_pMapLayer[i])
// 		{
// 			if (iExitCode = Pair.second->Update_Layer(fTimeDelta))
// 				return iExitCode;
// 		}
// 	}
// 	
// 	return iExitCode;
}

_int CObject_Manager::LastUpdate_ObjectMgr(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxSize; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			if (iExitCode = Pair.second->LastUpdate_Layer(fTimeDelta))
				return iExitCode;
		}
	}

	return iExitCode;

}

void CObject_Manager::Release_GameObject(const _uint & iSceneIdx)
{
	for (auto& Pair : m_pMapLayer[iSceneIdx])
	{
		Safe_Release(Pair.second);
	}
	m_pMapLayer[iSceneIdx].clear();
}

CLayer * CObject_Manager::Find_Layer(const _uint& iSceneIdx, const _tchar * pLayerTag)
{
	if (m_iMaxSize <= iSceneIdx)
		return nullptr;

	auto iter = find_if(m_pMapLayer[iSceneIdx].begin(), m_pMapLayer[iSceneIdx].end(), CFind_Tag(pLayerTag));

	if(iter == m_pMapLayer[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

_ulong CObject_Manager::Free(void)
{
	_ulong dwRefCnt = 0;

	for (size_t i = 0; i < m_iMaxSize; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			if (dwRefCnt = Safe_Release(Pair.second))
				return dwRefCnt;
		}
		m_pMapLayer[i].clear();
	}

	Safe_Delete_Array(m_pMapLayer);

	return dwRefCnt;
}
