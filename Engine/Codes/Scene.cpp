#include "..\Headers\Scene.h"
#include "Object_Manager.h"
#include "Component_Manager.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pRenderer(nullptr)
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
{
	m_pGraphicDev->AddRef();
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();

}

HRESULT CScene::Ready_Scene(const _uint& eSceneID, const _tchar* pRendererTag)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pRenderer = (CRenderer*)m_pComponent_Manager->Clone_Component(eSceneID, pRendererTag);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return NOERROR;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;

	if (iExitCode = m_pObject_Manager->Update_ObjectMgr(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

_int CScene::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;

	if (iExitCode = m_pObject_Manager->LastUpdate_ObjectMgr(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

void CScene::Render_Scene(void)
{
	if (nullptr == m_pRenderer)
		return;

	m_pRenderer->Render_Renderer();

}

_ulong CScene::Free(void)
{
	_ulong dwRefCnt = 0;


	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pGraphicDev);


	return dwRefCnt;
}
