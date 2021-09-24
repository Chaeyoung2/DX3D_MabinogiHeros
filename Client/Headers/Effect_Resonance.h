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

class CEffect_Resonance : public CGameObject
{
private:
	explicit CEffect_Resonance(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect_Resonance() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom_0 = nullptr;
	CTexture*				m_pTextureCom_1 = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float					m_fAngle = 0.f;
	_float					m_fFrame = 0.f;
	_float					m_fRotSpeed = 0.5f;
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
	_bool					m_bIsSettingBoneMatrix_Completed = false;
private:
	HRESULT Ready_Component(void);
	void SetUp_BillBoard(void);
public:
	static CEffect_Resonance* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

