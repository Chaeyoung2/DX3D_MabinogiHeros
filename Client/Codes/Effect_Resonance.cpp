#include "stdafx.h"
#include "..\Headers\Effect_Resonance.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CEffect_Resonance::CEffect_Resonance(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom_0(nullptr)
	, m_pTextureCom_1(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CEffect_Resonance::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	// Set_Position 방법 1.
//  	CTransform* pTransform_Player = (CTransform*)(CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
//  	_vec3 vecPlayerPos = *(pTransform_Player->Get_Infomation(CTransform::INFO_POSITION));
// 	_vec3 vecEffectPos = _vec3(vecPlayerPos.x + 3.f, vecPlayerPos.y + 5.f, vecPlayerPos.z + 0.5f);
// 	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &vecEffectPos);

		// Set_Position 방법 2.
	CMesh_Dynamic* pMeshCom_Player =
		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh", 0));
	m_pBoneMatrix_Parent = pMeshCom_Player->Get_BoneMatrix("ValveBiped_Bip01_L_UpperArm01");

	CTransform* pTransCom_Player = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
	m_pWorldMatrix_Parent = pTransCom_Player->Get_WorldMatrix();

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, ((_vec3*)&m_pBoneMatrix_Parent->m[3][0]));


	m_pTransCom->Scaling(&_vec3(35.f, 35.f, 1.f));
//	m_pTransCom->RotationZ(30.f);

	
	return NOERROR;
}

_int CEffect_Resonance::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;

	if (m_bIsSettingBoneMatrix_Completed == false)
	{
		m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
		_vec3 vPosition = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
		vPosition.x += 50.f;
		vPosition.y -= 20.f;

		m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &vPosition);
		m_bIsSettingBoneMatrix_Completed = true;
	}

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

	//SetUp_BillBoard();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect_Resonance::LastUpdate_GameObject(const _float & fTimeDelta)
{
	//Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect_Resonance::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
// 	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
// 	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
// 	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);



	// 월드행렬을 장치에 셋팅한다.	
	m_pTransCom->SetUp_OnGraphicDev();
	m_pTextureCom_0->SetUp_OnGraphicDev(0, 0);
	//m_pTextureCom_1->SetUp_OnGraphicDev(0, 0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

//	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

HRESULT CEffect_Resonance::Ready_Component(void)
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
	pComponent = m_pTextureCom_0 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Resonance_0");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	// For.Com_Texture_1
	pComponent = m_pTextureCom_1 = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Resonance_1");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

void CEffect_Resonance::SetUp_BillBoard(void)
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

CEffect_Resonance * CEffect_Resonance::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Resonance *	pInstance = new CEffect_Resonance(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CEffect_Resonance Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect_Resonance::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom_0);
	Engine::Safe_Release(m_pTextureCom_1);

	return CGameObject::Free();
}
