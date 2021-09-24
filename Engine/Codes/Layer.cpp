#include "..\Headers\Layer.h"
#include "GameObject.h"

USING(Engine)

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, const _uint & iIndex)
{
	auto iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);	
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

HRESULT CLayer::Ready_Layer(void)
{
	return NOERROR;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (iExitCode = pGameObject->Update_GameObject(fTimeDelta))
			return iExitCode;		
	}

	return iExitCode;
}

_int CLayer::LastUpdate_Layer(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (iExitCode = pGameObject->LastUpdate_GameObject(fTimeDelta))
			return iExitCode;
	}

	return iExitCode;
}

CLayer * CLayer::Create(void)
{
	CLayer *	pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		MSG_BOX("CLayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CLayer::Free(void)
{
	_ulong dwRefCnt = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (dwRefCnt = Safe_Release(pGameObject))
			return dwRefCnt;
	}

	m_ObjectList.clear();

	return dwRefCnt;
}


