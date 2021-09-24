#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTrail_Buffer;
	class CTexture;
	class CRenderer;
	class CShader;
}


class CTrail_LongBlade : public CGameObject
{
private:
	explicit CTrail_LongBlade(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrail_LongBlade() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void Point_Memorize(const _float& fTimeDelta);
private:	
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CTrail_Buffer*			m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
private:
	LPD3DXLINE				m_pLine;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float						m_fAngle = 0.f;
	_float						m_fCreateTime = 0.f;
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
	_bool						m_bIsSettingBoneMatrix_Completed = false;
private:
	const _matrix*			m_pmatTarget = nullptr;
private:
	typedef list<D3DXVECTOR3>		POINTLIST;
	POINTLIST		m_vPointlist;
	_float					m_fLength = 0.f;
	_float					m_fAccTime = 0.f;
private:
	float		m_fDetail = 0.f;
	float		m_fAlpha = 0.f;
private:
	HRESULT Ready_Component(void);
public:
	static CTrail_LongBlade* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

