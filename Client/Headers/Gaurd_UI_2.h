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

class CGaurd_UI2 : public CGameObject
{
public:
	explicit	CGaurd_UI2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGaurd_UI2() = default;
public:
	virtual HRESULT Ready_GameObject(_int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CShader*				m_pShaderCom = nullptr;
private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float					m_fCreateTime = 0.f;
	_float					m_fAlpheValue = 1.f;
	_bool					m_bIsDead = false;
	_bool					m_bIsInit = true;
	_int						m_iDrawID = 0;
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CGaurd_UI2* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);
	virtual _ulong Free(void);
};

