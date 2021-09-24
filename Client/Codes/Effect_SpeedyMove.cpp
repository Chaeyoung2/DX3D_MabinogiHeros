#include "stdafx.h"
#include "..\Headers\Effect_SpeedyMove.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CEffect_SpeedyMove::CEffect_SpeedyMove(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CEffect_SpeedyMove::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	// Set_Position 방법 1.
	  	CTransform* pTransform_Player = (CTransform*)(CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
	  	_vec3 vecPlayerPos = *(pTransform_Player->Get_Infomation(CTransform::INFO_POSITION));
	 	_vec3 vecEffectPos = _vec3(vecPlayerPos.x, vecPlayerPos.y + 2.f, vecPlayerPos.z - 3.5f);
	 	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &vecEffectPos);

	// Set_Position 방법 2.
// 	CMesh_Dynamic* pMeshCom_Player =
// 		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh", 0));
// 	m_pBoneMatrix_Parent = pMeshCom_Player->Get_BoneMatrix("ValveBiped_Bip01_Spine2");
// 
// 	CTransform* pTransCom_Player = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
// 	m_pWorldMatrix_Parent = pTransCom_Player->Get_WorldMatrix();
// 
// 	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, ((_vec3*)&m_pBoneMatrix_Parent->m[3][0]));


	m_pTransCom->Scaling(&_vec3(2.f, 2.f, 1.f));
	//	m_pTransCom->RotationZ(30.f);


	return NOERROR;
}

_int CEffect_SpeedyMove::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;

	m_fCreateTime += fTimeDelta;
	if (m_fCreateTime >= 2.f)
	{
		return 1;
		}
// 	if (m_bIsSettingBoneMatrix_Completed == false)
// 	{
// 		m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
// 		_vec3 vPosition = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
// 		//vPosition.x += 50.f;
// 		vPosition.y -= 50.f;
// 
// 		m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &vPosition);
// 		//m_pTransCom->RotationY(D3DXToRadian(-60.f));
//  		//m_pTransCom->RotationZ(D3DXToRadian(60.f));
// 		m_pTransCom->RotationX(D3DXToRadian(-60.f));
// 		m_bIsSettingBoneMatrix_Completed = true;
// 	}
	
	//m_fAngle += fTimeDelta;

	m_pTransCom->RotationZ(m_fAngle);

	m_fAngle += fTimeDelta * m_fRotSpeed;

	// 	++m_fFrame;
	// 	if (m_fFrame >= 90)
	// 		m_fFrame = 0.f;

	// 	CVIBuffer* pBuffer = (CVIBuffer*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	// 	if (nullptr == pBuffer)
	// 		return -1;
	// 
	// 	tuple<_ulong, _ulong, _ulong>	BufferInfo = pBuffer->Get_BufferInfomation();
	// 
	// 	m_pTransCom->SetUpHeight_OnBuffer(pBuffer->Get_VertexPos(), get<0>(BufferInfo), get<1>(BufferInfo), get<2>(BufferInfo));

	SetUp_BillBoard();

	m_pTransCom->Invalidate_Worldmatrix();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect_SpeedyMove::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect_SpeedyMove::Render_GameObject(void)
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

HRESULT CEffect_SpeedyMove::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransCom->Get_WorldMatrix());

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pEffect->SetMatrix("g_matView", &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");


	pEffect->SetVector("g_vLightDir", &_vec4(1.f, -1.f, 1.f, 0.f));

	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CEffect_SpeedyMove::Ready_Component(void)
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
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Speedy_Move");
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

void CEffect_SpeedyMove::SetUp_BillBoard(void)
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

CEffect_SpeedyMove * CEffect_SpeedyMove::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_SpeedyMove *	pInstance = new CEffect_SpeedyMove(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CEffect_SpeedyMove Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect_SpeedyMove::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}
