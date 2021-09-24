#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class CComponent_Manager;
class DLL_EXPORT CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject() = default;
public:
	CComponent* Get_Component(const _tchar* pComponentTag);
	_float Get_ViewZ(void) const { 
		return m_fViewZ;}
	void Set_IsDead(_bool bIsDead) {
		m_bIsDead = bIsDead;
	}
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev = nullptr;
protected:
	CComponent_Manager*						m_pComponent_Manager;
protected:
	map<const _tchar*, CComponent*>			m_mapComponent;
	typedef map<const _tchar*, CComponent*> MAPCOMPONENT;
protected:
	_float									m_fViewZ = 0.f;
	_bool									m_bIsDead = false;
protected:
	void Compute_ViewZ(_vec3 vPosition);
private:
	CComponent* Find_Component(const _tchar* pComponentTag);
public:
	virtual _ulong Free(void);
};

END