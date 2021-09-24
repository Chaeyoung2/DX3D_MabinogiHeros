#include "stdafx.h"
#include "..\Headers\SmashEffect.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CEffect_Smash::CEffect_Smash(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_vecFrame(_vec3(0.f, 0.f, 0.f))
	, m_vecMaxFrame(_vec3(5.f, 6.f, 0.f))
{

}

HRESULT CEffect_Smash::Ready_GameObject(_vec3 vPosition)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


// 
 //	CMesh_Dynamic* pMeshCom_Player =
 //		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh", 0));

	//m_pBoneMatrix_Parent = pMeshCom_Player->Get_BoneMatrix("ValveBiped_Anim_Attachment_LH");
 	//CTransform* pTransCom_Player = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
 	//m_pWorldMatrix_Parent = pTransCom_Player->Get_WorldMatrix();


	m_pTransCom->Scaling(&_vec3(4.f, 4.f, 1.f));

//	m_pTransCom->RotationX(30.f);
	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(vPosition.x, vPosition.y, vPosition.z));

	return NOERROR;
}

_int CEffect_Smash::Update_GameObject(const _float & fTimeDelta)
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

	//system("cls");
	//cout << m_fFrameTime << endl;

	CTransform* pTransCom_Zecallion = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Transform", 0));
	_vec3 vZecallion_Position = *pTransCom_Zecallion->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vDir = vZecallion_Position - *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vScaleInfo = m_pTransCom->Get_ScaleInfo();

	vDir.x *= vScaleInfo.x; vDir.y *= vScaleInfo.y; vDir.z *= vScaleInfo.z;

	m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vDir);

	m_pTransCom->Go_Staight(fTimeDelta * 50.f);

	m_fFrameTime += fTimeDelta;

 	if (m_fFrameTime >= 0.05f)
 	{
		m_vecFrame.x += 0.8f;
		m_fFrameTime = 0.f;
	}


	if (m_vecFrame.x >= m_vecMaxFrame.x)
	{
		m_vecFrame.x = 0.f;
		m_vecFrame.y += 1.5f;
	}

	if (m_vecFrame.y >= m_vecMaxFrame.y)
	{
/*		m_vecFrame = _vec3(0.f, 0.f, 0.f);*/
		return 1;
	}

	SetUp_BillBoard();

	//m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));

	//m_pTransCom->Invalidate_Worldmatrix();


	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect_Smash::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect_Smash::Render_GameObject(void)
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CEffect_Smash::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");


	pEffect->SetVector("g_vecFrame", &(_vec4)m_vecFrame);

	pEffect->SetVector("g_vecMaxFrame", &(_vec4)m_vecMaxFrame);

	pEffect->SetFloat("g_fSize", 192.f);

	pEffect->SetFloat("g_fCountX", 5.f);
	pEffect->SetFloat("g_fCountY", 6.f);

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CEffect_Smash::Ready_Component(void)
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
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_SmashEffect");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();



	// For.Shader_Effect
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect_Multi");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

void CEffect_Smash::SetUp_BillBoard(void)
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

CEffect_Smash * CEffect_Smash::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPosition)
{
	CEffect_Smash *	pInstance = new CEffect_Smash(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPosition)))
	{
		MSG_BOX("CEffect_Smash Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect_Smash::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}