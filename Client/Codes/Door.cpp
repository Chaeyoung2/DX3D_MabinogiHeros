#include "stdafx.h"
#include "..\Headers\Door.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pCollisionCom(nullptr)
	, m_pShaderCom(nullptr)
{
}


HRESULT CDoor::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_pTransCom->Scaling(&_vec3(0.06f, 0.06f, 0.06f));
	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(65.f, 10.f, 235.f));
	m_vecOriPos = _vec3(65.f, 10.f, 235.f);


	m_vecGoalPos = _vec3(65.f, -26.f, 235.f);
	m_vecGoalPos_ToUp = _vec3(65.f, 25.f, 235.f);
	m_vecGoalPos_ToDownSecond = _vec3(65.f, -30.f, 235.f);

	return NOERROR;
}

_int CDoor::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransCom->Invalidate_Worldmatrix();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	Move_DependingOnZecallion(fTimeDelta);

	if (m_bIsDead == true)
		return 1;

	return _int();
}

_int CDoor::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CDoor::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

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
}

void CDoor::Move_DependingOnZecallion(const _float & fTimeDelta)
{
	CMesh_Dynamic* pZecallion_Mesh = (CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Mesh", 0);
	_float fZecallion_GetTrackPosition = pZecallion_Mesh->Get_TrackPosition();
	_int iCurZecallionAniIdx = pZecallion_Mesh->Get_AnimationIdx();

// 	system("cls");
// 	cout << "fZecallion_GetTrackPosition : " << fZecallion_GetTrackPosition << endl;
// 	cout << "iCurZecallionAniIdx : " << iCurZecallionAniIdx << endl;

	if (iCurZecallionAniIdx == 8 && m_bIsToDown_Completed == false) // SPAWN
	{
		m_bIsToDown_First = true;
		m_bIsToDown_Completed = true;
	}

	if (iCurZecallionAniIdx == 8)
	{
		if (fZecallion_GetTrackPosition >= 2.85 && fZecallion_GetTrackPosition <= 2.9)
		{
			m_bIsToUp = true;
		}
	}


	//
	if (m_bIsToDown_First == true)
	{
		_vec3 vDir = m_vecGoalPos - m_vecOriPos;
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.x *= vecScaleInfo.x;
		vDir.y *= vecScaleInfo.y;
		vDir.z *= vecScaleInfo.z;
		m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vDir);
		m_pTransCom->Go_Staight(fTimeDelta * m_fSpeed);

		_vec3 vCurrentPos = *(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

		if (vCurrentPos.y <= m_vecGoalPos.y)
		{
			m_bIsToDown_First = false;
			m_vecOriPos = vCurrentPos;
		}
	}

	if (m_bIsToUp == true)
	{
		_vec3 vDir = m_vecGoalPos_ToUp - m_vecOriPos;
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.x *= vecScaleInfo.x;
		vDir.y *= vecScaleInfo.y;
		vDir.z *= vecScaleInfo.z;
		m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vDir);
		m_pTransCom->Go_Staight(fTimeDelta * m_fSpeed);

		_vec3 vCurrentPos = *(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));


		if (vCurrentPos.y >= m_vecGoalPos_ToUp.y)
		{
			m_bIsToUp = false;
			m_bIsToDown_Second = true;
			m_vecOriPos = vCurrentPos;
		}
	}

	if (m_bIsToDown_Second == true)
	{
		_vec3 vDir = m_vecGoalPos_ToDownSecond - m_vecOriPos;
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.x *= vecScaleInfo.x;
		vDir.y *= vecScaleInfo.y;
		vDir.z *= vecScaleInfo.z;
		m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vDir);
		m_pTransCom->Go_Staight(fTimeDelta * m_fSpeed);

		_vec3 vCurrentPos = *(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

// 		system("cls");
// 		cout << "m_vecGoalPos_ToDownSecond.x : " << m_vecGoalPos_ToDownSecond.x << endl;
// 		cout << "m_vecGoalPos_ToDownSecond.y : " << m_vecGoalPos_ToDownSecond.y << endl;
// 		cout << "m_vecGoalPos_ToDownSecond.z : " << m_vecGoalPos_ToDownSecond.z << endl;
// 		cout << "vCurrentPos.x : " << vCurrentPos.x << endl;
// 		cout << "vCurrentPos.y : " << vCurrentPos.y << endl;
// 		cout << "vCurrentPos.z : " << vCurrentPos.z << endl;

		if (vCurrentPos.y <= m_vecGoalPos_ToDownSecond.y)
		{
			m_bIsToDown_Second = false;
		}
	}

}

HRESULT CDoor::Ready_Component(void)
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
	pComponent = m_pMeshCom = (CMesh_Static*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Door_01");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Component_Mesh", pComponent));


	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	return NOERROR;

}

HRESULT CDoor::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CDoor * CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoor *	pInstance = new CDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CDoor Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CDoor::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}

