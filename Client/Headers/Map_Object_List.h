#pragma once

#include "Defines.h"
#include "Engine_Defines.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CPicking;
	class CObject_Manager;
	class CTerrain_Buffer;
}
class CMyToolView;
class CMap_Object_List : public CGameObject
{
public:
	explicit CMap_Object_List(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMap_Object_List() = default;
public:
	vector<PMAPOBJ>*		Get_vecMapObjectList() {
		return &m_MapObjectList;
	}
public:
	void Set_ObjectManager(Engine::CObject_Manager* pObject_Manager) {
		m_pObject_Manager = pObject_Manager;
	}
	void Set_SettingCompleted(_bool bIsSettingCompleted)
	{
		m_bIsSetCompleted = bIsSettingCompleted;
	}

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	void			MapObject_Create(_vec3 vecScale, _vec3 vecRot, const _int & iDrawID, const _int & iOption);
	void			MapObject_Load(PMAPOBJ pMapObj);
	void			MapObject_ForPreview_Create(_vec3 vecScale, _vec3 vecRot , const _int & iDrawID, const _int & iOption);
	void			MapObject_ForPreview_Delete();
private:
	HRESULT Ready_Component(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CTerrain_Buffer*			m_pTerrainBufferCom = nullptr;
	CPicking*				m_pPickingCom = nullptr;
private:
	_int		m_iVtxCntX = 129;
	_int		m_iVtxCntZ = 257;
	_int		m_iInterval = 1;
	//_bool	m_bStartPicking = false;
private:
	vector<PMAPOBJ>	m_MapObjectList;
	CMyToolView*		m_pMainView;
	Engine::CObject_Manager* m_pObject_Manager = nullptr;
public:
	CGameObject*		m_pForPreview = nullptr;
public:
	_bool					m_bIsSetCompleted = false;
	_bool					m_bIsSetUpObjectCompleted = false; // 미리보기 객체 최초 한번 생성하기 위해.
public:
	static CMap_Object_List* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

