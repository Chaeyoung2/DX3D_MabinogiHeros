#pragma once

#include "Defines.h"
#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CMesh_Dynamic;
	class CRenderer;
}

class CGreatSword : public CGameObject
{
private:
	explicit CGreatSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGreatSword() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CMesh_Dynamic*			m_pMeshCom = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float						m_fAngle = 0.f;
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
private:
	HRESULT Ready_Component(void);
public:
	static CGreatSword* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

