#include "stdafx.h"
#include "..\Headers\ZecallionAxe.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CZecallionAxe::CZecallionAxe(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pBoneMatrix_Parent(nullptr)
	, m_pWorldMatrix_Parent(nullptr)
{
}

HRESULT CZecallionAxe::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	CMesh_Dynamic* pMeshCom_Zecallion =
		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Mesh", 0));
	m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Anim_Attachment_RH");


	CTransform* pTransCom_Zecallion = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Transform", 0));
	m_pWorldMatrix_Parent = pTransCom_Zecallion->Get_WorldMatrix();

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, (_vec3*)&m_pBoneMatrix_Parent->m[3][0]);

	//m_pTransCom->RotationX(D3DXToRadian(180.f));
	m_pTransCom->RotationY(D3DXToRadian(270.f));
	//m_pTransCom->RotationZ(D3DXToRadian(180.f));

	return NOERROR;
}

_int CZecallionAxe::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom)
		return -1;

	m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
	
	CCollision* pTargetCollision = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Collision_Front", 0);

	m_pCollision->Collision_OBB(pTargetCollision);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

// 	cout << "Axe Pos In Collision Function : " << (m_pTransCom->Get_Infomation(CTransform::INFO_POSITION))->x << "," << (m_pTransCom->Get_Infomation(CTransform::INFO_POSITION))->y << "," << (m_pTransCom->Get_Infomation(CTransform::INFO_POSITION))->z << endl;


	return _int();
}

_int CZecallionAxe::LastUpdate_GameObject(const _float & fTimeDelta)
{

	return _int();
}

void CZecallionAxe::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// 월드행렬을 장치에 셋팅한다.
	m_pTransCom->SetUp_OnGraphicDev();

	m_pMeshCom->Render_Mesh();
	
	if(g_bShowingCollider)
		m_pCollision->Render_Collider();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CZecallionAxe::Ready_Component(void)
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
	pComponent = m_pMeshCom = (CMesh_Dynamic*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_ZecallionAxe");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	pComponent->AddRef();

	// For.Com_Collision
	pComponent = m_pCollision = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_Axe", pComponent));
	pComponent->AddRef();
	m_pCollision->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom, 0, _vec3(1.f, 1.f, 0.4f), _vec3(0.f, 0.f, -120.f));




	return NOERROR;
}

CZecallionAxe * CZecallionAxe::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CZecallionAxe *	pInstance = new CZecallionAxe(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CSword Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CZecallionAxe::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pCollision);

	return CGameObject::Free();
}
