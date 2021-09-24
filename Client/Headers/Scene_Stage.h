#pragma once

#include "Defines.h"
#include "Scene.h"

class CScene_Stage : public Engine::CScene
{
private:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Stage() = default;
public:
	virtual HRESULT Ready_Scene(const _uint& eSceneID, const _tchar* pRendererTag);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
private:
	HRESULT Ready_LightInfo(void);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Zecallion(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Door(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Fire(const _tchar* pLayerTag);
public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& eSceneID, const _tchar* pRendererTag);
	virtual _ulong Free(void);
};

