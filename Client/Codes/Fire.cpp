#include "stdafx.h"
#include "..\Headers\Fire.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{

}

HRESULT CFire::Ready_GameObject(_int iDrawID, _vec3 vPosition)
{
	m_iDrawID = iDrawID;

	if (FAILED(Ready_Component()))
		return E_FAIL;



	_vec3 vScale = _vec3(10.f, 10.f, 1.f);
	_float fUpY = 5.f;
	_float fPlusX = 0.f;
	_float fPlusZ = 0.f;

	if (iDrawID == 1)
	{
		fPlusX -= 0.5f;
		fUpY += 1.5f;
		vScale = _vec3(5.f, 10.f, 1.f);
	}
	else if (iDrawID == 3)
	{

		if (vPosition.x <= 60.f)
		{
			if (vPosition.z <= 130.f)
			{
				fPlusX += 1.f;
				fUpY += 8.f;
			}
			else
			{
				fPlusX += 3.f;
				fUpY += 17.f;
			}
		}
		else
		{
			if (vPosition.z <= 130.f)
			{
				fPlusX -= 1.f;
				fUpY += 8.f;
				fPlusZ += 2.f;
			}
			else
			{
				fPlusX -= 3.f;
				fUpY += 17.f;
				fPlusZ += 3.f;
			}
		}

		//m_pTransCom->RotationZ(30.f);
	}
	else if(iDrawID == 5 || iDrawID == 6 || iDrawID == 7)
	{
		vScale = _vec3(20.f, 10.f, 1.f);
		fPlusZ -= 5.f;
	}
	else
	{
		vScale = _vec3(20.f, 20.f, 1.f);
		fUpY += 5.f;
	}

	//	m_pTransCom->RotationX(30.f);
	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(vPosition.x + fPlusX, vPosition.y + fUpY, vPosition.z + fPlusZ));
	m_pTransCom->Scaling(&vScale);

	return NOERROR;
}

_int CFire::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom
		|| nullptr == m_pBufferCom)
		return -1;

	m_fFrame++;
	if (m_fFrame >= m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}

	SetUp_BillBoard();

	// 		system("cls");
	// 		cout << m_fFrame << endl;

	m_pTransCom->Invalidate_Worldmatrix();


	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CFire::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));

	return _int();
}

void CFire::Render_GameObject(void)
{
	// m_pTransCom->SetUp_OnGraphicDev();

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

HRESULT CFire::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);


	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CFire::Ready_Component(void)
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
	switch (m_iDrawID)
	{
	case 0:
	{
		m_fMaxFrame = 30.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_Fire");
	}
	break;
	case 1:
	{
		m_fMaxFrame = 30.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_FireB");
	}
	break;
	case 2:
	{
		m_fMaxFrame = 50.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_FlameA");
	}
	break;
	case 3:
	{
		m_fMaxFrame = 30.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_FlameB");
	}
	break;
	case 4:
	{
		m_fMaxFrame = 30.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_FlameC");
	}
	break;
	case 5:
	{
		m_fMaxFrame = 25.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_GroundFireA");
	}
	break;
	case 6:
	{
		m_fMaxFrame = 26.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_GroundFireB");
	}
	break;
	case 7:
	{
		m_fMaxFrame = 21.f;
		pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Environment_GroundFireC");
	}
	break;
	}
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

void CFire::SetUp_BillBoard(void)
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

CFire * CFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iDrawID, _vec3 vPosition)
{
	CFire *	pInstance = new CFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(iDrawID, vPosition)))
	{
		MSG_BOX("CFire Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CFire::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}