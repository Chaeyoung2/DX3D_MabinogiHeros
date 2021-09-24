
#include "stdafx.h"
#include "..\Headers\Effect_ShockWave.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CEffect_ShockWave::CEffect_ShockWave(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CEffect_ShockWave::Ready_GameObject(_vec3 vPosition)
{

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vecCurScale = _vec3(1.f, 1.f, 1.f);

	_float fMultiValue = 8.f;
	m_vecGoalScale = _vec3(m_vecCurScale.x * fMultiValue, m_vecCurScale.y * fMultiValue, 1.f);

	m_pTransCom->Scaling(&m_vecCurScale);
	m_pTransCom->RotationX(D3DXToRadian(90.f));
	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &vPosition);

	return NOERROR;
}

_int CEffect_ShockWave::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;



	//SetUp_BillBoard();

	m_pTransCom->Scaling(&m_vecCurScale);

	Expand_Radius(fTimeDelta);

	if (m_vecCurScale.x <= m_vecGoalScale.x)
	{
		//m_fSpeed += fTimeDelta;
		m_vecCurScale.x += fTimeDelta * m_fSpeed;
		m_vecCurScale.y += fTimeDelta * m_fSpeed;
	}
	else
		return 1;

	// 		system("cls");
	// 		cout << m_fFrame << endl;


	// m_pTransCom->Invalidate_Worldmatrix();


	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CEffect_ShockWave::LastUpdate_GameObject(const _float & fTimeDelta)
{
	//Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CEffect_ShockWave::Render_GameObject(void)
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

}

HRESULT CEffect_ShockWave::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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


	Safe_Release(pEffect);

	return NOERROR;
}

void CEffect_ShockWave::Expand_Radius(const _float & fTimeDelta)
{
	// 	system("cls");
	// 	cout << m_vecCurScale.x << endl;
	// 	cout << m_vecCurScale.y << endl;
	// 	if (m_vecCurScale.x <= m_vecGoalScale.x)
	// 	{
	// 		//m_fSpeed += fTimeDelta;
	// 		m_vecCurScale.x += fTimeDelta * m_fSpeed; 
	// 		m_vecCurScale.y += fTimeDelta * m_fSpeed;
	// 	}
}

HRESULT CEffect_ShockWave::Ready_Component(void)
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
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_RingOuterWind01");
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

void CEffect_ShockWave::SetUp_BillBoard(void)
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

CEffect_ShockWave * CEffect_ShockWave::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPosition)
{
	CEffect_ShockWave *	pInstance = new CEffect_ShockWave(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPosition)))
	{
		MSG_BOX("CEffect_ShockWave Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect_ShockWave::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}