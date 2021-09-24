#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CTransform : public CComponent
{
public:
	enum INFOMATION { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POSITION, INFO_END };
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:
	const _matrix* Get_WorldMatrix(void) {
		return &m_matWorld_Parent;
	}
	const _matrix* Get_WorldMatrix_NotRot(void) {
		return &(m_matWorld_NotRot_Parent);
	}
	const _vec3* Get_Infomation(INFOMATION eInfo) const {
		return (_vec3*)&m_matWorld.m[eInfo][0];
	}
// 	const _vec3* Get_ParentMatrix_ForCollisionBox() const {
// 		return (_vec3*)&m_matWorld_Parent.m[INFO_POSITION][0];
// 	}
	const _matrix* Get_ParentMatrix_ForCollisionBox() const {
		return &m_matWorld_Parent;
	}
	_float Get_AngleX(void);
	_float Get_AngleY(void);
	_vec3 Get_ScaleInfo(void);
public:
	void Set_Infomation(INFOMATION eInfo, _vec3* pInfomation) {
		memcpy(&m_matWorld.m[eInfo][0], pInfomation, sizeof(_vec3));
	}
	void Set_Parent(_matrix matParent) {
		m_matParent = matParent;
	}
	void SetUp_OnGraphicDev(void);
	void Invalidate_Worldmatrix(void);
	void SetUpHeight_OnBuffer(const _vec3* pVertexPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwInterval);

public:
	void Scaling(_vec3* pScale); // 확대축소한다
	void RotationX(_float fAngle);
	void RotationY(_float fAngle);
	void RotationZ(_float fAngle);
	void Go_Staight(_float fDistanceToFrame);






	// Go_Straight(); // 바라보고있는 방향으로 간다.
	// Get_Scaling(); // 크게 확대비율을 얻어온다.
	// Get_Angle(); // 몇도나 회전했는지


public:
	virtual HRESULT Ready_Component(void);

private:
	_matrix				m_matWorld;
	_matrix				m_matWorld_Parent;

	_matrix				m_matWorld_NotRot;
	_matrix				m_matWorld_NotRot_Parent;

	_matrix				m_matParent;
private:
	void Make_WorldMatrix_NotRot(void);
public:
	virtual CComponent* Clone(void);
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);

};

END