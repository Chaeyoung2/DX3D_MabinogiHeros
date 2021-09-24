#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "Effect_Blood.h"
#include "Effect_Hit.h"

namespace Engine
{
	class CObject_Manager;
	class CTransform;
	class CMesh_Dynamic;
	class CRenderer;
	class CCollision;
	class CShader;
	class CInformation;
}

class CSoundMgr;
class CZecallion : public CGameObject
{
private:
	explicit CZecallion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CZecallion() = default;
public:
	enum BOSS_STATE {
		BOSS_RUN, BOSS_ATTACK_COMBO0, BOSS_ATTACK_COMBO1, BOSS_ATTACK_DOUBLEMASH, BOSS_ATTACK_JUMP_THROW, BOSS_ATTACK_JUMP_THROW_END,
		BOSS_TURN_RIGHT, BOSS_TURN_LEFT, BOSS_SPAWN, BOSS_IDLE, BOSS_DAMAGE_RIGHT, BOSS_DAMAGE_LEFT, BOSS_DAMAGE_RIGHT2,
		BOSS_STANDUP_FACEDOWN, BOSS_DOWN_FACEDOWN, BOSS_DYING_BACK, BOSS_DYING_FRONT
		, BOSS_GUARD_BEGIN, BOSS_GUARD_DURING, BOSS_GUARD_END
	};
	enum BOSS_PATTERN {
		PATTERN_NONE, PATTERN_IDLE, PATTERN_STALKING, PATTERN_ATTACK, PATTERN_GAURD, PATTERN_DAMAGED, PATTERN_END
	};
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void		Check_Period_ForAnimation();
	void		Set_MagicNum_InState();
	void		Set_BoneMatrix_WorldMatrix();
	void		Set_AnimationSet_InChangingState();
public:
	void		Update_BoneMatrix();
	void		Update_FollowingCollision();
	void		Update_Pattern_FollowingCollision(const _float& fTimeDelta);
	void		Update_Pattern_FollowingGapWithPlayer(const _float& fTimeDelta);
	void		Move_InState(const _float& fTimeDelta);
	void		Move_InAnimation(const _float& fTimeDelta);
	void		Create_Effect_Blood(CEffect_Blood::ZECALLION_BONE eBoneID);
	void		Create_Effect_Hit(CEffect_Hit::ZECALLION_BONE eBoneID, _int iEffectIndex);
	void		Create_Effect_ShockWave();
	void		Create_UI(_int iUIIndex);
	void		Update_Sound_FollowingState();
private:
	CRenderer*					m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CMesh_Dynamic*			m_pMeshCom = nullptr;
	CCollision*					m_pCollisionCom = nullptr;
	CCollision*					m_pCollisionCom_ForCombat = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CInformation*				m_pInformationCom = nullptr;
	// 콜라이더
	CCollision*					m_pCollisionCom_Front = nullptr; 		CCollision*			m_pCollisionCom_Head = nullptr;
	CCollision*					m_pCollisionCom_Left = nullptr;			CCollision* 			m_pCollisionCom_Right = nullptr;
	CCollision*					m_pCollisionCom_Left_Hand = nullptr;  CCollision*			m_pCollisionCom_Right_Hand = nullptr;
	CCollision*					m_pCollisionCom_Left_Calf = nullptr;  CCollision*			m_pCollisionCom_Right_Calf = nullptr;
	CCollision*					m_pCollisionCom_Foot = nullptr;
	CTransform*				m_pTransCom_Front = nullptr; 				CTransform*	m_pTransCom_Head = nullptr;
	CTransform*				m_pTransCom_Left = nullptr;					CTransform* 	m_pTransCom_Right = nullptr;
	CTransform*				m_pTransCom_Left_Hand = nullptr;			CTransform*	m_pTransCom_Right_Hand = nullptr;
	CTransform*				m_pTransCom_Left_Calf = nullptr;			CTransform*	m_pTransCom_Right_Calf = nullptr;
	CTransform*				m_pTransCom_Foot = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CSoundMgr*			m_pSoundMgr = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float							m_fAngle = 270.f;
	_float							m_fSpeed = 5.5f;
	_float							m_fOriSpeed = 0.f;
	BOSS_STATE				m_eCurrentState = BOSS_IDLE;
	BOSS_STATE				m_ePreviousState = BOSS_IDLE;
	BOSS_PATTERN			m_ePattern = PATTERN_NONE;
	_double						m_Period = 0;
	_double						m_MagicNum = 0.25;
	_bool							m_bIsSetBoneMatrix_ForCollider = false;
	_float							m_fAdvanceTime = 1.2f;
private: // 뼈 matrix.
	const _matrix*			m_pBoneMatrix_Left_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Right_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Head_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Front_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Left_Hand_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Right_Hand_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Left_Calf_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Right_Calf_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Foot_Parent = nullptr;
private:
	_bool							m_bIsCombatMode = false;
	_bool							m_bIsStandUpFaceDown_Completed_OnInit = false;
	_bool							m_bIsSettingCompleted_Run_AfterStandUpFaceDown = false;
	_bool							m_bIsPermitted_Collision = false;
	// 상태.
	_bool							m_bIsRunToPlayer = false;
	_bool							m_bIsBreakAfterAttack = false;
	_bool							m_bIsStartWalking_Init = false; //처음 컷씬에서

	_float							m_fTimeDelta_Walking_Init = 0.f;

	_float							m_fTimeDelta_After_STANDUPFACEDOWN = 0.f;
	_float							m_fTimeDelta_ForDoingNotInitCollision = 0.f;
	_float							m_fTimeDelta_PatternIdleToAttack = 0.f;
	_float							m_fTimeDelta_ForBreak_AfterAttack = 0.f;

	_vec3							m_vecOriLook;
	_bool							m_bIsSettingAngleCompleted_InRunningToPlayer = false; // 각도는 계속 바뀌면 안되고, 플레이어와 각도가 어긋날 때마다 true 켜주는 식으로.
	_float							m_fGoalAngle = 0.f;

	// 대시
	//	_bool							m_bIsSettingCompletedLook_ForDash = false;
	_bool							m_bIsSettingAngleCompleted_InDashingToPlayer = false;

	_bool							m_bIsDashInAttack0_0 = false;
	_bool							m_bIsDashInAttack0_1 = false;
	_bool							m_bIsDashInAttack0_2 = false;
	_bool							m_bIsDashInAttack1_0 = false;
	_bool							m_bIsDashInDoubleMash_0 = false;
	_bool							m_bIsDashInDoubleMash_1 = false;
	_bool							m_bIsDashInDoubleMash_2 = false;
	_bool							m_bIsDashInSpawn = false;
	_float							m_fTimeDeltaDash = 0.f;


	// 각도
	_bool							m_bIsSettingAngleCompleted_BeforeAttacking = false;

	// 충돌
	_bool							m_bIsCollided_WithPlayerWhole = false;
	_bool							m_bIsPermitted_Collision_WithBlade = false;
	_float							m_fTimeDelta_Invincible_WithBlade = 0.f;
	SPHEREINFO				m_tSphereInfo_Whole;

	// 공격.
	_uint								m_iAttackCount = 0;

	// 가드.
	_float							m_fTimeDelta_Gaurd = 0.f;
	_bool							m_bIsGaurding = false;

	// 점프.
	_bool							m_bIsJumpInAttackJumpThrow_0 = false;
	_bool							m_bIsJumpInAttackJumpThrow_1 = false;

	// 이펙트.
	_bool							m_bIsCompletedToCreateEffect_ShockWave = false;
	_bool							m_bIsCompletedToCreateEffect_AmingPoint = false;

	// UI.
	_bool							m_bIsCompletedToCreateUI_Gaurd2 = false;
	_bool							m_bIsCompletedToCreateUI_Gaurd3 = false;
	_bool							m_bIsCompletedToCreateUI_ZecallionHPBar = false;

	// 사운드
	_bool							m_bIsCompletedToPlaySound_PropBegin = false;
	_bool							m_bIsCompletedToPlaySound_SpawnWalk0 = false;
	_bool							m_bIsCompletedToPlaySound_SpawnWalk1= false;
	_bool							m_bIsCompletedToPlaySound_Chain = false;
	_bool							m_bIsCompletedToPlaySound_DoorStart = false;
	_bool							m_bIsCompletedToPlaySound_DoorDown = false;
	_bool							m_bIsCompletedToPlaySound_Run = false;
	_bool							m_bIsCompletedToPlaySound_AttackCombo0 = false;
	_bool							m_bIsCompletedToPlaySound_AttackCombo1 = false;
	_bool							m_bIsCompletedToPlaySound_DoubleMash0 = false;
	_bool							m_bIsCompletedToPlaySound_DoubleMash1 = false;
	_bool							m_bIsCompletedToPlaySound_JumpThrow = false;
	_bool							m_bIsCompletedToPlaySound_GaurdBegin = false;
	_bool							m_bIsCompletedToPlaySound_GaurdDuring = false;

	_bool							m_bIsCompletedToPlaySound_Spawn_Shout = false;
	_bool							m_bIsCompletedToPlaySound_AttackCombo0_Shout = false;
	_bool							m_bIsCompletedToPlaySound_AttackCombo1_Shout = false;
	_bool							m_bIsCompletedToPlaySound_DoubleMash_Shout = false;
	_bool							m_bIsCompletedToPlaySound_JumpThrow_Shout0 = false;
	_bool							m_bIsCompletedToPlaySound_JumpThrow_End_Shout = false;
	_bool							m_bIsCompletedToPlaySound_GaurdBegin_Shout = false;
	_bool							m_bIsCompletedToPlaySound_GaurdEnd_Shout = false;

	_bool							m_bIsCompletedToPlaySound_Clear = false;
private:
	HRESULT Ready_Component(void);
public:
	static CZecallion* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

