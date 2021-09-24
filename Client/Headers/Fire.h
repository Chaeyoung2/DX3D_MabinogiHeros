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

class CFire : public CGameObject
{
private:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFire() = default;
public:
	virtual HRESULT Ready_GameObject(_int iDrawID, _vec3 vPosition);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
private:
	_float					m_fAngle = 0.f;
	_float					m_fFrame = 0.f;
	_float					m_fMaxFrame = 0.f;
	_int						m_iDrawID = 0;
	_float					m_fFrameTime = 0.f;
	_float					m_fCreateTime = 0.f;
private:
	HRESULT Ready_Component(void);
	void SetUp_BillBoard(void);
public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iDrawID, _vec3 vPosition);
	virtual _ulong Free(void);
};

