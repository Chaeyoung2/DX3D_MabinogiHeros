#include "stdafx.h"
#include "..\Headers\UI.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Shader.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pShaderCom(nullptr)
{

}

HRESULT CUI::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fX = 100.0f;
	m_fY = 100.0f;
	m_fSizeX = 200.0f;
	m_fSizeY = 200.0f;

	//m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(rand() % 10, 7.0f, rand() % 10));

	return NOERROR;
}

_int CUI::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return _int();
}

_int CUI::LastUpdate_GameObject(const _float & fTimeDelta)
{


	return _int();
}

void CUI::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CUI::Ready_Component(void)
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
	pComponent = m_pBufferCom = (CRect_Texture*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTexture");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	pComponent->AddRef();

	// For.Com_Texture
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Texture_Sample");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Shader_Default");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;
	D3DXMatrixIdentity(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matView, matProj;
	D3DXMatrixIdentity(&matView);


	matView._11 = m_fSizeX;
	matView._22 = m_fSizeY;
	matView._41 = m_fX - g_iBackCX * 0.5f;
	matView._42 = -m_fY + g_iBackCY * 0.5f;

	pEffect->SetMatrix("g_matView", &matView);

	// w, h범위에 있는 놈들을 -1, 1 ~ 1, -1안으로 옮겨준다.
	D3DXMatrixOrthoLH(&matProj, g_iBackCX, g_iBackCY, 0.0f, 1.f);

	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

	Safe_Release(pEffect);

	return NOERROR;
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI *	pInstance = new CUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CUI Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CUI::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}
