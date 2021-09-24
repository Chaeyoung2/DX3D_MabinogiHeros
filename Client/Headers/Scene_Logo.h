#pragma once

#include "Defines.h"
#include "Scene.h"

class CLoading;
class CScene_Logo : public Engine::CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene(const _uint& eSceneID, const _tchar* pRendererTag);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
public:
	CLoading* Get_Loading() {
		if (nullptr == m_pLoading)
			return nullptr;
		return m_pLoading;
	}
private:
	CLoading*				m_pLoading = nullptr;
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Navigation(void);
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& eSceneID, const _tchar* pRendererTag);
	virtual _ulong Free(void);
};

