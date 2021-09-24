#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class CGameObject;
class CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer() = default;
public:
	CComponent* Get_Component(const _tchar* pComponentTag, const _uint& iIndex);
public:
	HRESULT Add_GameObject(CGameObject* pGameObject);
public:
	HRESULT Ready_Layer(void);
	_int Update_Layer(const _float& fTimeDelta);
	_int LastUpdate_Layer(const _float& fTimeDelta);
private:
	list<CGameObject*>				m_ObjectList;
	typedef list<CGameObject*>		OBJECTLIST;
public:
	static CLayer* Create(void);
	virtual _ulong Free(void);
};

END