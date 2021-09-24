#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class CLayer;
class CGameObject;
class DLL_EXPORT CObject_Manager : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	CComponent*	 Get_Component(const _uint& iSceneIdx, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex);
public:
	HRESULT Reserve_ContainerSize(const _uint& iMaxSize);
	HRESULT Add_GameObject(const _uint& iSceneIdx, const _tchar* pLayerTag, CGameObject* pGameObject);
	_int Update_ObjectMgr(const _float& fTimeDelta);
	_int LastUpdate_ObjectMgr(const _float& fTimeDelta);
	void Release_GameObject(const _uint& iSceneIdx);
	
private:	
	map<const _tchar*, CLayer*>*			m_pMapLayer = nullptr;
	typedef map<const _tchar*, CLayer*>		MAPLAYER;
private:
	_uint				m_iMaxSize = 0;
private:
	CLayer* Find_Layer(const _uint& iSceneIdx, const _tchar* pLayerTag);
public:
	virtual _ulong Free(void);
};

END