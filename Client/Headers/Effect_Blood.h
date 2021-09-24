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

class CEffect_Blood : public CGameObject
{
private:
	explicit CEffect_Blood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect_Blood() = default;
public:
	enum ZECALLION_BONE { BONE_LEFT_CALF, BONE_RIGHT_CALF, BONE_RIGHT_HAND, BONE_LEFT_HAND, BONE_FRONT, BONE_END };
public:
	virtual HRESULT Ready_GameObject(ZECALLION_BONE eBoneID, _vec3 vBloodPosition, _int iDrawID);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRect_Texture*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom_1 = nullptr;
	CTexture*				m_pTextureCom_2 = nullptr;
	CTexture*				m_pTextureCom_3 = nullptr;
	CShader*					m_pShaderCom = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_int						m_iDrawID = 1;
	_float					m_fAngle = 0.f;
	_float					m_fFrame = 0.f;
	_float					m_fMaxFrame = 0.f;
	_float					m_fRotSpeed = 0.5f;
	_float					m_fCreateTime = 0.f;
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
	_bool					m_bIsSettingBoneMatrix_Completed = false;
	ZECALLION_BONE	m_eBoneID = BONE_END;
private:
	HRESULT Ready_Component(void);
	void SetUp_BillBoard(void);
public:
	static CEffect_Blood* Create(LPDIRECT3DDEVICE9 pGraphicDev, ZECALLION_BONE eBoneID, _vec3 vBloodPosition, _int iDrawID);
	virtual _ulong Free(void);
};

