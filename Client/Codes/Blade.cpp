#include "stdafx.h"
#include "..\Headers\Blade.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CBlade::CBlade(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pBoneMatrix_Parent(nullptr)
	, m_pWorldMatrix_Parent(nullptr)
	, m_pCollisionCom(nullptr)
	, m_pShaderCom(nullptr)
{
}

HRESULT CBlade::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	CMesh_Dynamic* pMeshCom_Player =
		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh", 0));

	m_pBoneMatrix_Parent = pMeshCom_Player->Get_BoneMatrix("ValveBiped_Anim_Attachment_RH");


	CTransform* pTransCom_Player = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
	m_pWorldMatrix_Parent = pTransCom_Player->Get_WorldMatrix();


	//m_pTransCom->RotationX(D3DXToRadian(180.f));
	m_pTransCom->RotationY(D3DXToRadian(270.f));
	//m_pTransCom->RotationZ(D3DXToRadian(180.f));

	return NOERROR;
}

_int CBlade::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom)
		return -1;

	m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);


	return _int();
}

_int CBlade::LastUpdate_GameObject(const _float & fTimeDelta)
{

	return _int();
}

void CBlade::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pTransCom->SetUp_OnGraphicDev();

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh();

	pEffect->EndPass();
	pEffect->End();


	if(g_bShowingCollider)
		m_pCollisionCom->Render_Collider();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CBlade::Ready_Component(void)
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


	// For.Com_Mesh
	pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Blade");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	pComponent->AddRef();


	// For.Com_Collision
	pComponent = m_pCollisionCom = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom->SetUp_Collision(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom, 0);

	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

HRESULT CBlade::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransCom->Get_WorldMatrix());

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pEffect->SetMatrix("g_matView", &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(1.f, -1.f, 1.f, 0.f));

	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

CBlade * CBlade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlade *	pInstance = new CBlade(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CBlade Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBlade::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pCollisionCom);

	return CGameObject::Free();
}
