#pragma once

#include "Component.h"

BEGIN(Engine)

class CMesh;
class CTransform;
class DLL_EXPORT CCollision : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE };
	enum COLOR { COL_GREEN, COL_RED, COL_END };
private:
	explicit CCollision(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollision(const CCollision& rhs);
	virtual ~CCollision() = default;
public:
	OBBINFO Get_OBBInfo(void) const {
		return m_OBBInfo;
	}
	SPHEREINFO Get_SPHEREInfo(void) const {
		return m_SphereInfo;
	}
	const _vec3* Get_Min() {
		return &m_vMin;
	}
	const _vec3* Get_Max() {
		return &m_vMax;
	}
	const _matrix* Get_WorldMatrix() {
		return m_pWorldMatrix;
	}
	const _matrix* Get_WorldMatrix_NotRot() {
		return m_pWorldMatrix_NotRot;
	}

public:
	HRESULT Ready_Collision(void);
	HRESULT SetUp_Collision(TYPE eType, CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex);
//	HRESULT SetUp_Collision_Ex(TYPE eType, CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex, _vec3& vMin = _vec3(0.f,0.f,0.f), const _vec3 vMax = _vec3(0.f, 0.f, 0.f));
	HRESULT SetUp_Collision_Ex(TYPE eType, CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex, _vec3& vScale = _vec3(1.f, 1.f, 1.f), _vec3& vTrans = _vec3(0.f, 0.f, 0.f));
	HRESULT SetUp_Collision_Sphere(TYPE eType, CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex, SPHEREINFO pSphereInfo);
	// AABB : 회전하지 않은 큐브형태의 박스.
	_bool Collision_AABB(CCollision* pTargetCollsion);
	_bool Collision_OBB(CCollision* pTargetCollsion);
	_bool Collision_Sphere(CCollision* pTargetCollision);
	void Render_Collider(void);
	void Update_SpherePos(_vec3 vPosition);
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	LPDIRECT3DTEXTURE9				m_pTexture[COL_END] = { nullptr };
	LPD3DXMESH							m_pSphereMesh = nullptr;
	LPD3DXBUFFER						m_pAdjacency = nullptr;
private:
	_vec3							m_vMin, m_vMax;
	const _matrix*					m_pWorldMatrix, *m_pWorldMatrix_NotRot;
	_bool							m_isColl = false;
	TYPE							m_eType;
private:
	OBBINFO							m_OBBInfo;
	SPHEREINFO					m_SphereInfo;
private:
	const _matrix*			m_pBoneMatrix_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Parent = nullptr;
private:
	void SetUp_Axis(void);
public:
	virtual CComponent* Clone(void);
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END