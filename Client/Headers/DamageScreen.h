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

class CDamageScreen : public CGameObject
{
public:
	explicit	CDamageScreen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDamageScreen() = default;
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
	_float					m_fCreateTime = 0.f;
	_float					m_fAlphaValue = 1.f;
	_bool					m_bIsAlphaValueUp = false;
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CDamageScreen* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

