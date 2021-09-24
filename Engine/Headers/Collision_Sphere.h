#pragma once

#include "Component.h"

BEGIN(Engine)

class CMesh;
class CTransform;
class DLL_EXPORT CCollision_Sphere : public CComponent
{
public:
	enum COLOR { COL_GREEN, COL_RED, COL_END };
private:
	explicit CCollision_Sphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollision_Sphere(const CCollision_Sphere& rhs);
	virtual ~CCollision_Sphere() = default;
public:
// 	OBBINFO Get_OBBInfo(void) const {
// 		return m_OBBInfo;
// 	}
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
	HRESULT SetUp_Collision(CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex);
	_bool Collision_SPHERE(CCollision_Sphere* pTargetCollsion);

	void Render_Collider(void);
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	LPDIRECT3DTEXTURE9				m_pTexture[COL_END] = { nullptr };
	ID3DXMesh*			m_pBoundingSphere;
	float				m_fRadius;
private:
	_vec3								m_vMin, m_vMax;
	const _matrix*					m_pWorldMatrix, *m_pWorldMatrix_NotRot;
	_bool								m_isColl = false;

private:
	void SetUp_Axis(void);
public:
	virtual CComponent* Clone(void);
	static CCollision_Sphere* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END