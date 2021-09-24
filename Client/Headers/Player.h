#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMesh_Dynamic;
	class CRenderer;
	class CCollision;
	class CNavigation;
	class CShader;
	class CInformation;
}

class CSoundMgr;
class CKeyMgr;
class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer() = default;
private:
// 	enum PLAYER_STATE {
// 		PLAYER_IDLE, PLAYER_DRINK, PLAYER_ATTACK_RUN_SPRINT, PLAYER_ATTACK01_A, PLAYER_ATTACK01_B, PLAYER_ATTACK02, PLAYER_ATTACK03, PLAYER_ATTACK04,
// 		PLAYER_RUN_SPRINT, PLAYER_RUN_SPRINT_STOP, PLAYER_BATTLE_RUN, PLAYER_BATTLE_RUN_STOP, 
// 		PLAYER_DANCE, PLAYER_DAMAGE_LIGHT_RIGHT, PLAYER_DAMAGE_LIGHT_LEFT, PLAYER_DAMAGE_LIGHT_FRONT, PLAYER_DAMAGE_LIGHT_BACK,
// 		PLAYER_DAMAGE_LAUNCH_STRONG, PLAYER_DOWN_BACK_IDLE, PLAYER_LAUNCH_BEGIN, PLAYER_LAUNCH_DURING, PLAYER_LAUNCH_END
// 	};
	enum PLAYER_STATE {
		Player_idle, Player_idle_mb, Player_Run, Player_Run_Stop, Player_Teleport_mana_stone,
		Player_Damage_light_front, Player_Damage_light_right, Player_Damage_light_back, Player_Damage_light_left,
		Player_Damage_middle_front_begin, Player_Damage_middle_front_end, Player_Damage_middle_back_begin, Player_Damage_middle_back_end,
		Player_Take_snatch, Player_Attack1, Player_Attack2, Player_Attack3, Player_Attack_Strong1, Player_Attack_Strong4, Player_Attack_Strong5, Player_Mana_blade,
		Player_Attack_mb_strong1, Player_Attack_mb_strong2, Player_Attack_mb_strong3, Player_Attack_mb_strong4, Player_Attack_mb_strong5, Player_Attack_mb_strong6,
		Player_Attack_doomsayer, Player_Attack_Resonance, Player_Attack_ruin_blade, Player_Attack_castlet_dimension_hole, Player_Attack_lord_of_mana,
		Player_Dark_knight_transformation_begin1, Player_Dark_knight_transformation_begin2, Player_Dark_knight_transformation_end1, Player_Speedy_move, Player_Mana_region
	};
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
public:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void		Set_MagicNum_InState();
	void		Set_BoneMatrix_WorldMatrix();
public:
	void	Check_KeyInput(const _float& fTimeDelta);
	void	Check_AnimationInState();
	void  Create_Effect();
	void Create_UI();
	void	Check_Period_ForAnimation();
public:
	void	Move_Speed(const _float& fTimeDelta);
	void	Move_InAnimation(const _float& fTimeDelta);
public:
	void	Update_BoneMatrix();
	void Update_FollowingCollision();
private:
	CRenderer*					m_pRendererCom = nullptr;
	CTransform*					m_pTransCom = nullptr;	
	CMesh_Dynamic*			m_pMeshCom = nullptr;
	CCollision*					m_pCollisionCom_Front = nullptr; 	CCollision* 	m_pCollisionCom_Right = nullptr;
	CCollision*					m_pCollisionCom_Left = nullptr;		CCollision*	m_pCollisionCom_Back = nullptr;
	CCollision*					m_pCollisionCom_Whole = nullptr;
	CTransform*					m_pTransCom_Front = nullptr; 	CTransform* 	m_pTransCom_Right = nullptr;
	CTransform*					m_pTransCom_Left = nullptr;		CTransform*	m_pTransCom_Back = nullptr;
	CInformation*				m_pInformationCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;
	CKeyMgr*						m_pKeyManager = nullptr;
	CSoundMgr*				m_pSoundMgr = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float							m_fAngle = 0.f;
	_float							m_fSpeed = 7.5f;
	PLAYER_STATE				m_eCurrentState = Player_idle;
	PLAYER_STATE				m_ePreviousState = Player_idle;
private: // 애니메이션.
	_double						m_Period = 0;
	_double						m_MagicNum = 0.25;
	_float							m_fAdvanceTime = 1.2f;
	_float							m_fTimeSpeedyMove = 0.f;
	_float							m_fOriSpeed = 0.f;
	_bool							m_bIsStartingTurn = false;
	_float							m_fAngleAcc_Turn = 0.f;
	_float							m_fOriAngle = 0.f;
	_float							m_fTargetAngle = 0.f;
	_float							m_fTimeDelta_Running = 0.f;
	_bool							m_bIsPermitted_Ani_RunStop = false;
	_bool							m_bIsStart_toCountRunning = false;
private:
	_bool							m_bIsStrongMode = false; 
	_bool							m_bIsOnManaBlade = false; 
	_bool							m_bIsMovingSpeed = false;
	_bool							m_bBeingDamagedMiddleFront = false; // 타임 델타 세기 위함.
	_float							m_fTimeDelta_BeingDamagedMiddleFront = 0.f;
	_bool							m_bIsNotPermitted_ToPlayAnimation = false;
	// Dash.
	_bool							m_bIsDashInAttack1 = false;
	_bool							m_bIsBackInAttack1 = false;
	_bool							m_bIsDashInAttack2 = false;
	_bool							m_bIsBackInAttack2 = false;
	_bool							m_bIsDashInAttack3 = false;
	_bool							m_bIsBackInAttack3 = false;
	_bool							m_bIsDashInAttackStrong1 = false;
	_bool							m_bIsBackInAttackStrong1 = false;
	_bool							m_bIsDashInAttackStrong4 = false;
	_bool							m_bIsBackInAttackStrong4 = false;
	_bool							m_bIsDashInAttackStrong5 = false;
	_bool							m_bIsBackInAttackStrong5 = false;
	_bool							m_bIsSetBoneMatrix_ForCollider = false;
	_int							m_iAttackAcc = 0; // 1,2,3 타 구현 위함. true 일 때 LBUTTON 눌리면 다음 타로 활성화.
	// Back.
	_bool							m_bIsBack_InLightFrontDamaged = false;
	_bool							m_bIsBack_InMiddleFrontDamaged = false;

	// 충돌.
	_bool						m_bIsCollided_WithBoss = false;
	_bool						m_bIsPermitted_Collision_WithAxe = false;
	_bool						m_bIsPermitted_Collision_WithLeftHand = false;
	_float						m_fTimeDelta_Invincible_WithAxe = 0.f;
	_float						m_fTimeDelta_Invincible_WithLeftHand = 0.f;
	SPHEREINFO			m_tSphereInfo_Whole;
	_bool						m_bIsPermitted_OBB_DependingOnWholeSphere = false;
private: // 뼈 matrix.
	const _matrix*			m_pBoneMatrix_Left_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Left_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Right_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Right_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Back_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Back_Parent = nullptr;
	const _matrix*			m_pBoneMatrix_Front_Parent = nullptr;
	const _matrix*			m_pWorldMatrix_Front_Parent = nullptr;
private:
	HRESULT Ready_Component(void);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

