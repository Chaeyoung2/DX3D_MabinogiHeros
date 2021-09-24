#include "stdafx.h"
#include "..\Headers\Map_Object.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CMap_Object::CMap_Object(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pCollisionCom(nullptr)
	, m_pShaderCom(nullptr)
{
}

HRESULT CMap_Object::Ready_GameObject(_vec3 vecScale, _vec3 vecRot, _int iOption, _int iDrawID, _vec3 VecPos)
{
	//m_fAngle = fAngle;
	m_iOption = iOption;
	m_iDrawID = iDrawID;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &VecPos);


	// 잔디가 너무 커..ㅜㅜ 스케일 조정해서 더 작게 해야 됨.
	if (iDrawID >= 41 && iDrawID <= 50)
	{
		m_pTransCom->Scaling(&_vec3(vecScale.x * 0.7f, vecScale.y * 0.7f, vecScale.z * 0.7f));
	}
	else if (iDrawID >= 38 && iDrawID <= 40)
	{
		m_pTransCom->Scaling(&_vec3(vecScale.x * 0.4f, vecScale.y * 0.4f, vecScale.z * 0.4f));
	}
	else
		m_pTransCom->Scaling(&vecScale);

	// 	m_pTransCom->RotationX(vecRot.x);
	m_pTransCom->RotationY(vecRot.y);
	// 	m_pTransCom->RotationZ(vecRot.z);

	return NOERROR;
}

_int CMap_Object::Update_GameObject(const _float & fTimeDelta)
{



	// 충돌.
	CCollision* pTargetCollision = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Collision", 0);

	//m_pCollisionCom->Collision_OBB(pTargetCollision);

	m_pTransCom->Invalidate_Worldmatrix();

	//if (true == m_isRender)
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	if (m_bIsDead == true)
		return 1;

	return _int();
}

_int CMap_Object::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pFrustumCom
		|| nullptr == m_pTransCom)
		return 0;

	//m_isRender = m_pFrustumCom->Culling_toFrustum(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION), 3.0f);

	return _int();
}

void CMap_Object::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// 	if (m_iDrawID >= 22 && m_iDrawID <= 50)
	// 	{
	// 
	// 	}
	// 	else
	// 	{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	SetUp_ConstantTable(pEffect);


	// 월드행렬을 장치에 셋팅한다.
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pMeshCom->Render_Mesh();

	pEffect->EndPass();
	pEffect->End();
	//	}


	if (g_bShowingCollider && nullptr != m_pCollisionCom)
		m_pCollisionCom->Render_Collider();
}


HRESULT CMap_Object::Ready_Component(void)
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
	switch (m_iDrawID)
	{
	case 0:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Box_00");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 1:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Box_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 2:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Box_02");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 3:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_CampFire");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 4:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Door_00");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 5:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Door_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 6:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Door_02");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 7:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Furniture_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 8:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Furniture_02");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 9:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_House_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 10:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_00");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	case 11:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));

	}
	break;
	case 12:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_02");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));

	}
	break;
	case 13:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_03");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));

	}
	break;
	case 14:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_04");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));

	}
	break;
	case 15:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_05");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 16:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_06");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));

	}
	break;
	case 17:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Pillar_07");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 18:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_FireRock");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 19:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Rock");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}

	case 20:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Ship");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 21:
	{
	}
	break;
	case 22:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_00");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 23:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 24:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_02");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 25:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_03");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 26:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_04");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 27:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_05");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 28:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_06");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 29:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_07");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	case 30:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_08");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	case 31:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_09");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 32:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_10");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 33:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_11");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 34:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_12");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 35:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_13");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 36:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_14");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 37:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Tree_15");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 38:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_00");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 39:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_01");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 40:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_02");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 41:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_03");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;

	case 42:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_04");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 43:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_05");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 44:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_06");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 45:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_07");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 46:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_08");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 47:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_09");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 48:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_10");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 49:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_11");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 50:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Weed_12");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 51:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Torch_0");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 52:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Torch_1");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	case 53:
	{
		pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Torch_2");
		if (nullptr == pComponent)
			return E_FAIL;
		m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));
	}
	break;
	default:
		break;
	}
	//pComponent->AddRef();


	// 	// For.Com_Collision
	// 	pComponent = m_pCollisionCom = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	// 	if (nullptr == pComponent)
	// 		return E_FAIL;
	// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	// 	pComponent->AddRef();
	// 	m_pCollisionCom->SetUp_Collision(CCollision::TYPE_AABB, m_pMeshCom, m_pTransCom, 0);

	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_MapObject");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	// For.Com_Frustum
	pComponent = m_pFrustumCom = (CFrustum*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Frustum");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Frustum", pComponent));
	pComponent->AddRef();


	return NOERROR;
}

HRESULT CMap_Object::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CMap_Object * CMap_Object::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vecScale, _vec3 vecRot, _int iOption, _int iDrawID, _vec3 VecPos)
{
	CMap_Object *	pInstance = new CMap_Object(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vecScale, vecRot, iOption, iDrawID, VecPos)))
	{
		MSG_BOX("CMap_Object Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMap_Object::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pCollisionCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pFrustumCom);

	return CGameObject::Free();
}
