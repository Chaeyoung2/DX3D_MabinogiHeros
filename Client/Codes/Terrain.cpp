#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "Component_Manager.h"
#include "Input_Device.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pPickingCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pFilterCom(nullptr)
	, m_pTextureCom_1(nullptr)
	, m_pTextureCom_3(nullptr)
	, m_pFrustumCom(nullptr)
{

}

HRESULT CTerrain::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	
// 	ZeroMemory(&m_MtrlInfo, sizeof(D3DMATERIAL9));
// 
// 	m_MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
// 	m_MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
// 	m_MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);


	//m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(0.0f, 0.0f, 5.0f));

	return NOERROR;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (CInput_Device::Get_Instance()->Get_DIMState(CInput_Device::DIM_LBUTTON) & 0x80)
	{
// 		_vec3		vTmp = m_pPickingCom->Picking_OnRectBuffer(g_hWnd, m_pBufferCom->Get_VertexPos(), get<0>(m_pBufferCom->Get_BufferInfomation()), get<1>(m_pBufferCom->Get_BufferInfomation()));
// 		int a = 10;
	}
	
	m_pTransCom->Invalidate_Worldmatrix();
	
	//Optimize_Buffer();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);


	return _int();
}

_int CTerrain::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CTerrain::Render_GameObject(void)
{
	if (nullptr == m_pShaderCom)
		return;

	// 셰이더용 이펙트 객체.
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	// 셰이더 컨스턴트 테이블 세팅.
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	// 월드행렬을 장치에 셋팅한다.
// 	m_pTransCom->SetUp_OnGraphicDev();
// 	m_pTextureCom->SetUp_OnGraphicDev(0, 0);
// 	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
}

HRESULT CTerrain::Ready_Component(void)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	// For.Com_Picking
	pComponent = m_pPickingCom = (CPicking*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Picking");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Picking", pComponent));
	pComponent->AddRef();


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
	pComponent = m_pBufferCom = (CTerrain_Buffer*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Buffer_Terrain");
	if (nullptr == pComponent)
		return E_FAIL;		
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	pComponent->AddRef();

	// For.Com_Texture
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();
	// For.Com_Texture1
	pComponent = m_pTextureCom_1 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain_1");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();
	// For.Com_Texture3
	pComponent = m_pTextureCom_3 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain_3");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	// For.Com_Texture_Effect
	pComponent = m_pEffectCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Resonance_0");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Terrain");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	// For.Com_Filter
	pComponent = m_pFilterCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain_Filter");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Filter", pComponent));
	pComponent->AddRef();

	// For.Com_Frustum
	pComponent = m_pFrustumCom = (CFrustum*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Frustum");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Frustum", pComponent));
	pComponent->AddRef();


	return NOERROR;
}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransCom->Get_WorldMatrix());

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pEffect->SetMatrix("g_matView", &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");
	m_pTextureCom_1->SetUp_OnShader(pEffect, "g_MixTexture_1");
	m_pTextureCom_3->SetUp_OnShader(pEffect, "g_MixTexture_3");
	m_pFilterCom->SetUp_OnShader(pEffect, "g_FilterTexture");
	//m_pEffectCom->SetUp_OnShader(pEffect, "g_BrushTexture");

	pEffect->SetVector("g_vLightDir", &_vec4(1.f, -1.f, 1.f, 0.f));

	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);


	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CTerrain::Optimize_Buffer(void)
{
	if (nullptr == m_pBufferCom
		|| nullptr == m_pFrustumCom)
		return E_FAIL;

	if (FAILED(m_pBufferCom->Optimize_Buffer(m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain *	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTerrain::Free(void)
{
	Engine::Safe_Release(m_pPickingCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pEffectCom);
	Engine::Safe_Release(m_pTextureCom_1);
	Engine::Safe_Release(m_pTextureCom_3);
	Engine::Safe_Release(m_pFrustumCom);


	return CGameObject::Free();
}
