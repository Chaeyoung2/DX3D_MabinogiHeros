#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTerrain_Buffer;
	class CTexture;
	class CRenderer;
	class CPicking;
	class CShader;
	class CFrustum;
}

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CTerrain_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom_1 = nullptr;
	CTexture*				m_pTextureCom_3 = nullptr;
	CTexture*				m_pFilterCom = nullptr;
	CTexture*				m_pEffectCom = nullptr;
	CPicking*				m_pPickingCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CFrustum*				m_pFrustumCom = nullptr;

// private:
// 	D3DMATERIAL9			m_MtrlInfo;
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT Optimize_Buffer(void);
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

