#pragma once

#include "Defines.h"
#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CMesh_Static;
	class CRenderer;
	class CCollision;
	class CShader;
}

class CBlade : public CGameObject
{
private:
	explicit CBlade(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBlade() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CMesh_Static*			m_pMeshCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float						m_fAngle = 0.f;
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CBlade* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

