// #include "stdafx.h"
// #include "..\Headers\Effect_Blood.h"
// #include "Component_Manager.h"
// #include "Object_Manager.h"
// 
// CEffect_Blood::CEffect_Blood(LPDIRECT3DDEVICE9 pGraphicDev)
// 	: CGameObject(pGraphicDev)
// 	, m_pBufferCom(nullptr)
// 	, m_pTextureCom_1(nullptr)
// 	, m_pTextureCom_2(nullptr)
// 	, m_pTextureCom_3(nullptr)
// 	, m_pRendererCom(nullptr)
// 	, m_pTransCom(nullptr)
// {
// 
// }
// 
// HRESULT CEffect_Blood::Ready_GameObject(ZECALLION_BONE eBoneID, _int iDrawID)
// {
// 	if (FAILED(Ready_Component()))
// 		return E_FAIL;
// 
// 	m_eBoneID = eBoneID;
// 	m_iDrawID = iDrawID;
// 
// 	if (m_iDrawID == 1)
// 		m_fMaxFrame = 9.f;
// 	else
// 		m_fMaxFrame = 16.f;
// 
// 	CMesh_Dynamic* pMeshCom_Zecallion =
// 		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Mesh", 0));
// 
// 	switch (m_eBoneID)
// 	{
// 	case BONE_LEFT_CALF:
// 		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_L_Calf");
// 		break;
// 	case BONE_RIGHT_CALF:
// 		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_R_Calf");
// 		break;
// 	case BONE_LEFT_HAND:
// 		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_L_Hand");
// 		break;
// 	case BONE_RIGHT_HAND:
// 		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("ValveBiped_Bip01_R_Hand");
// 		break;
// 	case BONE_FRONT:
// 		m_pBoneMatrix_Parent = pMeshCom_Zecallion->Get_BoneMatrix("jiggle_zecallion_front");
// 		break;
// 	}
// 
// 	CTransform* pTransCom_Zecallion = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Transform", 0));
// 	m_pWorldMatrix_Parent = pTransCom_Zecallion->Get_WorldMatrix();
// 
// 	m_pTransCom->Scaling(&_vec3(10.f, 10.f, 1.f));
// 	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(0.f, 0.f, 0.f));
// 
// 	return NOERROR;
// }
// 
// _int CEffect_Blood::Update_GameObject(const _float & fTimeDelta)
// {
// 	if (nullptr == m_pTransCom
// 		|| nullptr == m_pRendererCom
// 		|| nullptr == m_pBufferCom)
// 		return -1;
// 
// // 	m_fCreateTime += fTimeDelta;
// // 	if (m_fCreateTime >= 1.5f)
// // 	{
// // 		return 1;
// // 	}
// 
// 
// 	 	++m_fFrame;
// 		if (m_fFrame >= m_fMaxFrame)
// 			m_fFrame = 0.f;
// 			//			return 1;
// 
// 	 
// 	//SetUp_BillBoard();
// 
// 	// m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
// 
// 	//m_pTransCom->Invalidate_Worldmatrix();
// 
// 	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);
// 
// 	return _int();
// }
// 
// _int CEffect_Blood::LastUpdate_GameObject(const _float & fTimeDelta)
// {
// 	//Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));
// 
// 	return _int();
// }
// 
// void CEffect_Blood::Render_GameObject(void)
// {
// 
// 	if (nullptr == m_pShaderCom)
// 		return;
// 
// 	m_pTransCom->SetUp_OnGraphicDev();
// 
// 	// 셰이더용 이펙트 객체.
// 	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
// 	if (nullptr == pEffect)
// 		return;
// 	pEffect->AddRef();
// 
// 
// 	// 셰이더 컨스턴트 테이블 세팅.
// 	if (FAILED(SetUp_ConstantTable(pEffect)))
// 		return;
// 
// 	pEffect->Begin(nullptr, 0);
// 	pEffect->BeginPass(1);
// 
// 	m_pBufferCom->Render_Buffer();
// 
// 	pEffect->EndPass();
// 	pEffect->End();
// 
// 
// }
// 
// HRESULT CEffect_Blood::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
// {
// // 	system("cls");
// // 	cout << m_fFrame << endl;
// 
// 	pEffect->AddRef();
// 
// 	pEffect->SetMatrix("g_matWorld", m_pTransCom->Get_WorldMatrix());
// 
// 	_matrix		matView, matProj;
// 
// 	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
// 	pEffect->SetMatrix("g_matView", &matView);
// 
// 	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
// 	pEffect->SetMatrix("g_matProj", &matProj);
// 
// 	switch (m_iDrawID)
// 	{
// 	case 1:
// 		m_pTextureCom_1->SetUp_OnShader(pEffect, "g_BaseTexture", (int)m_fFrame);
// 		break;
// 	case 2:
// 		m_pTextureCom_2->SetUp_OnShader(pEffect, "g_BaseTexture", (int)m_fFrame);
// 		break;
// 	case 3:
// 		m_pTextureCom_3->SetUp_OnShader(pEffect, "g_BaseTexture", (int)m_fFrame);
// 		break;
// 	default:
// 		break;
// 	}
// 	
// 
// 
// 	pEffect->SetVector("g_vLightDir", &_vec4(1.f, -1.f, 1.f, 0.f));
// 
// 	D3DXMatrixInverse(&matView, nullptr, &matView);
// 
// 	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);
// 
// 	Safe_Release(pEffect);
// 
// 	return NOERROR;
// }
// 
// HRESULT CEffect_Blood::Ready_Component(void)
// {
// 	if (nullptr == m_pComponent_Manager)
// 		return E_FAIL;
// 
// 	CComponent*		pComponent = nullptr;
// 
// 	// For.Com_Renderer
// 	pComponent = m_pRendererCom = (CRenderer*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
// 	pComponent->AddRef();
// 
// 
// 	// For.Com_Transform
// 	pComponent = m_pTransCom = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
// 	pComponent->AddRef();
// 
// 
// 	// For.Com_Buffer
// 	pComponent = m_pBufferCom = (CRect_Texture*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTexture");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
// 	pComponent->AddRef();
// 
// 	// For.Com_Texture_1
// 	pComponent = m_pTextureCom_1 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_1");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
// 	pComponent->AddRef();
// 	// For.Com_Texture_2
// 	pComponent = m_pTextureCom_2 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_2");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
// 	pComponent->AddRef();
// 	// For.Com_Texture_3
// 	pComponent = m_pTextureCom_3 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_3");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
// 	pComponent->AddRef();
// 
// 	// For.Shader_Effect
// 	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
// 	pComponent->AddRef();
// 
// 	return NOERROR;
// }
// 
// void CEffect_Blood::SetUp_BillBoard(void)
// {
// 	if (nullptr == m_pTransCom)
// 		return;
// 
// 	//  현재 객체의 스케일저ㅏㅇ보를 얻어온다.
// 	_vec3		vScale = m_pTransCom->Get_ScaleInfo();
// 
// 	// 카메라에는 스케일이 적용되어있지 않기 때문에.
// 	_matrix			matRot;
// 	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matRot);
// 
// 	D3DXMatrixInverse(&matRot, nullptr, &matRot);
// 
// 	_vec3		vRight, vUp, vLook;
// 
// 	memcpy(&vRight, &matRot.m[0][0], sizeof(_vec3));
// 	m_pTransCom->Set_Infomation(CTransform::INFO_RIGHT, &(vRight * vScale.x));
// 
// 	//memcpy(&vUp, &matRot.m[1][0], sizeof(_vec3));
// 	//m_pTransCom->Set_Infomation(CTransform::INFO_UP, &(vUp * vScale.y));
// 
// 	memcpy(&vLook, &matRot.m[2][0], sizeof(_vec3));
// 	m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &(vLook * vScale.z));
// }
// 
// CEffect_Blood * CEffect_Blood::Create(LPDIRECT3DDEVICE9 pGraphicDev, ZECALLION_BONE eBoneID, _int iDrawID)
// {
// 	CEffect_Blood *	pInstance = new CEffect_Blood(pGraphicDev);
// 
//  	if (FAILED(pInstance->Ready_GameObject(eBoneID, iDrawID)))
// 	{
// 		MSG_BOX("CEffect_Blood Created Failed");
// 		Safe_Release(pInstance);
// 	}
// 	return pInstance;
// }
// 
// _ulong CEffect_Blood::Free(void)
// {
// 	Engine::Safe_Release(m_pRendererCom);
// 	Engine::Safe_Release(m_pTransCom);
// 	Engine::Safe_Release(m_pBufferCom);
// 	Engine::Safe_Release(m_pTextureCom_1);
// 	Engine::Safe_Release(m_pTextureCom_2);
// 	Engine::Safe_Release(m_pTextureCom_3);
// 	Engine::Safe_Release(m_pShaderCom);
// 
// 	return CGameObject::Free();
// }


#include "stdafx.h"
#include "..\Headers\Effect_Blood.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CEffect_Blood::CEffect_Blood(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom_1(nullptr)
	, m_pTextureCom_2(nullptr)
	, m_pTextureCom_3(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CEffect_Blood::Ready_GameObject(ZECALLION_BONE eBoneID, _vec3 vBloodPosition, _int iDrawID)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eBoneID = eBoneID;
	m_iDrawID = iDrawID;

	if (m_iDrawID == 1)
	{
		m_fMaxFrame = 8.f;
		m_pTransCom->Scaling(&_vec3(3.f, 3.f, 1.f));
	}
	else
	{
		m_fMaxFrame = 15.f;
		m_pTransCom->Scaling(&_vec3(3.f, 3.f, 1.f));
	}

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


	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(vBloodPosition.x, vBloodPosition.y, vBloodPosition.z - 5.f));

	return NOERROR;
}

_int CEffect_Blood::Update_GameObject(const _float & fTimeDelta)
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
	m_fFrame++;
	if (m_fFrame >= m_fMaxFrame)
	{
		m_fFrame = 0.f;
		return 1;
	}


	//SetUp_BillBoard();

	//m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));

	//m_pTransCom->Invalidate_Worldmatrix();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect_Blood::LastUpdate_GameObject(const _float & fTimeDelta)
{
	//Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect_Blood::Render_GameObject(void)
{

	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);*/



	// 월드행렬을 장치에 셋팅한다.	
	m_pTransCom->SetUp_OnGraphicDev();
	m_pTextureCom_1->SetUp_OnGraphicDev(0, _uint(m_fFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);


}

HRESULT CEffect_Blood::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	switch (m_iDrawID)
	{
	case 1:
		m_pTextureCom_1->SetUp_OnShader(pEffect, "g_BaseTexture", (int)m_fFrame);
		break;
	case 2:
		m_pTextureCom_2->SetUp_OnShader(pEffect, "g_BaseTexture", (int)m_fFrame);
		break;
	case 3:
		m_pTextureCom_3->SetUp_OnShader(pEffect, "g_BaseTexture", (int)m_fFrame);
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

HRESULT CEffect_Blood::Ready_Component(void)
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

	// For.Com_Texture_1
	pComponent = m_pTextureCom_1 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_1");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();
	// For.Com_Texture_2
	pComponent = m_pTextureCom_2 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_2");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();
	// For.Com_Texture_3
	pComponent = m_pTextureCom_3 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_3");
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

void CEffect_Blood::SetUp_BillBoard(void)
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

CEffect_Blood * CEffect_Blood::Create(LPDIRECT3DDEVICE9 pGraphicDev, ZECALLION_BONE eBoneID, _vec3 vBloodPosition, _int iDrawID)
{
	CEffect_Blood *	pInstance = new CEffect_Blood(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eBoneID, vBloodPosition, iDrawID)))
	{
		MSG_BOX("CEffect_Blood Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect_Blood::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom_1);
	Engine::Safe_Release(m_pTextureCom_2);
	Engine::Safe_Release(m_pTextureCom_3);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}
