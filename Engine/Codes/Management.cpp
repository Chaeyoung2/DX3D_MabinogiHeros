#include "..\Headers\Management.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr)
{
}



HRESULT CManagement::SetUp_CurrentScene(CScene * pNewScene)
{
	_ulong dwRefCnt = 0;

	if (nullptr != m_pScene)
	{
		if (dwRefCnt = Safe_Release(m_pScene))
			return E_FAIL;
	}

	m_pScene = pNewScene;

	return NOERROR;
}

_int CManagement::Update_CurrentScene(const _float & fTimeDelta)
{	
	if (nullptr == m_pScene)
		return -1;

	_int		iExitCode = 0;

	if (iExitCode = m_pScene->Update_Scene(fTimeDelta))
		return iExitCode;

	if (iExitCode = m_pScene->LastUpdate_Scene(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

void CManagement::Render_CurrentScene(void)
{
	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

_ulong CManagement::Free(void)
{
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pScene))
	{
		MSG_BOX("Currrent Scene Release Failed");
		return dwRefCnt;
	}
	
	return dwRefCnt;
}
