#include "stdafx.h"
#include "..\Headers\Map_Object_List.h"
#include "Component_Manager.h"
#include "Map_Object.h"
#include "Object_Manager.h"

CMap_Object_List::CMap_Object_List(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pPickingCom(nullptr)
{
}


HRESULT CMap_Object_List::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);


	//Cube_Load();

	//m_pTransCom->Scaling(&_vec3(10.f, 10.f, 10.f));
	return NOERROR;
}

_int CMap_Object_List::Update_GameObject(const _float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CMap_Object_List::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CMap_Object_List::Render_GameObject(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 월드행렬을 장치에 셋팅한다.
	m_pTransCom->SetUp_OnGraphicDev();
	//m_pTextureCom->SetUp_OnGraphicDev(0, 0);
	//m_pBufferCom->Render_Buffer(); 


	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

}

HRESULT CMap_Object_List::Ready_Component(void)
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



	// For.Com_Picking
	pComponent = m_pPickingCom = (CPicking*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Picking");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Picking", pComponent));
	pComponent->AddRef();

 


	return NOERROR;
}


void CMap_Object_List::MapObject_Create(_vec3 vecScale, _vec3 vecRot, const _int & iDrawID, const _int & iOption)
{
	// 터레인 버퍼에서 터레인 포지션을 가져옴
	CVIBuffer* pBuffer = (CTerrain_Buffer*)CComponent_Manager::Get_Instance()->Find_Component(SCENE_STATIC, L"Component_Buffer_Terrain");
	//CVIBuffer* pBuffer = (CVIBuffer*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STATIC, L"Layer_BackGround", L"Com_Buffer_Terrain", 0);
	//m_pVertexPos = m_pTerrainBufferCom->Get_VertexPos();

	// 피킹된 위치 받아오고
	tuple<_ulong, _ulong, _ulong>	BufferInfo = pBuffer->Get_BufferInfomation();
	 _vec3 VecPos = m_pPickingCom->Picking_OnRectBuffer_ReturnVec3(g_hWnd, pBuffer->Get_VertexPos(), get<0>(BufferInfo), get<1>(BufferInfo));


	// GameObject Add
	CGameObject* pGameObject = nullptr;
	if (VecPos.x == 0.f && VecPos.y == 0.f && VecPos.z == 0.f)
		return;
	pGameObject = CMap_Object::Create(m_pGraphicDev, vecScale, vecRot, iOption, iDrawID, VecPos);
	if (nullptr == pGameObject)
		return;

	CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_BackGround", pGameObject);

	// 파일 입출력 데이터
	MAPOBJ* pMapObj = new MAPOBJ;
	pMapObj->vPos = VecPos;
	pMapObj->vScale = vecScale;
	pMapObj->vRot = vecRot;
	pMapObj->byDrawID = iDrawID;
	pMapObj->byOption = iOption;
	//pMapObj->fAngle = fAngle;


	m_MapObjectList.push_back(pMapObj);



}


void CMap_Object_List::MapObject_Load(PMAPOBJ pMapObj)
{

	// 3) 밖에서 받아옴

	// GameObject Add
	CGameObject* pGameObject = nullptr;

	if (pMapObj == nullptr)
		return;

	pGameObject = CMap_Object::Create(m_pGraphicDev, pMapObj->vScale, pMapObj->vRot, pMapObj->byOption, pMapObj->byDrawID, pMapObj->vPos);

	if (nullptr == pGameObject)
		return;

	CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_BackGround", pGameObject);


}

void CMap_Object_List::MapObject_ForPreview_Create(_vec3 vecScale, _vec3 vecRot, const _int & iDrawID, const _int & iOption)
{

	// 터레인 버퍼에서 터레인 포지션을 가져옴
	CVIBuffer* pBuffer = (CTerrain_Buffer*)CComponent_Manager::Get_Instance()->Find_Component(SCENE_STATIC, L"Component_Buffer_Terrain");
	//CVIBuffer* pBuffer = (CVIBuffer*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STATIC, L"Layer_BackGround", L"Com_Buffer_Terrain", 0);
	//m_pVertexPos = m_pTerrainBufferCom->Get_VertexPos();

	// 피킹된 위치 받아오고
    tuple<_ulong, _ulong, _ulong>	BufferInfo = pBuffer->Get_BufferInfomation();
	_vec3 VecPos = m_pPickingCom->Picking_OnRectBuffer_ReturnVec3(g_hWnd, pBuffer->Get_VertexPos(), get<0>(BufferInfo), get<1>(BufferInfo));


	// GameObject Add
	CGameObject* pGameObject = nullptr;

	pGameObject = CMap_Object::Create(m_pGraphicDev, vecScale, vecRot, iOption, iDrawID, VecPos);
	if (nullptr == pGameObject)
		return;

 	//((CMap_Object*)pGameObject)->Set_IsForPreview(true);

	CObject_Manager::Get_Instance()->Add_GameObject(SCENE_OFF, L"Layer_Preview", ((CGameObject*)pGameObject));



	m_pForPreview = pGameObject;

}
      
void CMap_Object_List::MapObject_ForPreview_Delete()
{
	//CObject_Manager::Get_Instance()->Release_GameObject(SCENE_OFF);
	//Safe_Release(m_pForPreview);
	//CObject_Manager::Get_Instance()->Release_GameObject_Particular(SCENE_OFF, L"Layer_Preview");
	if(m_pForPreview != nullptr)
		((CGameObject*)m_pForPreview)->Set_IsDead(true);
	m_bIsSetCompleted = false;
}

CMap_Object_List * CMap_Object_List::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap_Object_List *	pInstance = new CMap_Object_List(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CMap_Object_List Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMap_Object_List::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pPickingCom);
	Engine::Safe_Release(m_pTerrainBufferCom);

	return CGameObject::Free();
}
