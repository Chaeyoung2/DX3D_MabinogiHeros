#include "..\Headers\GameObject.h"
#include "Component_Manager.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
{
	m_pGraphicDev->AddRef();
	m_pComponent_Manager->AddRef();
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{	

	return Find_Component(pComponentTag);
}

HRESULT CGameObject::Ready_GameObject(void)
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject(void)
{
}

void CGameObject::Compute_ViewZ(_vec3 vPosition)
{
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3			vCam_Position;
	memcpy(&vCam_Position, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vPosition - vCam_Position));
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFind_Tag(pComponentTag));

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

_ulong CGameObject::Free(void)
{
	for (auto& Pair : m_mapComponent)
	{
		Safe_Release(Pair.second);
	}
	m_mapComponent.clear();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pComponent_Manager);

	return _ulong();
}
