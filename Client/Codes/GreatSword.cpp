#include "stdafx.h"
#include "..\Headers\GreatSword.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CGreatSword::CGreatSword(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pBoneMatrix_Parent(nullptr)
	, m_pWorldMatrix_Parent(nullptr)
{
}

HRESULT CGreatSword::Ready_GameObject(void)
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

_int CGreatSword::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom)
		return -1;

	m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);


	return _int();
}

_int CGreatSword::LastUpdate_GameObject(const _float & fTimeDelta)
{

	return _int();
}

void CGreatSword::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// 월드행렬을 장치에 셋팅한다.
	m_pTransCom->SetUp_OnGraphicDev();

	m_pMeshCom->Render_Mesh();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CGreatSword::Ready_Component(void)
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
	pComponent = m_pMeshCom = (CMesh_Dynamic*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_GreatSword");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	pComponent->AddRef();





	return NOERROR;
}

CGreatSword * CGreatSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGreatSword *	pInstance = new CGreatSword(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CSword Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CGreatSword::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pMeshCom);

	return CGameObject::Free();
}
