#include "stdafx.h"
#include "..\Headers\Effect_Hit.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CEffect_Hit::CEffect_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom_0(nullptr)
	, m_pTextureCom_1(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CEffect_Hit::Ready_GameObject(ZECALLION_BONE eBoneID, _vec3 vBloodPosition, _int iDrawID, _vec3 vecAngle)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eBoneID = eBoneID;
	m_iDrawID = iDrawID;


	CMesh_Dynamic* pMeshCom_Zecallion =
		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Mesh", 0));

	switch (m_eBoneID)
	{
	case BONE_LEFT_CALF:
		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_L_Calf");
		break;
	case BONE_RIGHT_CALF:
		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_R_Calf");
		break;
	case BONE_LEFT_HAND:
		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_L_Hand");
		break;
	case BONE_RIGHT_HAND:
		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_R_Hand");
		break;
	case BONE_FRONT:
		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("jiggle_zecallion_front");
		break;
	}

	CTransform* pTransCom_Zecallion = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Transform", 0));
	m_pWorldMatrix_Parent = pTransCom_Zecallion->Get_WorldMatrix();


	m_pTransCom->Scaling(&_vec3(10.f, 10.f, 1.f));
	m_pTransCom->RotationX(vecAngle.x); m_pTransCom->RotationY(vecAngle.y); m_pTransCom->RotationZ(vecAngle.z);

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(vBloodPosition.x, vBloodPosition.y, vBloodPosition.z ));

	return NOERROR;
}

_int CEffect_Hit::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;

	// 	m_fCreateTime += fTimeDelta;
	// 	if (m_fCreateTime >= 1.5f)
	// 	{
	// 		return 1;
	// 	}

	// 	system("cls");
	// 	cout << m_fFrame << endl;

	/*	m_fFrame += (fTimeDelta * 10.f);*/
// 	m_fFrame++;
// 	if (m_fFrame >= m_fMaxFrame)
// 	{
// 		m_fFrame = 0.f;
// 		return 1;
// 	}


	SetUp_BillBoard();

	//m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));

	//m_pTransCom->Invalidate_Worldmatrix();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect_Hit::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect_Hit::Render_GameObject(void)
{
	m_pTransCom->SetUp_OnGraphicDev();

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
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CEffect_Hit::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	// 	system("cls");
	// 	cout << m_fFrame << endl;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransCom->Get_WorldMatrix());

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pEffect->SetMatrix("g_matView", &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

//	m_pTextureCom_1->SetUp_OnShader(pEffect, "g_BaseTexture");
 
 	switch (m_iDrawID)
 	{
 	case 0:
 		m_pTextureCom_0->SetUp_OnShader(pEffect, "g_BaseTexture");
 		break;
 	case 1:
 		m_pTextureCom_1->SetUp_OnShader(pEffect, "g_BaseTexture");
 		break;
 	default:
 		break;
 	}



	pEffect->SetVector("g_vLightDir", &_vec4(1.f, -1.f, 1.f, 0.f));

	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CEffect_Hit::Ready_Component(void)
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

	// For.Com_Texture_0
	pComponent = m_pTextureCom_0 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_HitEffect0");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	// For.Com_Texture_1
	pComponent = m_pTextureCom_1 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_HitEffect1");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();


	// For.Shader_Effect
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

void CEffect_Hit::SetUp_BillBoard(void)
{
	if (nullptr == m_pTransCom)
		return;

	//  현재 객체의 스케일저ㅏㅇ보를 얻어온다.
	_vec3		vScale = m_pTransCom->Get_ScaleInfo();

	// 카메라에는 스케일이 적용되어있지 않기 때문에.
	_matrix			matRot;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matRot);

	D3DXMatrixInverse(&matRot, nullptr, &matRot);

	_vec3		vRight, vUp, vLook;

	memcpy(&vRight, &matRot.m[0][0], sizeof(_vec3));
	m_pTransCom->Set_Infomation(CTransform::INFO_RIGHT, &(vRight * vScale.x));

	//memcpy(&vUp, &matRot.m[1][0], sizeof(_vec3));
	//m_pTransCom->Set_Infomation(CTransform::INFO_UP, &(vUp * vScale.y));

	memcpy(&vLook, &matRot.m[2][0], sizeof(_vec3));
	m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &(vLook * vScale.z));
}

CEffect_Hit * CEffect_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, ZECALLION_BONE eBoneID, _vec3 vBloodPosition, _int iDrawID, _vec3 vecAngle)
{
	CEffect_Hit *	pInstance = new CEffect_Hit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eBoneID, vBloodPosition, iDrawID, vecAngle)))
	{
		MSG_BOX("CEffect_Hit Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect_Hit::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom_1);
	Engine::Safe_Release(m_pTextureCom_0);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}