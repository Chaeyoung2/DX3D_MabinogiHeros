#pragma once

#include "Defines.h"
#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CZecallion_HPBar_Font : public CGameObject
{
public:
	explicit	CZecallion_HPBar_Font(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CZecallion_HPBar_Font() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float					m_fX2, m_fY2, m_fSizeX2, m_fSizeY2;
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CZecallion_HPBar_Font* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

