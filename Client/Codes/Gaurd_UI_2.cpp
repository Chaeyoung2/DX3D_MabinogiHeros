#include "stdafx.h"
#include "..\Headers\Gaurd_UI_2.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Shader.h"

CGaurd_UI2::CGaurd_UI2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pShaderCom(nullptr)
{

}

HRESULT CGaurd_UI2::Ready_GameObject(_int iIndex)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fX = (float)g_iBackCX * 0.5f;
	m_fY = (float)g_iBackCY * 0.5f;
	m_fSizeX = 800.f;
	m_fSizeY = 138.f;

	m_iDrawID = iIndex;

	return NOERROR;
}

_int CGaurd_UI2::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;

	// 	system("cls");
	// 	cout << "m_fAlpheValue : " << m_fAlpheValue << endl;
	// 	cout << "m_fCreateTime : " << m_fCreateTime << endl;
	// 	cout << "m_bIsInit : " << m_bIsInit << endl;

	if (m_fAlpheValue > 0.f && m_bIsInit == true)
		m_fAlpheValue -= fTimeDelta /** 1.3f*/;

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);


	m_fCreateTime += fTimeDelta;

	if (m_fCreateTime >= 10.f)
	{
		m_fAlpheValue += fTimeDelta/* * 1.3f*/;
		m_bIsInit = false;
	}

	if (m_fAlpheValue >= 0.8f && m_bIsInit == false)
		m_bIsDead = true;

	if (m_bIsDead == true)
		return 1;

	return _int();
}

_int CGaurd_UI2::LastUpdate_GameObject(const _float & fTimeDelta)
{


	return _int();
}

void CGaurd_UI2::Render_GameObject(void)
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

HRESULT CGaurd_UI2::Ready_Component(void)
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

		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_UI_Gaurd_1");

	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();


	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_UI");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

HRESULT CGaurd_UI2::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	pEffect->SetFloat("g_fAlphaValue", m_fAlpheValue);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

	Safe_Release(pEffect);

	return NOERROR;
}

CGaurd_UI2 * CGaurd_UI2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CGaurd_UI2 *	pInstance = new CGaurd_UI2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(iIndex)))
	{
		MSG_BOX("CGaurd_UI2 Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CGaurd_UI2::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}
