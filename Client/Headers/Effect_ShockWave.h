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

class CEffect_ShockWave : public CGameObject
{
private:
	explicit CEffect_ShockWave(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect_ShockWave() = default;
public:
	virtual HRESULT Ready_GameObject(_vec3 vPosition);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void Expand_Radius(const _float& fTimeDelta);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
private:
	_float					m_fAngle = 0.f;
	_float					m_fSpeed = 0.1f;
	_float					m_fCreateTime = 0.f;
	_vec3					m_vecGoalScale;
	_vec3					m_vecCurScale;
private:
	HRESULT Ready_Component(void);
	void SetUp_BillBoard(void);
public:
	static CEffect_ShockWave* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPosition);
	virtual _ulong Free(void);
};

