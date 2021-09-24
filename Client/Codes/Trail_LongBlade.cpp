#include "stdafx.h"
#include "..\Headers\Trail_LongBlade.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CTrail_LongBlade::CTrail_LongBlade(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
{
}

HRESULT CTrail_LongBlade::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_fDetail = 3.f;
	m_fAlpha = 0.f;

	D3DXCreateLine(m_pGraphicDev, &m_pLine);

	CMesh_Dynamic* pMeshCom_Player =
		((CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh", 0));

	m_pBoneMatrix_Parent = pMeshCom_Player->Get_BoneMatrix("ValveBiped_Anim_Attachment_RH");


	CTransform* pTransCom_Player = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0));
	m_pWorldMatrix_Parent = pTransCom_Player->Get_WorldMatrix();

	m_pTransCom->Scaling(&_vec3(100.f, 100.f, 100.f));

	return NOERROR;
}

_int CTrail_LongBlade::Update_GameObject(const _float & fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	m_fCreateTime += fTimeDelta;

	if (m_fCreateTime >= 2.f)
	{
		Engine::Safe_Release(m_pLine);
		m_vPointlist.clear();
		return 1;
	}




	return _int();
}

_int CTrail_LongBlade::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom
		|| nullptr == m_pRendererCom)
		return -1;

	if (m_pmatTarget == nullptr)
	{
		CTransform* pTransCom_Blade = ((CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 1));
		m_pmatTarget = pTransCom_Blade->Get_WorldMatrix();
	}

	m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));
	
	Point_Memorize(fTimeDelta);


	return _int();
}

void CTrail_LongBlade::Render_GameObject(void)
{
	if (m_vPointlist.size() <= 3)
		return;

	POINTLIST::iterator iter_begin	= m_vPointlist.begin();
	POINTLIST::iterator iter_end = m_vPointlist.end();

	_vec3* pPoint = new _vec3[m_vPointlist.size() * 0.5f];
	_vec3* pDir = new _vec3[m_vPointlist.size() * 0.5f];

	_int iIndex = 0;

	for (; iter_begin != iter_end; ++iter_begin)
	{
		pPoint[iIndex] = (*iter_begin);
		++iter_begin;

		pDir[iIndex] = (*iter_begin);
		++iIndex;
	}

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pBufferCom->Set_VertexTrail(pPoint, pDir, m_fLength, m_vPointlist.size());

	m_pTransCom->SetUp_OnGraphicDev(); // ¾ê ÇØ¾ßµÇ³ª..?
 
	if (nullptr == m_pShaderCom)
		return;

	// ¼ÎÀÌ´õ¿ë ÀÌÆåÆ® °´Ã¼.
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	// ¼ÎÀÌ´õ ÄÁ½ºÅÏÆ® Å×ÀÌºí ¼¼ÆÃ.
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	//m_pBufferCom->Render_Buffer(m_pTransCom->Get_WorldMatrix());
	//m_pBufferCom->Render_Buffer(m_pTransCom->Get_WorldMatrix());

	pEffect->EndPass();
	pEffect->End();


	_vec3		vPoint[2];
	vPoint[0] = _vec3(0.f, 0.f, 0.f);
	vPoint[1] = _vec3(0.f, 0.f, -50.f);

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIX		matTemp = (*m_pmatTarget) * matView * matProj;

	m_pLine->SetWidth(2.f);
	m_pLine->Begin();
	//m_pLine->DrawTransform(vPoint, 2, &matTemp, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
	m_pLine->End();

	Engine::Safe_Delete_Array(pPoint);
	Engine::Safe_Delete_Array(pDir);

	Safe_Release(pEffect);


// 

// 
// 

}

HRESULT CTrail_LongBlade::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CTrail_LongBlade::Point_Memorize(const _float& fTimeDelta)
{
	while (m_vPointlist.size() >= 40)
	{
		m_vPointlist.pop_front();
		m_vPointlist.pop_front();
	}


	m_fAccTime += fTimeDelta;

	if (0.01f < m_fAccTime)
	{
		_vec3		vPoint[2];
		vPoint[0] = _vec3(0.f, 0.f, 0.f);
		vPoint[1] = _vec3(0.f, 0.f, -150.f);

		_vec3		vDir, vPos;
		vDir = vPoint[1] - vPoint[0];
		m_fLength = D3DXVec3Length(&vDir) / 2.f;
		D3DXVec3Normalize(&vDir, &vDir);

		vPos = vDir * m_fLength;

		D3DXVec3TransformCoord(&vPos, &vPos, m_pmatTarget);
		m_vPointlist.push_back(vPos);

		D3DXVec3TransformNormal(&vDir, &vDir, m_pmatTarget);
		m_vPointlist.push_back(vDir);

		m_fAccTime = 0.f;
	}
}

HRESULT CTrail_LongBlade::Ready_Component(void)
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
	pComponent = m_pBufferCom = (CTrail_Buffer*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Buffer_Trail");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	pComponent->AddRef();

	// For.Com_Texture
	pComponent = m_pTextureCom = (CTexture*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Trail_LongBlade00");
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

CTrail_LongBlade * CTrail_LongBlade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrail_LongBlade *	pInstance = new CTrail_LongBlade(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTrail_LongBlade Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTrail_LongBlade::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	return CGameObject::Free();
}
