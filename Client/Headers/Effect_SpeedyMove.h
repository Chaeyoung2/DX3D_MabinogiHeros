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

class CEffect_SpeedyMove : public CGameObject
{
private:
	explicit CEffect_SpeedyMove(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect_SpeedyMove() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float					m_fAngle = 0.f;
	_float					m_fFrame = 0.f;
	_float					m_fRotSpeed = 0.5f;
	_float					m_fCreateTime = 0.f;
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
	_bool					m_bIsSettingBoneMatrix_Completed = false;
private:
	HRESULT Ready_Component(void);
	void SetUp_BillBoard(void);
public:
	static CEffect_SpeedyMove* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

