#include "stdafx.h"
#include "..\Headers\Effect.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CEffect::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(rand() % 10, 7.0f, rand() % 10));

	return NOERROR;
}

_int CEffect::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;		

	
	++m_fFrame;
	if (m_fFrame >= 90)
		m_fFrame = 0.f;

	CVIBuffer* pBuffer = (CVIBuffer*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBuffer)
		return -1;

	
	tuple<_ulong, _ulong, _ulong>	BufferInfo = pBuffer->Get_BufferInfomation();

	//m_pTransCom->SetUpHeight_OnBuffer(pBuffer->Get_VertexPos(), get<0>(BufferInfo), get<1>(BufferInfo), get<2>(BufferInfo));

	SetUp_BillBoard();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect::Render_GameObject(void)
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
	m_pTextureCom->SetUp_OnGraphicDev(0, _uint(m_fFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

HRESULT CEffect::Ready_Component(void)
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
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

void CEffect::SetUp_BillBoard(void)
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

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect *	pInstance = new CEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	return CGameObject::Free();
}
