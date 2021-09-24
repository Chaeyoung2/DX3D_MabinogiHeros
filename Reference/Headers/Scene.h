#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent_Manager;
class CObject_Manager;
class CRenderer;
class DLL_EXPORT CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene() = default;
public:
	virtual HRESULT Ready_Scene(const _uint& eSceneID, const _tchar* pRendererTag);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
protected:
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CRenderer*				m_pRenderer = nullptr;

public:
	virtual _ulong Free(void);
};

END