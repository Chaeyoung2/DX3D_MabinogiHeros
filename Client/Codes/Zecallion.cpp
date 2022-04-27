#include "stdafx.h"
#include "..\Headers\Zecallion.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "NameCard.h"
#include "Gaurd_UI.h"
#include "Effect_Hit.h"
#include "Effect_ShockWave.h"
 #include "SoundMgr.h"
#include "Zecallion_HPBar.h"
#include "Zecallion_HPBar2.h"
#include "Zecallion_HPBar_Font.h"
#include "Gaurd_UI_2.h"
#include "Clear_UI.h"

CZecallion::CZecallion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_eCurrentState(BOSS_IDLE)
	, m_pCollisionCom(nullptr)
	, m_pCollisionCom_Front(nullptr)
	, m_pCollisionCom_Left(nullptr)
	, m_pCollisionCom_Right(nullptr)
	, m_pCollisionCom_Head(nullptr)
	, m_pCollisionCom_Left_Hand(nullptr)
	, m_pCollisionCom_Right_Hand(nullptr)
	, m_pCollisionCom_Left_Calf(nullptr)
	, m_pCollisionCom_Right_Calf(nullptr)
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pInformationCom(nullptr)
	, m_pSoundMgr(CSoundMgr::GetInstance())
{
	m_fSpeed = 10.f;
	m_fOriSpeed = m_fSpeed;
};

HRESULT CZecallion::Ready_GameObject(void)
{
	// 컴포넌트 준비.
	if (FAILED(Ready_Component()))
		return E_FAIL;
	// 위치 세팅.
	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(60.0f, 0.0f, 235.f));
	// 크기 세팅.
	m_pTransCom->Scaling(&_vec3(0.05f, 0.05f, 0.05f));
	// 첫 애니메이션은 IDLE, 초기 MagicNumber == 0.25
	m_pMeshCom->Set_AnimationSet(BOSS_IDLE, m_MagicNum);

	m_pTransCom->RotationY(m_fAngle);

	return NOERROR;
}

_int CZecallion::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransCom || nullptr == m_pRendererCom || nullptr == m_pMeshCom)
		return -1;

	// 체력 없으면 죽자!
	OBJ_INFO tInfo = m_pInformationCom->Get_ObjInfo();
	if (tInfo.fHP <= 0)
	{
		if (m_bIsCompletedToPlaySound_Clear == false)
		{
			m_eCurrentState = BOSS_DYING_BACK;
			m_pSoundMgr->StopSoundAll();
			m_pSoundMgr->PlayBGM(L"bgm_charles.wav", CHANNEL_BACKGROUND, 0.8f);
			Create_UI(4);
			m_bIsCompletedToPlaySound_Clear = true;
		}

	}
	if (isDead) {
		return 1;
	}


	// 상태에 따른 사운드.
	Update_Sound_FollowingState();

	//  처음에 켜지자마자 충돌이 돼.. ㅜㅜ 시간 세서 한 3초뒤에 충돌 검사하도록.
	if (m_bIsPermitted_Collision == false)
		m_fTimeDelta_ForDoingNotInitCollision += fTimeDelta;
	if (m_fTimeDelta_ForDoingNotInitCollision >= 2.f)
	{
		m_bIsPermitted_Collision = true;
	}

	// 상태에 따른 매직 넘버 세팅.
	Set_MagicNum_InState();

	// Collision 컴포넌트들 부모 행렬, 뼈 행렬 세팅. 최초 한번만.
	if (m_bIsSetBoneMatrix_ForCollider == false)
	{
		Set_BoneMatrix_WorldMatrix();
		m_bIsSetBoneMatrix_ForCollider = true;
	}

	// 애니메이션 재생.
	m_pMeshCom->Play_AnimationSet(fTimeDelta * m_fAdvanceTime);

	// SetAnimationSet은 상태가 바뀔 때 ! 한번만 호출하기 위해서.
	Set_AnimationSet_InChangingState();


	// 각도에 따라 RotationY.
	m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

	// 현재 애니메이션의 Period값을 얻어와서 동작이 끝날 때쯤 Idle상태로 변환.
	Check_Period_ForAnimation();

	// 지형에 태우기 위함.
	CVIBuffer* pBuffer = (CVIBuffer*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBuffer)
		return -1;
	tuple<_ulong, _ulong, _ulong>	BufferInfo = pBuffer->Get_BufferInfomation();
	if (m_eCurrentState != m_bIsJumpInAttackJumpThrow_0 || m_eCurrentState != m_bIsJumpInAttackJumpThrow_1)
		m_pTransCom->SetUpHeight_OnBuffer(pBuffer->Get_VertexPos(), get<0>(BufferInfo), get<1>(BufferInfo), get<2>(BufferInfo));


	// 충돌에 따른 젝칼리온 상태 구분.
	Update_Pattern_FollowingCollision(fTimeDelta);

	// 플레이어와의 거리에 따른 젝칼리온 상태 업데이트.
	Update_Pattern_FollowingGapWithPlayer(fTimeDelta);

	// 상태에 따른 움직임. (스토킹)
	Move_InState(fTimeDelta);

	// 포효 끝나고 전투 모드 들어가면 일정 시간 센 뒤 플레이어 향해 달려감.
	if (m_ePattern == PATTERN_IDLE && m_bIsSettingCompleted_Run_AfterStandUpFaceDown == false)
		m_fTimeDelta_After_STANDUPFACEDOWN += fTimeDelta;
	_float fTimeToRun_AfterSTANDUPFACEDOWN = 2.f;
	if (m_fTimeDelta_After_STANDUPFACEDOWN >= fTimeToRun_AfterSTANDUPFACEDOWN) // 포효 후 일정 시간 끝나면 STALIKING ON.
	{
		m_fTimeDelta_After_STANDUPFACEDOWN = 0.f;
		m_eCurrentState = BOSS_RUN;
		m_ePattern = PATTERN_STALKING;
		m_bIsSettingCompleted_Run_AfterStandUpFaceDown = true;
	}

	// 구 콜라이더 pos 업데이트.
	//m_pCollisionCom_Front->Update_SpherePos(*m_pTransCom_Front->Get_Infomation(CTransform::INFO_POSITION));

	// 공격 후 잠시 쉬는 시간일 때
	if (m_bIsBreakAfterAttack == true)
	{
		// 		m_ePattern = PATTERN_IDLE;
		// 		m_eCurrentState = BOSS_IDLE;

		m_fTimeDelta_ForBreak_AfterAttack += fTimeDelta;

		if (m_fTimeDelta_ForBreak_AfterAttack >= 1.f)
		{
			m_bIsBreakAfterAttack = false;
			m_fTimeDelta_ForBreak_AfterAttack = 0.f;
		}
	}

	// 공격하면서 대시.
	Move_InAnimation(fTimeDelta);


	// Transform 컴포넌트 월드 행렬 갱신.
	m_pTransCom->Invalidate_Worldmatrix();

	// 각 뼈들 월드 행렬 업데이트.
	Update_BoneMatrix();

	// Render Group에 계속 Add.
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	// 가드 시간 카운트
	if (m_bIsGaurding == true)
	{
		m_fTimeDelta_Gaurd += fTimeDelta;
		if (m_fTimeDelta_Gaurd >= 4.5f)
		{
			m_fTimeDelta_Gaurd = 0.f;
			m_eCurrentState = BOSS_GUARD_END;
			if (m_bIsCompletedToCreateUI_Gaurd3 == false)
			{
				Create_UI(3);
				m_bIsCompletedToCreateUI_Gaurd3 = true;
			}
			m_bIsGaurding = false;
		}
	}

	// 충돌 후 무적 시간 카운트
	if (m_bIsPermitted_Collision_WithBlade == false) // With Axe
	{
		m_fTimeDelta_Invincible_WithBlade += fTimeDelta;

		if (m_fTimeDelta_Invincible_WithBlade >= 2.f)
		{
			m_bIsPermitted_Collision_WithBlade = true;
			m_fTimeDelta_Invincible_WithBlade = 0.f;
		}
	}

	// 이펙트.
	if (m_eCurrentState == BOSS_ATTACK_JUMP_THROW) // ShockWave
	{
		_float fTrackPosition = m_pMeshCom->Get_TrackPosition();

		if (fTrackPosition >= 3.3f && m_bIsCompletedToCreateEffect_ShockWave == false)
		{
			Create_Effect_ShockWave();
			m_bIsCompletedToCreateEffect_ShockWave = true;
		}
	}
	else
	{
		m_bIsCompletedToCreateEffect_ShockWave = false;
	}

	if (m_eCurrentState == BOSS_GUARD_BEGIN) // AmingPoint
	{
		// 		_float fTrackPosition = m_pMeshCom->Get_TrackPosition();
		// 
		// 		if (fTrackPosition >= 1.f && m_bIsCompletedToCreateEffect_AmingPoint == false)
		// 		{
		if (m_bIsCompletedToCreateUI_Gaurd2 == false)
		{
			Create_UI(2);
			m_bIsCompletedToCreateUI_Gaurd2 = true;
		}
		m_bIsCompletedToCreateEffect_AmingPoint = true;
		/*		}*/
	}
	else
	{
		m_bIsCompletedToCreateEffect_AmingPoint = false;
	}

	return _int();
}

_int CZecallion::LastUpdate_GameObject(const _float & fTimeDelta)
{

	Update_FollowingCollision();

	const _matrix* matTransSParent = m_pTransCom_Front->Get_ParentMatrix_ForCollisionBox();
	m_pCollisionCom_Front->Update_SpherePos(*((_vec3*)&matTransSParent->m[3][0]));

	return _int();
}

void CZecallion::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	SetUp_ConstantTable(pEffect);


	// 월드행렬을 장치에 셋팅한다.
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	m_pMeshCom->Render_Mesh();
	pEffect->EndPass();
	pEffect->End();

	m_pTransCom_Front->SetUp_OnGraphicDev();
	m_pTransCom_Left_Hand->SetUp_OnGraphicDev();
	m_pTransCom_Right_Hand->SetUp_OnGraphicDev();
	m_pTransCom_Right_Calf->SetUp_OnGraphicDev();
	m_pTransCom_Left_Calf->SetUp_OnGraphicDev();

	if (g_bShowingCollider)
	{
		//m_pCollisionCom->Render_Collider();
		m_pCollisionCom_ForCombat->Render_Collider();
		// 

		m_pCollisionCom_Front->Render_Collider();
		// 
		// 		m_pTransCom_Head->SetUp_OnGraphicDev();
		// 		m_pCollisionCom_Head->Render_Collider();
		// 
		//m_pTransCom_Left->SetUp_OnGraphicDev();
		//m_pCollisionCom_Left->Render_Collider();

		//m_pTransCom_Right->SetUp_OnGraphicDev();
		//m_pCollisionCom_Right->Render_Collider();


		m_pCollisionCom_Left_Hand->Render_Collider();


		m_pCollisionCom_Right_Hand->Render_Collider();


		m_pCollisionCom_Right_Calf->Render_Collider();


		m_pCollisionCom_Left_Calf->Render_Collider();

		//m_pTransCom_Foot->SetUp_OnGraphicDev();
		//m_pCollisionCom_Foot->Render_Collider();
	}

}


void CZecallion::Check_Period_ForAnimation()
{
	// 	system("cls");
	// 	cout << m_iAttackCount << endl;
	//  	system("cls");
	//  	cout << "m_MagicNum : " << m_MagicNum << endl;
	//  	cout << "m_pMeshCom->Get_Period() : " << m_pMeshCom->Get_Period() << endl;
	//  	cout << "m_pMeshCom->Get_TrackPosition()  : " << m_pMeshCom->Get_TrackPosition() << endl;
	//  	cout << "m_eCurrentState : " << m_eCurrentState << endl;

	// 특정 동작;이 끝나도 Period - magicNum에 트랙 포지션이 도달해야 IDLE 상태로 바꿔 줄 것임.
	if (m_pMeshCom->Get_Period() - m_MagicNum <= m_pMeshCom->Get_TrackPosition()
		/*&& m_eCurrentState != BOSS_RUN*/ && m_eCurrentState != BOSS_IDLE) // Run은 애니메이션 끝까지 돌릴 필요 없다. IDLE도.
	{
		// 포효 동작 끝났을 때 trackPosition 끝났으면! SPAWN
		if (m_eCurrentState == BOSS_STANDUP_FACEDOWN)
		{
			m_eCurrentState = BOSS_SPAWN;
		}
		else if (m_eCurrentState == BOSS_DYING_BACK)
		{
			// m_eCurrentState = BOSS_IDLE;
			// 죽었으면 사라지셈.
			isDead = true;
		}
		else if (m_eCurrentState == BOSS_SPAWN) 		// SPAWN 끝났을 때 trackPosition 끝났으면! IDLE
		{
			Create_UI(0);
			m_ePattern = PATTERN_IDLE;
			m_eCurrentState = BOSS_IDLE;
		}
		else if (m_eCurrentState == BOSS_RUN)
		{
			m_eCurrentState = BOSS_RUN;
			m_pMeshCom->SetTrackPos(0);
		}
		else if (m_eCurrentState == BOSS_GUARD_BEGIN)
		{
			m_eCurrentState = BOSS_GUARD_DURING;
		}
		else if (m_eCurrentState == BOSS_GUARD_DURING)
		{
			m_bIsGaurding = true;
		}
		else if (m_eCurrentState == BOSS_GUARD_END)
		{
			m_ePattern = PATTERN_ATTACK;
			m_eCurrentState = BOSS_ATTACK_JUMP_THROW;
			m_iAttackCount = 0;
		}
		else if (m_eCurrentState == BOSS_ATTACK_JUMP_THROW)
		{
			m_ePattern = PATTERN_ATTACK;
			m_eCurrentState = BOSS_ATTACK_JUMP_THROW_END;
			m_bIsCompletedToCreateUI_Gaurd2 = false;
			m_bIsCompletedToCreateUI_Gaurd3 = false;
		}
		else if (m_eCurrentState == BOSS_ATTACK_DOUBLEMASH)
		{
			m_eCurrentState = BOSS_IDLE;
			m_ePattern = PATTERN_IDLE;
			m_bIsBreakAfterAttack = true;
		}
		else if (m_eCurrentState == BOSS_ATTACK_COMBO0)
		{
			m_eCurrentState = BOSS_IDLE;
			m_ePattern = PATTERN_IDLE;
			//m_bIsSettingAngleCompleted_BeforeAttacking = false;
		}
		else if (m_eCurrentState == BOSS_ATTACK_COMBO1)
		{
			m_eCurrentState = BOSS_IDLE;
			m_ePattern = PATTERN_IDLE;
			//m_bIsSettingAngleCompleted_BeforeAttacking = false;
		}
		else
		{
			m_eCurrentState = BOSS_IDLE;
			m_ePattern = PATTERN_IDLE;
		}

		if (m_iAttackCount >= 5)
		{
			m_eCurrentState = BOSS_GUARD_BEGIN;
			m_ePattern = PATTERN_GAURD;
			m_iAttackCount = 0;
		}

		m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
	}
}

void CZecallion::Set_MagicNum_InState()
{
	// 	switch (m_eCurrentState)
	// 	{
	// 	case BOSS_STANDUP_FACEDOWN:
	// 		m_MagicNum = 0.35;
	// 		m_fAdvanceTime = 0.8f;
	// 		break;
	// 	case BOSS_ATTACK_COMBO0:
	// 		m_fAdvanceTime = 2.f;
	// 		break;
	// 	case BOSS_ATTACK_COMBO1:
	// 		m_MagicNum = 0.3;
	// 		m_fAdvanceTime = 2.f;
	// 		break;
	// 	case BOSS_RUN:
	// 		m_fAdvanceTime = 1.2f;
	// 		break;
	// 	default:
	// 		m_MagicNum = 0.25;
	// 		m_fAdvanceTime = 1.4f;
	// 		break;
	// 	}
}

void CZecallion::Set_BoneMatrix_WorldMatrix()
{
	m_pBoneMatrix_Head_Parent = m_pMeshCom->Get_BoneMatrix("EYE_glow_R");

	m_pBoneMatrix_Front_Parent = m_pMeshCom->Get_BoneMatrix("jiggle_zecallion_front");

	m_pBoneMatrix_Left_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_L_Shoulder01");

	m_pBoneMatrix_Right_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_R_Shoulder01");

	m_pBoneMatrix_Left_Hand_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_L_Hand");

	m_pBoneMatrix_Right_Hand_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_R_Hand");

	m_pBoneMatrix_Left_Calf_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_L_Calf");

	m_pBoneMatrix_Right_Calf_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_R_Calf");

	//m_pBoneMatrix_Foot_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Anim_Attachment_CAM_Origin");
}

void CZecallion::Set_AnimationSet_InChangingState()
{

	if (m_ePreviousState != m_eCurrentState)
	{

		m_ePreviousState = m_eCurrentState;

		switch (m_eCurrentState)
		{
		case BOSS_STANDUP_FACEDOWN:
			m_MagicNum = 0.35;
			m_fAdvanceTime = 0.8f;
			break;
		case BOSS_SPAWN:
			m_MagicNum = 0.3;
			m_fAdvanceTime = 1.3f;
			break;
		case BOSS_ATTACK_COMBO0:
			m_MagicNum = 0.3;
			m_fAdvanceTime = 1.4f;
			break;
		case BOSS_ATTACK_COMBO1:
			m_MagicNum = 0.45;
			m_fAdvanceTime = 2.f;
			break;
		case BOSS_RUN:
			m_fAdvanceTime = 1.2f;
			break;
		case BOSS_GUARD_BEGIN:
			m_MagicNum = 0.3;
			break;
		case BOSS_DAMAGE_LEFT:
			m_MagicNum = 0.3;
			m_fAdvanceTime = 0.8f;
			break;
		default:
			m_MagicNum = 0.25;
			m_fAdvanceTime = 1.4f;
			break;
		}

		m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
		m_pMeshCom->SetTrackPos(0);
	}
}

void CZecallion::Update_BoneMatrix()
{
	m_pTransCom_Left->Set_Parent((*m_pBoneMatrix_Left_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Right->Set_Parent((*m_pBoneMatrix_Right_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Left_Hand->Set_Parent((*m_pBoneMatrix_Left_Hand_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Right_Hand->Set_Parent((*m_pBoneMatrix_Right_Hand_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Left_Calf->Set_Parent((*m_pBoneMatrix_Left_Calf_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Right_Calf->Set_Parent((*m_pBoneMatrix_Right_Calf_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Head->Set_Parent((*m_pBoneMatrix_Head_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Front->Set_Parent((*m_pBoneMatrix_Front_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	//m_pTransCom_Foot->Set_Parent(/*(*m_pBoneMatrix_Foot_Parent) **/ (*m_pTransCom->Get_WorldMatrix()));
}

void CZecallion::Update_FollowingCollision()
{

	// 보스 LeftHand - 플레이어 Blade
	CCollision* pPlayer_Blade = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Collision", 1);
	CMesh_Dynamic* pPlayer_Mesh = (CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh", 0);
	_float fPlayer_GetTrackPosition = pPlayer_Mesh->Get_TrackPosition();
	_int iPlayerIdx = pPlayer_Mesh->Get_AnimationIdx();

	//system("cls");
	//cout << "iPlayerIdx : " << iPlayerIdx << endl;
	//cout << "fPlayer_GetTrackPosition : " << fPlayer_GetTrackPosition << endl;


	if (m_pCollisionCom_Left_Calf->Collision_OBB(pPlayer_Blade) && m_bIsPermitted_Collision_WithBlade == true)
	{
		if (iPlayerIdx == 14 || iPlayerIdx == 17)
		{
			if (fPlayer_GetTrackPosition >= 0.4 &&  fPlayer_GetTrackPosition <= 1.4)
				m_eCurrentState = BOSS_DAMAGE_LEFT;
			m_bIsPermitted_Collision_WithBlade = false;

			// 블러드 이펙트
			Create_Effect_Blood(CEffect_Blood::BONE_LEFT_CALF);
			// 히트 이펙트
			Create_Effect_Hit(CEffect_Hit::BONE_LEFT_CALF, 0);
			CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
			OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
			_float fAtt = tInfo.fAtt;
			m_pInformationCom->HPMinus(fAtt);
		}
		else if (iPlayerIdx == 15 || iPlayerIdx == 18)
		{
			if (fPlayer_GetTrackPosition >= 0.25 &&  fPlayer_GetTrackPosition <= 0.7)
				m_eCurrentState = BOSS_DAMAGE_LEFT;
			m_bIsPermitted_Collision_WithBlade = false;

			// 블러드 이펙트
			Create_Effect_Blood(CEffect_Blood::BONE_LEFT_CALF);
			// 히트 이펙트
			Create_Effect_Hit(CEffect_Hit::BONE_LEFT_CALF, 1);
			CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
			OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
			_float fAtt = tInfo.fAtt;
			m_pInformationCom->HPMinus(fAtt);
		}
		else if (iPlayerIdx == 16 || iPlayerIdx == 19)
		{
			if (fPlayer_GetTrackPosition >= 0.4 &&  fPlayer_GetTrackPosition <= 1.1)
				m_eCurrentState = BOSS_DAMAGE_LEFT;
			m_bIsPermitted_Collision_WithBlade = false;

			// 블러드 이펙트
			Create_Effect_Blood(CEffect_Blood::BONE_LEFT_CALF);
			// 히트 이펙트
			Create_Effect_Hit(CEffect_Hit::BONE_LEFT_CALF, 0);
			CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
			OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
			_float fAtt = tInfo.fAtt;
			m_pInformationCom->HPMinus(fAtt);
		}

	}

	if (m_pCollisionCom_Right_Calf->Collision_OBB(pPlayer_Blade) && m_bIsPermitted_Collision_WithBlade == true)
	{
		if (iPlayerIdx == 14 || iPlayerIdx == 17)
		{
			if (fPlayer_GetTrackPosition >= 0.4 &&  fPlayer_GetTrackPosition <= 1.4)
				m_eCurrentState = BOSS_DAMAGE_RIGHT;
			m_bIsPermitted_Collision_WithBlade = false;

			// 블러드 이펙트
			Create_Effect_Blood(CEffect_Blood::BONE_RIGHT_CALF);
			// 히트 이펙트
			Create_Effect_Hit(CEffect_Hit::BONE_LEFT_CALF, 0);

			CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
			OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
			_float fAtt = tInfo.fAtt;
			m_pInformationCom->HPMinus(fAtt);
		}
		else if (iPlayerIdx == 15 || iPlayerIdx == 18)
		{
			if (fPlayer_GetTrackPosition >= 0.25 &&  fPlayer_GetTrackPosition <= 0.7)
				m_eCurrentState = BOSS_DAMAGE_RIGHT;
			m_bIsPermitted_Collision_WithBlade = false;

			// 블러드 이펙트
			Create_Effect_Blood(CEffect_Blood::BONE_RIGHT_CALF);
			// 히트 이펙트
			Create_Effect_Hit(CEffect_Hit::BONE_LEFT_CALF, 1);

			CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
			OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
			_float fAtt = tInfo.fAtt;
			m_pInformationCom->HPMinus(fAtt);
		}
		else if (iPlayerIdx == 16 || iPlayerIdx == 19)
		{
			if (fPlayer_GetTrackPosition >= 0.4 &&  fPlayer_GetTrackPosition <= 0.6)
				m_eCurrentState = BOSS_DAMAGE_RIGHT;
			m_bIsPermitted_Collision_WithBlade = false;

			// 블러드 이펙트
			Create_Effect_Blood(CEffect_Blood::BONE_RIGHT_CALF);
			// 히트 이펙트
			Create_Effect_Hit(CEffect_Hit::BONE_LEFT_CALF, 0);

			CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
			OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
			_float fAtt = tInfo.fAtt;
			m_pInformationCom->HPMinus(fAtt);
		}

	}

}

void CZecallion::Update_Pattern_FollowingCollision(const _float& fTimeDelta)
{
	// 	system("cls");
	// 	cout << "m_ePattern : " << m_ePattern << endl;
	// 	cout << "m_bIsBreakAfterAttack : " << m_bIsBreakAfterAttack << endl;
	// 충돌 허락 안 됐을 땐 패턴 업데이트 X.
	if (m_bIsPermitted_Collision == false)
		return;

	// 플레이어 front 충돌 박스.
	CCollision* pPlayerCollision_front = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Collision_Front", 0);


	// 전투 모드 충돌 박스 - 플레이어 충돌 박스 OBB.
	// 젝칼리온 전투 모드 충돌 박스로 들어가면 한번 "포효"(STANDUP_FACEDOWN)한 뒤 m_ePattern을 NONE에서 IDLE로.
	if (m_bIsStandUpFaceDown_Completed_OnInit == false)
	{
		if (m_pCollisionCom_ForCombat->Collision_OBB(pPlayerCollision_front))
		{
			Create_UI(1);
			m_eCurrentState = BOSS_STANDUP_FACEDOWN;
			m_bIsStandUpFaceDown_Completed_OnInit = true; // 포효는 최초 한 번 해야 하므로..
		}
	}

	if (m_pCollisionCom_Foot->Collision_OBB(pPlayerCollision_front) &&
		(m_ePattern == PATTERN_STALKING || m_ePattern == PATTERN_ATTACK))
	{
		m_bIsCollided_WithPlayerWhole = true;
	}
	else
	{
		m_bIsCollided_WithPlayerWhole = false;
	}
}

void CZecallion::Update_Pattern_FollowingGapWithPlayer(const _float & fTimeDelta)
{
	//    	system("cls");
	//    	cout << "m_ePattern: " << m_ePattern << endl;
	//   	cout << "m_eCurrentState: " << m_eCurrentState << endl;
	// 	cout << "m_ePreviousState: " << m_ePreviousState << endl;
	// 	cout << "m_bIsDashInAttack0_0: " << m_bIsDashInAttack0_0 << endl;
	// 	cout << "m_bIsDashInAttack0_1: " << m_bIsDashInAttack0_1 << endl;
	// 	cout << "m_bIsDashInAttack1_0: " << m_bIsDashInAttack1_0 << endl;
	// 패턴 나눠주기.
	if (m_ePattern == PATTERN_NONE || m_ePattern == PATTERN_GAURD)
		return;

	if (m_bIsBreakAfterAttack)
		return;

	CTransform* pPlayer_Transform = (CTransform*)m_pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);
	_vec3 vPlayer_Pos = *pPlayer_Transform->Get_Infomation(CTransform::INFO_POSITION);
	_vec3 vZecallion_Pos = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
	_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

	//cout << D3DXVec3Length(&vZecToPlayer_Dir) << endl;

	// 	// IDLE 일 때, 거리가 10 넘어서면 STALKING On
	if (m_ePattern == PATTERN_IDLE && m_bIsSettingCompleted_Run_AfterStandUpFaceDown == true)
	{
		if (D3DXVec3Length(&vZecToPlayer_Dir) > 18.f)
		{
			m_ePattern = PATTERN_STALKING;
			m_eCurrentState = BOSS_RUN;
		}
		else
		{
			m_ePattern = PATTERN_ATTACK;
			m_eCurrentState = BOSS_RUN;
		}
	}
	// STALKING 일 때, 거리가 20보다 적으면 Attack On
	if (m_ePattern == PATTERN_STALKING && D3DXVec3Length(&vZecToPlayer_Dir) <= 18.f)
	{
		m_ePattern = PATTERN_ATTACK;
	}
	// 	// IDLE 상태일 때, 거리가 2보다 적으면 ATTACK On
	// 	if (m_ePattern == PATTERN_IDLE && D3DXVec3Length(&vZecToPlayer_Dir) <= 13.f)
	// 	{
	// 		if (m_bIsBreakAfterAttack)
	// 			return;
	// 		m_ePattern = PATTERN_ATTACK;
	// 	}
	// ATTACK 상태일 때, 일정 시간 후 플레이어 가격.
	if (m_ePattern == PATTERN_ATTACK || m_ePattern == PATTERN_NONE)
	{
		if (m_bIsBreakAfterAttack)
		{
			return;
		}

		if (m_eCurrentState == BOSS_ATTACK_COMBO0 || m_eCurrentState == BOSS_ATTACK_COMBO1 || m_eCurrentState == BOSS_ATTACK_DOUBLEMASH
			|| m_eCurrentState == BOSS_ATTACK_JUMP_THROW || m_eCurrentState == BOSS_ATTACK_JUMP_THROW_END)
			return;

		_float fDistancePlayerToBoss = D3DXVec3Length(&vZecToPlayer_Dir);

		//   		system("cls");
		//   		cout << "fDistancePlayerToBoss: " << fDistancePlayerToBoss << endl;
		// 		cout << "m_ePattern: " << m_ePattern << endl;
		// 		cout << "m_eCurrentState: " << m_eCurrentState << endl;

		// 거리가 약 15~20 사이면 DoubleMash, 10~15면 AttackCombo1, ~10이면 AttackCombo0

		if (fDistancePlayerToBoss > 18.f)
		{
			m_ePattern = PATTERN_STALKING;
			m_eCurrentState = BOSS_RUN;
		}
		if (fDistancePlayerToBoss <= 18.f && fDistancePlayerToBoss > 15.f)
		{
			m_eCurrentState = BOSS_ATTACK_DOUBLEMASH;
			m_iAttackCount++;
			m_pMeshCom->SetTrackPos(0);
		}
		if (fDistancePlayerToBoss <= 15.f && fDistancePlayerToBoss > 10.f)
		{
			m_eCurrentState = BOSS_ATTACK_COMBO1;
			m_iAttackCount++;
			m_pMeshCom->SetTrackPos(0);
		}
		if (fDistancePlayerToBoss <= 10.f)
		{
			m_eCurrentState = BOSS_ATTACK_COMBO0;
			m_iAttackCount++;
			m_pMeshCom->SetTrackPos(0);
		}

	}
}

// 플레이어 스토킹.
void CZecallion::Move_InState(const _float & fTimeDelta)
{
	if (m_bIsBreakAfterAttack)
		return;

	if (m_ePattern == PATTERN_ATTACK)
		return;

	if (m_bIsDashInAttack0_0 || m_bIsDashInAttack0_1 || m_bIsDashInAttack0_2 || m_bIsDashInAttack1_0)
		return;

	if (m_ePattern == PATTERN_STALKING)
	{
		CTransform* pPlayer_Transform = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);
		_vec3 vPlayer_Pos = *pPlayer_Transform->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecallion_Pos = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;


		// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
		m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);

		if (m_bIsSettingAngleCompleted_InRunningToPlayer == false)
		{
			// 각도 구하기.
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			_float fRadian = atan2f(vZecToPlayer_Dir.z, vZecToPlayer_Dir.x);
			_float fAngle = 180.f - D3DXToDegree(fRadian);
			m_fAngle = fAngle;
		}

		// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
		vZecToPlayer_Dir.x *= vecScaleInfo.x;
		vZecToPlayer_Dir.y *= vecScaleInfo.y;
		vZecToPlayer_Dir.z *= vecScaleInfo.z;
		m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
		if (m_bIsCollided_WithPlayerWhole == false)
			m_pTransCom->Go_Staight(fTimeDelta * m_fSpeed);
		m_pTransCom->RotationY(D3DXToRadian(m_fAngle));
	}

	else if (m_ePattern == PATTERN_ATTACK)
	{
		CTransform* pPlayer_Transform = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);
		_vec3 vPlayer_Pos = *pPlayer_Transform->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecallion_Pos = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

		if (m_bIsSettingAngleCompleted_BeforeAttacking == false)
		{
			// 각도 구하기.
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			_float fRadian = atan2f(vZecToPlayer_Dir.z, vZecToPlayer_Dir.x);
			_float fAngle = 180.f - D3DXToDegree(fRadian);
			m_fAngle = fAngle;
			//m_bIsSettingAngleCompleted_BeforeAttacking = true;
		}

		// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
		vZecToPlayer_Dir.x *= vecScaleInfo.x;
		vZecToPlayer_Dir.y *= vecScaleInfo.y;
		vZecToPlayer_Dir.z *= vecScaleInfo.z;
		m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
		m_pTransCom->RotationY(D3DXToRadian(m_fAngle));
	}
}

void CZecallion::Move_InAnimation(const _float & fTimeDelta)
{
	if (m_eCurrentState == BOSS_SPAWN)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 4.6 && m_pMeshCom->Get_TrackPosition() <= 4.7)
		{
			m_bIsDashInSpawn = true;
		}
	}

	if (m_bIsDashInSpawn)
	{
		// 플레이어 향한 대시 해야 하니까 Look 벡터 구해주기.
		CTransform* pPlayer_Transform = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);
		_vec3 vPlayer_Pos = *pPlayer_Transform->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecallion_Pos = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);

		_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

		// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
		m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


		// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
		vZecToPlayer_Dir.x *= vecScaleInfo.x;
		vZecToPlayer_Dir.y *= vecScaleInfo.y;
		vZecToPlayer_Dir.z *= vecScaleInfo.z;


		m_fTimeDeltaDash += fTimeDelta;


		//m_fSpeed = m_fOriSpeed *  3.f;
		m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
		if (m_bIsCollided_WithPlayerWhole == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
		m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

		if (m_fTimeDeltaDash >= 1.3f)
		{
			m_bIsDashInSpawn = false;
			m_fTimeDeltaDash = 0.f;
		}
	}

	if (m_ePreviousState == m_eCurrentState)
		return;

	//  	system("cls");
	//  	cout << "TrackPosition : " << m_pMeshCom->Get_TrackPosition() << endl;


	if (m_ePattern == PATTERN_ATTACK)
	{
		if (m_eCurrentState == BOSS_ATTACK_COMBO0)
		{
			if (m_pMeshCom->Get_TrackPosition() >= 0.15 && m_pMeshCom->Get_TrackPosition() <= 0.2)
			{
				m_bIsSettingAngleCompleted_BeforeAttacking = true; // 대시 시작하면 각도 그만!
				m_bIsDashInAttack0_0 = true;
			}
			if (m_pMeshCom->Get_TrackPosition() >= 4.35 && m_pMeshCom->Get_TrackPosition() <= 4.40)
			{
				m_bIsDashInAttack0_1 = true;
			}
		}

		if (m_eCurrentState == BOSS_ATTACK_COMBO1)
		{

			if (m_pMeshCom->Get_TrackPosition() >= 0.96 && m_pMeshCom->Get_TrackPosition() <= 1.01)
			{
				m_bIsDashInAttack1_0 = true;
			}
		}

		if (m_eCurrentState == BOSS_ATTACK_DOUBLEMASH)
		{
			if (m_pMeshCom->Get_TrackPosition() >= 1.1 && m_pMeshCom->Get_TrackPosition() <= 1.15)
			{
				m_bIsDashInDoubleMash_0 = true;
			}
			if (m_pMeshCom->Get_TrackPosition() >= 2.1 && m_pMeshCom->Get_TrackPosition() <= 2.15)
			{
				m_bIsDashInDoubleMash_1 = true;
			}
			if (m_pMeshCom->Get_TrackPosition() >= 5.2 && m_pMeshCom->Get_TrackPosition() <= 5.3)
			{
				m_bIsDashInDoubleMash_2 = true;
			}
		}
		if (m_eCurrentState == BOSS_ATTACK_JUMP_THROW)
		{
			if (m_pMeshCom->Get_TrackPosition() >= 3.1 && m_pMeshCom->Get_TrackPosition() <= 3.2)
			{
				m_bIsJumpInAttackJumpThrow_0 = true;
			}
			if (m_pMeshCom->Get_TrackPosition() >= 4.1 && m_pMeshCom->Get_TrackPosition() <= 4.2)
			{
				m_bIsJumpInAttackJumpThrow_1 = true;
			}
		}


		////////////////////////////////////////////////////

		// 플레이어 향한 대시 해야 하니까 Look 벡터 구해주기.
		CTransform* pPlayer_Transform = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);
		_vec3 vPlayer_Pos = *pPlayer_Transform->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecallion_Pos = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;


		// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
		m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


		// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
		_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
		vZecToPlayer_Dir.x *= vecScaleInfo.x;
		vZecToPlayer_Dir.y *= vecScaleInfo.y;
		vZecToPlayer_Dir.z *= vecScaleInfo.z;


		//////////////////////////////////////////////////////
		if (m_bIsDashInAttack0_0)
		{
			m_fTimeDeltaDash += fTimeDelta;

			m_fSpeed = m_fOriSpeed * 1.5f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.2f)
			{
				m_bIsDashInAttack0_0 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}
		if (m_bIsDashInAttack0_1)
		{
			_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			vZecToPlayer_Dir.x *= vecScaleInfo.x;
			vZecToPlayer_Dir.y *= vecScaleInfo.y;
			vZecToPlayer_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  3.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.16f)
			{
				m_bIsDashInAttack0_1 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}

		if (m_bIsDashInAttack1_0)
		{
			_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			vZecToPlayer_Dir.x *= vecScaleInfo.x;
			vZecToPlayer_Dir.y *= vecScaleInfo.y;
			vZecToPlayer_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  3.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.1f)
			{
				m_bIsDashInAttack1_0 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}
		if (m_bIsDashInDoubleMash_0)
		{
			_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			vZecToPlayer_Dir.x *= vecScaleInfo.x;
			vZecToPlayer_Dir.y *= vecScaleInfo.y;
			vZecToPlayer_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  2.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.1f)
			{
				m_bIsDashInDoubleMash_0 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}
		if (m_bIsDashInDoubleMash_1)
		{
			_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			vZecToPlayer_Dir.x *= vecScaleInfo.x;
			vZecToPlayer_Dir.y *= vecScaleInfo.y;
			vZecToPlayer_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  2.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.1f)
			{
				m_bIsDashInDoubleMash_1 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}
		if (m_bIsDashInDoubleMash_2)
		{
			_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			vZecToPlayer_Dir.x *= vecScaleInfo.x;
			vZecToPlayer_Dir.y *= vecScaleInfo.y;
			vZecToPlayer_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  4.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.15f)
			{
				m_bIsDashInDoubleMash_2 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}
		if (m_bIsJumpInAttackJumpThrow_0)
		{
			_vec3 vSkyPos = _vec3(vZecallion_Pos.x, vZecallion_Pos.y + 200, vZecallion_Pos.z);
			//

			_vec3 vZecToSky_Dir = vSkyPos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToSky_Dir, &vZecToSky_Dir);
			vZecToSky_Dir.x *= vecScaleInfo.x;
			vZecToSky_Dir.y *= vecScaleInfo.y;
			vZecToSky_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  3.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToSky_Dir);
			if (m_bIsCollided_WithPlayerWhole == false)
				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.2f)
			{
				m_bIsJumpInAttackJumpThrow_0 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}
		if (m_bIsJumpInAttackJumpThrow_1)
		{
			_vec3 vZecToPlayer_Dir = vPlayer_Pos - vZecallion_Pos;

			// 각도 연산해야 해서. 기존 Look 벡터 먼저 넣어주고
			m_vecOriLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);


			// 방향 벡터 원래 길이가 있었을 거 아님? 받아오자.
			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
			D3DXVec3Normalize(&vZecToPlayer_Dir, &vZecToPlayer_Dir);
			vZecToPlayer_Dir.x *= vecScaleInfo.x;
			vZecToPlayer_Dir.y *= vecScaleInfo.y;
			vZecToPlayer_Dir.z *= vecScaleInfo.z;


			m_fTimeDeltaDash += fTimeDelta;


			m_fSpeed = m_fOriSpeed *  5.f;
			m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vZecToPlayer_Dir);
// 			if (m_bIsCollided_WithPlayerWhole == false)
 				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
			m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

			if (m_fTimeDeltaDash >= 0.3f)
			{
				m_bIsJumpInAttackJumpThrow_1 = false;
				m_fTimeDeltaDash = 0.f;
				m_fSpeed = m_fOriSpeed;
			}
		}

	}

}

void CZecallion::Create_Effect_Blood(CEffect_Blood::ZECALLION_BONE eBoneID)
{
	// GameObject Add
	CGameObject* pGameObject = nullptr;


	_vec3	vBloodPosition;
	const _matrix* matParent;
	const _matrix* matWorld = m_pTransCom->Get_WorldMatrix();

	switch (eBoneID)
	{
	case CEffect_Blood::BONE_LEFT_CALF:
	{
		matParent = m_pTransCom_Left_Calf->Get_ParentMatrix_ForCollisionBox();
		const _matrix matResult = (*m_pBoneMatrix_Left_Calf_Parent) * (*matWorld);
		vBloodPosition = *(_vec3*)(&matResult.m[3][0]);
		pGameObject = CEffect_Blood::Create(m_pGraphicDev, eBoneID, vBloodPosition, 1);
		break;
	}
	case CEffect_Blood::BONE_RIGHT_CALF:
	{
		matParent = m_pTransCom_Right_Calf->Get_ParentMatrix_ForCollisionBox();
		const _matrix matResult2 = (*m_pBoneMatrix_Right_Calf_Parent) * (*matWorld);
		vBloodPosition = *(_vec3*)(&matResult2.m[3][0]);
		pGameObject = CEffect_Blood::Create(m_pGraphicDev, eBoneID, vBloodPosition, 1);
		break;
	}
	}



	if (nullptr == pGameObject)
		return;

	CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject);
}

void CZecallion::Create_Effect_Hit(CEffect_Hit::ZECALLION_BONE eBoneID, _int iEffectIndex)
{
	// GameObject Add
	CGameObject* pGameObject = nullptr;


	_vec3	vPosition;
	const _matrix* matParent;
	const _matrix* matWorld = m_pTransCom->Get_WorldMatrix();

	switch (eBoneID)
	{
	case CEffect_Hit::BONE_LEFT_CALF:
	{
		matParent = m_pTransCom_Left_Calf->Get_ParentMatrix_ForCollisionBox();
		const _matrix matResult = (*m_pBoneMatrix_Left_Calf_Parent) * (*matWorld);
		vPosition = *(_vec3*)(&matResult.m[3][0]);
		// 1
		pGameObject = CEffect_Hit::Create(m_pGraphicDev, eBoneID, vPosition, iEffectIndex, _vec3(0.f, 0.f, 30.f));
		if (nullptr == pGameObject)
			return;
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject);
		// 2
		pGameObject = CEffect_Hit::Create(m_pGraphicDev, eBoneID, vPosition, iEffectIndex, _vec3(0.f, 0.f, 60.f));
		if (nullptr == pGameObject)
			return;
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject);
		break;
	}
	case CEffect_Hit::BONE_RIGHT_CALF:
	{
		matParent = m_pTransCom_Right_Calf->Get_ParentMatrix_ForCollisionBox();
		const _matrix matResult2 = (*m_pBoneMatrix_Right_Calf_Parent) * (*matWorld);
		vPosition = *(_vec3*)(&matResult2.m[3][0]);
		// 1
		pGameObject = CEffect_Hit::Create(m_pGraphicDev, eBoneID, vPosition, iEffectIndex, _vec3(0.f, 0.f, 30.f));
		if (nullptr == pGameObject)
			return;
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject);
		// 2
		pGameObject = CEffect_Hit::Create(m_pGraphicDev, eBoneID, vPosition, iEffectIndex, _vec3(0.f, 0.f, 0.f));
		if (nullptr == pGameObject)
			return;
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject);
		break;
	}
	}




}

void CZecallion::Create_Effect_ShockWave()
{
	_vec3	vPosition;
	const _matrix* matParent;
	const _matrix* matWorld = m_pTransCom->Get_WorldMatrix();

	matParent = m_pMeshCom->Get_BoneMatrix("SMDImport");
	const _matrix matResult = (*matParent) * (*matWorld);
	vPosition = *(_vec3*)(&matResult.m[3][0]);


	CGameObject* pGameObject = nullptr;
	pGameObject = CEffect_ShockWave::Create(m_pGraphicDev, vPosition);
	if (nullptr == pGameObject)
		return;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Effect_WaveShock", pGameObject);
}

void CZecallion::Create_UI(_int iUIIndex)
{
	CGameObject* pGameObject = nullptr;

	if (iUIIndex == 0 && m_bIsCompletedToCreateUI_ZecallionHPBar == false)
	{
		pGameObject = CZecallion_HPBar::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return;
		m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Zecallion_UI", pGameObject);

		pGameObject = CZecallion_HPBar2::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return;
		m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Zecallion_UI", pGameObject);

		pGameObject = CZecallion_HPBar_Font::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return;
		m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Zecallion_UI", pGameObject);

		m_bIsCompletedToCreateUI_ZecallionHPBar = true;
	}
	else if (iUIIndex == 1)
	{
		pGameObject = CNameCard::Create(m_pGraphicDev);
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_NameCard", pGameObject);
	}
	else if (iUIIndex == 2)
	{
		pGameObject = CGaurd_UI::Create(m_pGraphicDev, 0);
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_GaurdUI", pGameObject);
	}
 	else if (iUIIndex == 3)
 	{
		pGameObject = CGaurd_UI2::Create(m_pGraphicDev, 1);
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_GaurdUI2", pGameObject);
}
	else if (iUIIndex == 4)
	{
		pGameObject = CClear_UI::Create(m_pGraphicDev, 0);
		CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_ClearUI", pGameObject); 
	}


	if (nullptr == pGameObject)
		return;


}
 
 void CZecallion::Update_Sound_FollowingState()
 {
 	_float fTrackPosition = m_pMeshCom->Get_TrackPosition();
 
 	if (m_eCurrentState == BOSS_SPAWN)
 	{
// 		system("cls");
// 		cout << fTrackPosition << endl;
		if ((fTrackPosition >= 5.5f && fTrackPosition <= 5.55f))
		{
			if (m_bIsCompletedToPlaySound_SpawnWalk0 == false)
			{
				m_pSoundMgr->PlaySound(L"meer_start_motion_03.wav", CHANNEL_MONSTER_WALK);
				m_bIsCompletedToPlaySound_SpawnWalk0 = true;
			}
		}
		if ((fTrackPosition >= 6.2f && fTrackPosition <= 6.25f))
		{
			if (m_bIsCompletedToPlaySound_SpawnWalk1 == false)
			{
				m_pSoundMgr->PlaySound(L"meer_start_motion_03.wav", CHANNEL_MONSTER_WALK);
				m_bIsCompletedToPlaySound_SpawnWalk1 = true;
			}
		}


		if ((fTrackPosition >= 15.f && fTrackPosition <= 15.5f))
		{
			if (m_bIsCompletedToPlaySound_PropBegin == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_prop_begin.wav", CHANNEL_MONSTER);
				m_bIsCompletedToPlaySound_PropBegin = true;
			}
		}
		if ((fTrackPosition >= 10.5f && fTrackPosition <= 11.f))
		{
			if (m_bIsCompletedToPlaySound_Spawn_Shout == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_01.wav", CHANNEL_MONSTER_SHOUT);
				m_bIsCompletedToPlaySound_Spawn_Shout = true;
			}
		}
		if ((fTrackPosition >= 0.05f && fTrackPosition <= 0.1f))
		{
			if (m_bIsCompletedToPlaySound_Chain == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_cs_chain.wav", CHANNEL_MONSTER);
				m_bIsCompletedToPlaySound_Chain = true;
			}
		}
		if ((fTrackPosition >= 0.5f && fTrackPosition <= 0.55f))
		{
			if (m_bIsCompletedToPlaySound_DoorStart == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_cs_door_start.wav", CHANNEL_MONSTER);
				m_bIsCompletedToPlaySound_DoorStart = true;
			}
		}
		if ((fTrackPosition >= 2.5f && fTrackPosition <= 2.55f))
		{
			m_bIsCompletedToPlaySound_Chain = false;
			m_bIsCompletedToPlaySound_DoorStart = false;
		}

		if ((fTrackPosition >= 2.8f && fTrackPosition <= 2.85f))
		{
			if (m_bIsCompletedToPlaySound_Chain == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_cs_chain.wav", CHANNEL_MONSTER);
				m_bIsCompletedToPlaySound_Chain = true;
			}
		}

		if ((fTrackPosition >= 2.9f && fTrackPosition <= 2.95f))
		{
			if (m_bIsCompletedToPlaySound_DoorStart == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_cs_door_start.wav", CHANNEL_MONSTER);
				m_bIsCompletedToPlaySound_DoorStart = true;
			}
		}

		if ((fTrackPosition >= 4.35f && fTrackPosition <= 4.4f))
		{
			if (m_bIsCompletedToPlaySound_DoorDown == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_cs_door_start.wav", CHANNEL_MONSTER);
				m_bIsCompletedToPlaySound_DoorDown = true;
			}
		}

 	}
	else if (m_eCurrentState == BOSS_RUN)
	{
		if ((fTrackPosition >= 0.9f && fTrackPosition <= 0.95f)
			|| (fTrackPosition >= 0.2f && fTrackPosition <= 0.25f))
		{
			if (m_bIsCompletedToPlaySound_Run == false)
			{
				m_pSoundMgr->PlaySound(L"meer_start_motion_03.wav", CHANNEL_MONSTER_WALK, 0.8f);
				m_bIsCompletedToPlaySound_Run = true;
			}
		}
		if ((fTrackPosition >= 0.5f && fTrackPosition <= 0.55f)
			|| (fTrackPosition >= 1.0f && fTrackPosition <= 1.5f))
		{
			m_bIsCompletedToPlaySound_Run = false;
		}
	}
	else if (m_eCurrentState == BOSS_ATTACK_COMBO0)
	{
		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_AttackCombo0_Shout = false;
			m_bIsCompletedToPlaySound_AttackCombo0 = false;

		}
		if ((fTrackPosition >= 1.25f && fTrackPosition <= 1.3f)) // Shout
		{
			if (m_bIsCompletedToPlaySound_AttackCombo0_Shout == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_01.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
				m_bIsCompletedToPlaySound_AttackCombo0_Shout = true;
			}
		}

		if ((fTrackPosition >= 3.75f && fTrackPosition <= 3.8f)) // Blade
		{
			if (m_bIsCompletedToPlaySound_AttackCombo0 == false)
			{
				m_pSoundMgr->PlaySound(L"common_swing_lv8.wav", CHANNEL_MONSTER_BLADE, 0.8f);
				m_bIsCompletedToPlaySound_AttackCombo0 = true;
			}
		}
	}
	else if (m_eCurrentState == BOSS_ATTACK_COMBO1)
	{
		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_AttackCombo1_Shout = false;
			m_bIsCompletedToPlaySound_AttackCombo1 = false;
		}
		if ((fTrackPosition >= 0.8f && fTrackPosition <= 0.85f)) // Shout
		{
			if (m_bIsCompletedToPlaySound_AttackCombo1_Shout == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_02.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
				m_bIsCompletedToPlaySound_AttackCombo1_Shout = true;
			}
		}

		if ((fTrackPosition >= 1.2f && fTrackPosition <= 1.25f)) // Blade
		{
			if (m_bIsCompletedToPlaySound_AttackCombo1 == false)
			{
				m_pSoundMgr->PlaySound(L"common_swing_lv9.wav", CHANNEL_MONSTER_BLADE, 0.8f);
				m_bIsCompletedToPlaySound_AttackCombo1 = true;
			}
		}
	}
	else if (m_eCurrentState == BOSS_ATTACK_DOUBLEMASH)
	{
		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_DoubleMash_Shout = false;
			m_bIsCompletedToPlaySound_DoubleMash0 = false;
		}

		if ((fTrackPosition >= 0.75f && fTrackPosition <= 0.8f)) // Shout
		{
			if (m_bIsCompletedToPlaySound_DoubleMash_Shout == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_03.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
				m_bIsCompletedToPlaySound_DoubleMash_Shout = true;
			}
		}

		if ((fTrackPosition >= 2.9f && fTrackPosition <= 2.95f)) // Blade
		{
			if (m_bIsCompletedToPlaySound_DoubleMash0 == false)
			{
				m_pSoundMgr->PlaySound(L"common_swing_lv10.wav", CHANNEL_MONSTER_BLADE, 0.8f);
				m_bIsCompletedToPlaySound_DoubleMash0 = true;
			}
		}
		if ((fTrackPosition >= 5.1f && fTrackPosition <= 5.2f)) // Blade
		{
			if (m_bIsCompletedToPlaySound_DoubleMash1 == false)
			{
				m_pSoundMgr->PlaySound(L"common_swing_lv9.wav", CHANNEL_MONSTER_BLADE2, 0.8f);
				m_bIsCompletedToPlaySound_DoubleMash1 = true;
			}
		}
	}
	else if (m_eCurrentState == BOSS_ATTACK_JUMP_THROW)
	{
		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_JumpThrow_Shout0 = false;
			m_bIsCompletedToPlaySound_JumpThrow = false;
		}

		if ((fTrackPosition >= 1.3f && fTrackPosition <= 1.35f)) // Shout
		{
			if (m_bIsCompletedToPlaySound_JumpThrow_Shout0 == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_long_03.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
				m_bIsCompletedToPlaySound_JumpThrow_Shout0 = true;
			}
		}

		if ((fTrackPosition >= 4.6f && fTrackPosition <= 4.65f)) // 쿵
		{
			if (m_bIsCompletedToPlaySound_JumpThrow == false)
			{
				m_pSoundMgr->PlaySound(L"meer_start_motion_05.wav", CHANNEL_MONSTER_BLADE, 0.8f);
				m_bIsCompletedToPlaySound_JumpThrow = true;
			}
		}
	}
	else if (m_eCurrentState == BOSS_ATTACK_JUMP_THROW_END)
	{
		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_JumpThrow_End_Shout = false;
		}

		if ((fTrackPosition >= 0.85f && fTrackPosition <= 0.9f)) // SHout
		{
			if (m_bIsCompletedToPlaySound_JumpThrow_End_Shout == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_03.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
				m_bIsCompletedToPlaySound_JumpThrow_End_Shout = true;
			}
		}
	}
	else if (m_eCurrentState == BOSS_GUARD_BEGIN)
	{
		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_GaurdBegin_Shout = false;
			m_bIsCompletedToPlaySound_GaurdBegin = false;
		}

		if ((fTrackPosition >= 0.25f && fTrackPosition <= 0.3f)) // SHout
		{
			if (m_bIsCompletedToPlaySound_GaurdBegin_Shout == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_03.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
				m_bIsCompletedToPlaySound_GaurdBegin_Shout = true;
			}
		}              

		if ((fTrackPosition >= 1.9f && fTrackPosition <= 1.95f)) // 쿵
		{
			if (m_bIsCompletedToPlaySound_GaurdBegin == false)
			{
				m_pSoundMgr->PlaySound(L"zecallion_shout_08.wav", CHANNEL_MONSTER_BLADE, 0.8f);
				m_bIsCompletedToPlaySound_GaurdBegin = true;
			}
		}
	}
	else if (m_eCurrentState == BOSS_GUARD_DURING)
	{
		if (fTrackPosition >= 0.f && fTrackPosition <= 0.05f)
		{
			m_bIsCompletedToPlaySound_GaurdDuring = false;
		}

		if ((fTrackPosition >= 0.1f && fTrackPosition <= 0.15f)) // 쿠구궁
		{
			if (m_bIsCompletedToPlaySound_GaurdDuring == false)
			{
				m_pSoundMgr->PlaySound(L"shake_ground.wav", CHANNEL_MONSTER_BLADE, 1.f);
				m_bIsCompletedToPlaySound_GaurdDuring = true;
			}
		}



		if (fTrackPosition >= 0.6f && fTrackPosition <= 0.65f)
		{
			m_bIsCompletedToPlaySound_GaurdDuring = false;
		}

		if ((fTrackPosition >= 1.f && fTrackPosition <= 1.05f)) // 쿠구궁
		{
			if (m_bIsCompletedToPlaySound_GaurdDuring == false)
			{
				m_pSoundMgr->PlaySound(L"shake_ground.wav", CHANNEL_MONSTER_BLADE, 1.f);
				m_bIsCompletedToPlaySound_GaurdDuring = true;
			}
		}

	}
	else if (m_eCurrentState == BOSS_GUARD_END)
	{

		if (fTrackPosition >= 0.1f && fTrackPosition <= 0.2f)
		{
			m_bIsCompletedToPlaySound_GaurdEnd_Shout = false;
		}

		if ((fTrackPosition >= 1.5f && fTrackPosition <= 1.55f)) // SHout
		{
			if (m_bIsCompletedToPlaySound_GaurdEnd_Shout == false)
			{
								m_pSoundMgr->PlaySound(L"zecallion_shout_01.wav", CHANNEL_MONSTER_SHOUT, 0.8f);
								m_bIsCompletedToPlaySound_GaurdEnd_Shout = true;
			}
		}
	}
 }


HRESULT CZecallion::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransCom->Get_WorldMatrix());

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pEffect->SetMatrix("g_matView", &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(1.f, -1.f, 1.f, 0.f));

	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CZecallion::Ready_Component(void)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	// For.Com_Renderer
	pComponent = m_pRendererCom = (CRenderer*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	pComponent->AddRef();


	// For.Com_Transform
	pComponent = m_pTransCom = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Front
	pComponent = m_pTransCom_Front = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Head
	pComponent = m_pTransCom_Head = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Left
	pComponent = m_pTransCom_Left = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Right
	pComponent = m_pTransCom_Right = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Left_Hand
	pComponent = m_pTransCom_Left_Hand = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Right
	pComponent = m_pTransCom_Right_Hand = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Left
	pComponent = m_pTransCom_Left_Calf = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// For.Com_Transform_Right
	pComponent = m_pTransCom_Right_Calf = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	pComponent->AddRef();
	// 	// For.Com_Transform_Foot
	// 	pComponent = m_pTransCom_Foot = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	// 	if (nullptr == pComponent)
	// 		return E_FAIL;
	// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	// 	pComponent->AddRef();


	// For.Com_Mesh (Zecallion)
	pComponent = m_pMeshCom = (CMesh_Dynamic*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Zecallion");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	pComponent->AddRef();

	// For.Com_Collision
	// 	pComponent = m_pCollisionCom = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	// 	if (nullptr == pComponent)
	// 		return E_FAIL;
	// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	// 	pComponent->AddRef();
	// 	m_pCollisionCom->SetUp_Collision(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom, 0);

	// For.Com_Collision_ForCombat
	pComponent = m_pCollisionCom_ForCombat = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_ForCombat->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom, 0, _vec3(15.f, 2.f, 7.f), _vec3(-1100.f, 0.f, 0.f));

	// For.Com_Collision --> Front
	pComponent = m_pCollisionCom_Front = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_Front", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Front->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Front, 0, _vec3(0.3f, 0.2f, 0.35f), _vec3(-25.f, -20.f, 0.f));
	// 	m_tSphereInfo_Whole.fRadius = 100; m_tSphereInfo_Whole.iSlices = 10; m_tSphereInfo_Whole.iStacks = 10; m_tSphereInfo_Whole.vCenterPos = *m_pTransCom_Front->Get_Infomation(CTransform::INFO_POSITION);
	// 	m_pCollisionCom_Front->SetUp_Collision_Sphere(CCollision::TYPE_SPHERE, m_pMeshCom, m_pTransCom_Front, 0, m_tSphereInfo_Whole);

	// For.Com_Collision -->  Head
	pComponent = m_pCollisionCom_Head = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Head->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Head, 0, _vec3(0.2f, 0.15f, 0.35f), _vec3(-15.f, -20.f, 0.f));

	// For.Com_Collision --> Left
	pComponent = m_pCollisionCom_Left = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Left->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Left, 0, _vec3(0.2f, 0.1f, 0.3f), _vec3(10.f, 0.f, 0.f));

	// For.Com_Collision --> Right
	pComponent = m_pCollisionCom_Right = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Right->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Right, 0, _vec3(0.2f, 0.1f, 0.3f), _vec3(10.f, -15.f, 0.f));

	// For.Com_Collision --> Left_Hand
	pComponent = m_pCollisionCom_Left_Hand = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_LeftHand", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Left_Hand->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Left_Hand, 0, _vec3(0.3f, 0.15f, 0.3f), _vec3(0.f, -10.f, 0.f));

	// For.Com_Collision --> Right_Hand
	pComponent = m_pCollisionCom_Right_Hand = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Right_Hand->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Right_Hand, 0, _vec3(0.2f, 0.1f, 0.2f), _vec3(-5.f, -10.f, 0.f));

	// For.Com_Collision --> Right_Calf
	pComponent = m_pCollisionCom_Right_Calf = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Right_Calf->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Right_Calf, 0, _vec3(0.4f, 0.15f, 0.3f), _vec3(15.f, -15.f, -10.f));

	// For.Com_Collision --> Left_Calf
	pComponent = m_pCollisionCom_Left_Calf = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Left_Calf->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Left_Calf, 0, _vec3(0.4f, 0.2f, 0.2f), _vec3(15.f, -10.f, 0.f));

	// For.Com_Collision --> Foot
	pComponent = m_pCollisionCom_Foot = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_Foot", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Foot->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom, 0, _vec3(0.8f, 0.3f, 1.f), _vec3(0.f, 0.f, 0.f));




	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	// For.Com_Information
	pComponent = m_pInformationCom = (CInformation*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Information_Zecallion");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Information", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

CZecallion * CZecallion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CZecallion *	pInstance = new CZecallion(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CZecallion Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CZecallion::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pTransCom_Front);
	Engine::Safe_Release(m_pTransCom_Left);
	Engine::Safe_Release(m_pTransCom_Right);
	Engine::Safe_Release(m_pTransCom_Head);
	Engine::Safe_Release(m_pTransCom_Left_Hand);
	Engine::Safe_Release(m_pTransCom_Right_Hand);
	Engine::Safe_Release(m_pTransCom_Left_Calf);
	Engine::Safe_Release(m_pTransCom_Right_Calf);
	Engine::Safe_Release(m_pMeshCom);
	//Engine::Safe_Release(m_pCollisionCom);
	Engine::Safe_Release(m_pCollisionCom_ForCombat);
	Engine::Safe_Release(m_pCollisionCom_Front);
	Engine::Safe_Release(m_pCollisionCom_Left);
	Engine::Safe_Release(m_pCollisionCom_Right);
	Engine::Safe_Release(m_pCollisionCom_Left_Hand);
	Engine::Safe_Release(m_pCollisionCom_Right_Hand);
	Engine::Safe_Release(m_pCollisionCom_Left_Calf);
	Engine::Safe_Release(m_pCollisionCom_Right_Calf);
	Engine::Safe_Release(m_pCollisionCom_Head);
	Engine::Safe_Release(m_pCollisionCom_Foot);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pInformationCom);


	return CGameObject::Free();
}
