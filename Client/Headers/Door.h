#pragma once

#include "Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMesh_Static;
	class CRenderer;
	class CCollision;
	class CShader;
}

class CDoor : public CGameObject
{
public:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDoor() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	void Move_DependingOnZecallion(const _float& fTimeDelta);
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CMesh_Static*			m_pMeshCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float		m_fAngle = 0.f;
	_float		m_fSpeed = 90.f;
	_vec3		m_vecGoalPos;
	_vec3		m_vecOriPos;
	_vec3		m_vecOriPos_ToUp;
	_vec3		m_vecGoalPos_ToUp;
	_vec3		m_vecGoalPos_ToDownSecond;
	_vec3		m_vecOriPos_ToDownSecond;
	_bool		m_bIsToDown_First = false;
	_bool		m_bIsToDown_Second = false;
	_bool		m_bIsToDown_Completed = false;
	_bool		m_bIsToUp = false;
	_float		m_fJumpSpeed = 10.f;
	_float		m_fJumpAcc = 5.f;
	_float		m_fCurrentPosY = 0.f;
public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

