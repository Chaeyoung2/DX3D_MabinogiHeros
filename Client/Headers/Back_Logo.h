#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CScreen_Texture;
	class CTexture;
	class CRenderer;
}

class CBack_Logo : public CGameObject
{
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBack_Logo() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CScreen_Texture*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
private:
	HRESULT Ready_Component(void);
public:
	static CBack_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

