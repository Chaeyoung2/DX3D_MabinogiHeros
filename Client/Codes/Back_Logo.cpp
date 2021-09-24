#include "stdafx.h"
#include "..\Headers\Back_Logo.h"
#include "Component_Manager.h"

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CBack_Logo::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(0.0f, 0.0f, 5.0f));
	//m_pTransCom->Scaling(&_vec3(1024.f, 768.f, 1.f));

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_PRIORITY, this);

	return _int();
}

_int CBack_Logo::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CBack_Logo::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 월드행렬을 장치에 셋팅한다.	
	//m_pTransCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, 0);

	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, g_iBackCX, g_iBackCY, 1.f);
	D3DXMatrixTranslation(&matTrans, g_iBackCX * 0.5f, g_iBackCY * 0.5f, 1.f);

	m_pBufferCom->Render_Buffer(&(matScale * matTrans));

	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
// 	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
// 	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
// 
// 	// 월드행렬을 장치에 셋팅한다.
// 	m_pTransCom->SetUp_OnGraphicDev();
// 
// 	m_pTextureCom->SetUp_OnGraphicDev(0, 0);
// 	m_pBufferCom->Render_Buffer();
// 
// 	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
// 	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CBack_Logo::Ready_Component(void)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	// For.Com_Renderer
	pComponent = m_pRendererCom = (CRenderer*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	pComponent->AddRef();


	// For.Com_Transform
	pComponent = m_pTransCom = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();


	// For.Com_Buffer
	pComponent = m_pBufferCom = (CScreen_Texture*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_Screen_Texture");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	pComponent->AddRef();

	// For.Com_Texture
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Texture_Logo");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBack_Logo *	pInstance = new CBack_Logo(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBack_Logo::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	return CGameObject::Free();
}
