#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "KeyMgr.h"
#include "Effect_Resonance.h"
#include "Effect_SpeedyMove.h"
#include "Trail_LongBlade.h"
#include "DamageScreen.h"
#include "SmashEffect.h"
#include "SoundMgr.h"



CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTransCom(nullptr)
	, m_pCollisionCom_Front(nullptr)
	, m_pCollisionCom_Left(nullptr)
	, m_pCollisionCom_Right(nullptr)
	, m_pCollisionCom_Back(nullptr)
	, m_pNavigationCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pKeyManager(CKeyMgr::GetInstance())
	, m_pSoundMgr(CSoundMgr::GetInstance())
/*	, m_pSoundMgr(CSoundMgr::GetInstance())*/
{
	m_MagicNum = 0.3;
}

HRESULT CPlayer::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransCom->Set_Infomation(CTransform::INFO_POSITION, &_vec3(35.0f, 0.0f, 15.f));
	m_pTransCom->Scaling(&_vec3(0.05f, 0.05f, 0.05f));

	// 	m_pTransCom_Back->Set_Infomation(CTransform::INFO_POSITION, &_vec3(3.f, 15.0f, 3.f));
	// //	m_pTransCom_Back->Scaling(&_vec3(0.05f, 0.05f, 0.05f));
	// 	m_pTransCom_Front->Set_Infomation(CTransform::INFO_POSITION, &_vec3(3.f, 15.0f, 3.f));
	// //	m_pTransCom_Front->Scaling(&_vec3(0.05f, 0.05f, 0.05f));
	// 	m_pTransCom_Left->Set_Infomation(CTransform::INFO_POSITION, &_vec3(3.f, 15.0f, 3.f));
	// //	m_pTransCom_Left->Scaling(&_vec3(0.05f, 0.05f, 0.05f));
	// 	m_pTransCom_Right->Set_Infomation(CTransform::INFO_POSITION, &_vec3(3.f, 15.0f, 3.f));
	// //	m_pTransCom_Right->Scaling(&_vec3(0.05f, 0.05f, 0.05f));

	m_fSpeed = 7.5f;
	m_fOriSpeed = m_fSpeed;

	// 뼈 Matrix, 월드 matrix
	Set_BoneMatrix_WorldMatrix();



	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	// 이 부분이 제대로 동작할때까지 검토해봐야.. 0이 나올수없는데..
	//printf("%f \n", m_pMeshCom->Get_TrackPosition());

	if (m_bIsSetBoneMatrix_ForCollider == false)
	{
		// 뼈 Matrix, 월드 matrix
		Set_BoneMatrix_WorldMatrix();
		m_bIsSetBoneMatrix_ForCollider = true;
	}

	if (nullptr == m_pTransCom || nullptr == m_pRendererCom || nullptr == m_pMeshCom)
		return -1;


	// 애니메이션셋 재생.
	if (m_bIsNotPermitted_ToPlayAnimation == false)
		m_pMeshCom->Play_AnimationSet(fTimeDelta * m_fAdvanceTime);

	// 상태에 따라 매직넘버 바꿀랭.
	Check_AnimationInState();
	//Set_MagicNum_InState();

	// 키 체크.
	Check_KeyInput(fTimeDelta);

	//// 현재 애니메이션의 Period값을 얻어와서 동작이 끝날때쯤 Idle상태로 변환하는거니까.
	Check_Period_ForAnimation();

	// 대쉬
	Move_Speed(fTimeDelta);

	// 평타 공격 때 움지깅ㅁ.
	Move_InAnimation(fTimeDelta);

	// Rotation.
	m_pTransCom->RotationY(D3DXToRadian(m_fAngle));

	// 지형 태우기.
	CVIBuffer* pBuffer = (CVIBuffer*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBuffer)
		return -1;
	tuple<_ulong, _ulong, _ulong>	BufferInfo = pBuffer->Get_BufferInfomation();
	m_pTransCom->SetUpHeight_OnBuffer(pBuffer->Get_VertexPos(), get<0>(BufferInfo), get<1>(BufferInfo), get<2>(BufferInfo));


	// 구 콜라이더 pos 업데이트.
	m_pCollisionCom_Back->Update_SpherePos(*m_pTransCom_Back->Get_Infomation(CTransform::INFO_POSITION));

	// 월드 행렬 업데이트.
	m_pTransCom->Invalidate_Worldmatrix();
	m_pTransCom_Back->Invalidate_Worldmatrix();
	// 	m_pTransCom_Right->Invalidate_Worldmatrix();
	// 	m_pTransCom_Left->Invalidate_Worldmatrix();
	//	m_pTransCom_Front->Invalidate_Worldmatrix();

	// 각 뼈들 월드 행렬 업데이트.
	Update_BoneMatrix();

	// Renderer에 Add_RenderGroup.
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);


	// 임시.
	_vec3 vPosition = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);

	// 충돌 후 무적 시간.
	if (m_bIsPermitted_Collision_WithAxe == false) // With Axe
	{
		m_fTimeDelta_Invincible_WithAxe += fTimeDelta;

		if (m_fTimeDelta_Invincible_WithAxe >= 2.f)
		{
			m_bIsPermitted_Collision_WithAxe = true;
			m_fTimeDelta_Invincible_WithAxe = 0.f;
		}
	}
	if (m_bIsPermitted_Collision_WithLeftHand == false) // With Axe
	{
		m_fTimeDelta_Invincible_WithLeftHand += fTimeDelta;

		if (m_fTimeDelta_Invincible_WithLeftHand >= 2.f)
		{
			m_bIsPermitted_Collision_WithLeftHand = true;
			m_fTimeDelta_Invincible_WithLeftHand = 0.f;
		}
	}

	// Damaged_middle_front 이후 시간 셈.
	if (m_bBeingDamagedMiddleFront == true)
	{
		m_fTimeDelta_BeingDamagedMiddleFront += fTimeDelta;

		if (m_fTimeDelta_BeingDamagedMiddleFront >= 1.f)
		{
			m_bBeingDamagedMiddleFront = false;
			m_bIsNotPermitted_ToPlayAnimation = false;
			m_fTimeDelta_BeingDamagedMiddleFront = 0.f;
		}
	}

	// 	if (m_bIsStart_toCountRunning == true)
	// 	{
	// 		m_fTimeDelta_Running += fTimeDelta;
	// 
	// 		if (m_fTimeDelta_Running >= 2.f)
	// 		{
	// 			m_bIsPermitted_Ani_RunStop = true;
	// 		}
	// 		else
	// 			m_bIsPermitted_Ani_RunStop = false;
	// 	}
	if (m_eCurrentState == Player_Run)
	{
		m_fTimeDelta_Running += fTimeDelta;

		if (m_fTimeDelta_Running >= 1.f)
		{
			m_bIsPermitted_Ani_RunStop = true;
		}
		else
			m_bIsPermitted_Ani_RunStop = false;
	}
	else
	{
		m_fTimeDelta_Running = 0.f;
	}

	// 	system("cls");
	//   	cout << "m_fTimeDelta_Running : " << m_fTimeDelta_Running << endl;
	//  	cout << "m_bIsPermitted_Ani_RunStop : " << m_bIsPermitted_Ani_RunStop << endl;
	//  	cout << "m_eCurrentState : " << m_eCurrentState << endl << endl;

	return _int();
}

_int CPlayer::LastUpdate_GameObject(const _float & fTimeDelta)
{
	// 충돌 검사.
	Update_FollowingCollision();

	return _int();
}

void CPlayer::Render_GameObject(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();


	if (g_bShowingCollider)
	{
		m_pCollisionCom_Whole->Render_Collider();
		m_pTransCom_Front->SetUp_OnGraphicDev();
		m_pCollisionCom_Front->Render_Collider();
		// 		m_pTransCom_Left->SetUp_OnGraphicDev();
		// 		m_pCollisionCom_Left->Render_Collider();
		// 		m_pTransCom_Right->SetUp_OnGraphicDev();
		// 		m_pCollisionCom_Right->Render_Collider();
		m_pTransCom_Back->SetUp_OnGraphicDev();
		m_pCollisionCom_Back->Render_Collider();
		//m_pCollisionCom_Sphere->Render_Collider();
	}
	if (g_bShowingNaviMesh)
		m_pNavigationCom->Render_Navigation();

}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CPlayer::Check_KeyInput(const _float & fTimeDelta)
{
	// 	system("cls");
	// 	cout << m_fAngle << endl;
	////////////////////// Direction.
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_A))
	{
		m_fAngle -= 120.0f * fTimeDelta;
	}

	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_D))
	{
		m_fAngle += 120.0f * fTimeDelta;
	}
	//  	if (m_pKeyManager->OnceKeyUp('S'))
	//  	{
	// 		m_fOriAngle = m_fAngle;
	// 		m_fTargetAngle = m_fOriAngle + 180.f;
	// 		m_bIsStartingTurn = true;
	//  	}

	//////////////////// Basic Movements. (Animation)
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_W) && (m_eCurrentState != Player_Attack1 && m_eCurrentState != Player_Attack2 && m_eCurrentState != Player_Attack3))
	{
		_vec3		vDir = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);

		// 네비메쉬 잠시 OFF
		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
		// 
		// 		if (CCell::MOVE_OUT != m_pNavigationCom->isMoving(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION)
		// 			, &(*D3DXVec3Normalize(&vDir, &vDir) * 3.0f * fTimeDelta)))
		// 		{
		//			if(m_bIsCollided_WithBoss == false)
		// 				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
		// 		}
		//  		else if (CCell::MOVE_OUT == m_pNavigationCom->isMoving(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION)
		//  			, &(*D3DXVec3Normalize(&vDir, &vDir) * 3.0f * fTimeDelta))) // // MOVE_OUT인데 걸쳐져 있는 상태라면? 슬라이더 베ㅐㄱ터.
		//  		{
		//  			// 1. 플레이어가 들어오고 있는 방향 벡터를 뒤집는다.
		//  			_vec3	vecReverseDir = _vec3(-vDir.x, -vDir.y, -vDir.z);
		//  			// 2. 해당 벽의 법선 벡터를 얻어온다.
		//  			_vec3 vecWallNormal = m_pNavigationCom->GetWallNormal(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION)
		//  				, &(*D3DXVec3Normalize(&vDir, &vDir) * 3.0f * fTimeDelta));
		//  			// 3. 벽의 법선 벡터를 길이 1로 만든다.
		//  			D3DXVec3Normalize(&vecWallNormal, &vecWallNormal);
		//  			// 4. 뒤집은 벡터와 벽의 법선 벡터를 내적하여 구한 길이를 벽의 법선 벡터에 곱한다.
		//  			_float fLength = D3DXVec3Dot(&vecReverseDir, &vecWallNormal);
		//  			vecWallNormal *= fLength;
		//  			// 5. 플레이어의 방향 벡터에 위 벡터를 더한다.
		//  			_vec3 vFinalDir = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK); 
		//  			vFinalDir += vecWallNormal;
		//  			// 5.1 원래 길이가 있었을 거 아님?
		//  			D3DXVec3Normalize(&vFinalDir, &vFinalDir);
		//  			_vec3 vecScaleInfo = m_pTransCom->Get_ScaleInfo();
		//  			vFinalDir.x *= vecScaleInfo.x;
		//  			vFinalDir.y *= vecScaleInfo.y;
		//  			vFinalDir.z *= vecScaleInfo.z;
		//  			// 6. 정보를 Set 해준다.
		//  			//m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vFinalDir); 
		//				if(m_bIsCollided_WithBoss == false)
		// // 				m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
		//  
		//  			// 7. 슬라이딩 벡터가 moveout인지 아닌지 판별 후 Go Straight 처리?
		//   			if (CCell::MOVE_OUT != m_pNavigationCom->isMoving(m_pTransCom->Get_Infomation(CTransform::INFO_POSITION)
		//   				, &vFinalDir))
		//   			{
		// 				m_pTransCom->Set_Infomation(CTransform::INFO_LOOK, &vFinalDir);
		//				if(m_bIsCollided_WithBoss == false)
		// 					m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);
		//   			}
		//  		}

		if (m_eCurrentState != Player_Speedy_move && m_eCurrentState != Player_Run)
		{
			m_eCurrentState = Player_Run;
		}

		m_bIsStart_toCountRunning = true;

	}
	else if (!CInput_Device::Get_Instance()->Get_DIKState(DIK_W) && m_eCurrentState == Player_Run) // 걷기 모션은 애니메이션 끝까지 돌릴 필요 없엉.
	{
		if (m_bIsPermitted_Ani_RunStop)
			m_eCurrentState = Player_Run_Stop;
		else
			m_eCurrentState = Player_idle;
	}


	// LBUTTON 기본 공격.

	if (m_pKeyManager->OnceKeyUp(VK_LBUTTON))
	{
		if (m_bIsOnManaBlade == false) // 마나 블레이드 OFF
		{
			if (m_eCurrentState != Player_Attack1 && m_eCurrentState != Player_Attack2 && m_eCurrentState != Player_Attack3) // 1타 기본 공격.
			{
				m_pSoundMgr->PlaySound(L"arisha_attack_01.wav", CHANNEL_PLAYER, 0.7f);
				m_pSoundMgr->PlaySound(L"arisha_swing_longblade_01.wav", CHANNEL_BLADE, 0.7f);
				m_eCurrentState = Player_Attack1;
			}
			else if (m_eCurrentState == Player_Attack1 && m_eCurrentState != Player_Attack2 && m_eCurrentState != Player_Attack3) // 1타->2타
			{
				m_pSoundMgr->PlaySound(L"arisha_attack_02.wav", CHANNEL_PLAYER, 0.7f);
				m_pSoundMgr->PlaySound(L"arisha_swing_longblade_02.wav", CHANNEL_BLADE, 0.7f);
				m_eCurrentState = Player_Attack2;
			}
			else if (m_eCurrentState == Player_Attack2 && m_eCurrentState != Player_Attack3 && m_eCurrentState != Player_Attack1) // 2타->3타
			{
				m_pSoundMgr->PlaySound(L"arisha_attack_03.wav", CHANNEL_PLAYER, 0.7f);
				m_pSoundMgr->PlaySound(L"arisha_swing_longblade_03.wav", CHANNEL_BLADE, 0.7f);
				m_eCurrentState = Player_Attack3;
			}

		}

		else // 마나 블레이드 ON
		{
			if (m_eCurrentState != Player_Attack_Strong1 && m_eCurrentState != Player_Attack_Strong4 && m_eCurrentState != Player_Attack_Strong5) // 1타 기본 공격.
			{
				m_pSoundMgr->PlaySound(L"arisha_attack_strong_01.wav", CHANNEL_PLAYER, 0.7f);
				m_pSoundMgr->PlaySound(L"arisha_swing_longblade_strong_01.wav", CHANNEL_BLADE, 0.7f);
				m_fAngle += 120.0f * fTimeDelta;
				m_eCurrentState = Player_Attack_Strong1;
			}
			else if (m_eCurrentState == Player_Attack_Strong1 && m_eCurrentState != Player_Attack_Strong4 && m_eCurrentState != Player_Attack_Strong5) // 1타->2타
			{
				m_pSoundMgr->PlaySound(L"arisha_attack_strong_04.wav", CHANNEL_PLAYER, 0.7f);
				m_pSoundMgr->PlaySound(L"arisha_swing_longblade_strong_02.wav", CHANNEL_BLADE, 0.7f);

				m_eCurrentState = Player_Attack_Strong4;
			}
			else if (m_eCurrentState == Player_Attack_Strong4 && m_eCurrentState != Player_Attack_Strong5 && m_eCurrentState != Player_Attack_Strong1) // 2타->3타
			{
				m_pSoundMgr->PlaySound(L"arisha_attack_strong_05.wav", CHANNEL_PLAYER, 0.7f);
				m_pSoundMgr->PlaySound(L"arisha_swing_longblade_strong_03.wav", CHANNEL_BLADE, 0.7f);

				m_eCurrentState = Player_Attack_Strong5;
			}
		}

		Create_Effect();
	}

	// RBUTTON 마나 스톤.
	if (m_pKeyManager->OnceKeyUp(VK_RBUTTON))
	{
		//		m_eCurrentState = Player_Teleport_mana_stone;
		m_eCurrentState = Player_Attack_mb_strong1;
		Create_Effect();
	}

	// Dash.
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_SPACE) && m_eCurrentState != Player_Speedy_move)
	{
		m_pSoundMgr->PlaySound(L"effect_instant_healing_erg.wav", CHANNEL_PLAYER);
		m_eCurrentState = Player_Speedy_move;
		//m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta); // Move_Speed 에서 해줄거얌
		m_bIsMovingSpeed = true;
		Create_Effect();
	}
	if (m_pKeyManager->OnceKeyUp(VK_SHIFT))
	{
		//		m_fOriAngle = m_fAngle;
		m_fTargetAngle = m_fAngle + 180.f;
		m_fAngle = m_fTargetAngle;

		m_eCurrentState = Player_Speedy_move;
		//m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta); // Move_Speed 에서 해줄거얌
		m_bIsMovingSpeed = true;
		Create_Effect();
	}

	////////////////// Skill.
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_1) && m_eCurrentState != Player_Attack_Resonance) // 1 Resonance
	{
		m_eCurrentState = Player_Attack_Resonance;
		//Create_Effect();
	}
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_2) && m_eCurrentState != Player_Mana_region) // 2 Mana region
	{
		m_eCurrentState = Player_Mana_region;
	}
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_3) && m_eCurrentState != Player_Attack_lord_of_mana) // 3 lord of mana
	{
		m_eCurrentState = Player_Attack_lord_of_mana;
	}
	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_4) && m_eCurrentState != Player_Attack_ruin_blade) // ruin Blade
	{
		m_eCurrentState = Player_Attack_ruin_blade;
	}

	/////////////////////// Mode.
	if (m_pKeyManager->OnceKeyUp('Z')) // Mana Blade On.
	{
		m_pSoundMgr->PlaySound(L"mana_blade.wav", CHANNEL_PLAYER, 0.7f);
		m_eCurrentState = Player_Mana_blade;
		m_bIsOnManaBlade = !m_bIsOnManaBlade;
	}

	/////////////////////// Showing Mode.
	if (m_pKeyManager->OnceKeyUp(VK_F3))
	{
		g_bShowingNaviMesh = !g_bShowingNaviMesh;
	}
}

void CPlayer::Check_AnimationInState()
{
	// 	system("cls");
	// 	cout << m_MagicNum << endl;

	if (m_ePreviousState != m_eCurrentState)
	{

		m_ePreviousState = m_eCurrentState;

		// 공격은 switch 말고 if
		if (m_eCurrentState == Player_Attack1 || m_eCurrentState == Player_Attack_Strong1 || m_eCurrentState == Player_Attack_Strong4 || m_eCurrentState == Player_Attack_Strong5)
		{
			m_fAdvanceTime = 2.f;
			m_MagicNum = 0.25;
		}
		else if (m_eCurrentState == Player_Attack2 || m_eCurrentState == Player_Attack3)
		{
			m_fAdvanceTime = 2.f;
			m_MagicNum = 0.3;
		}
		else  if (m_eCurrentState == Player_Damage_light_front)
		{
			m_MagicNum = 0.3;
		}
		else
		{
			switch (m_eCurrentState)
			{
			case Player_Run:
				m_MagicNum = 0.25;
				m_fAdvanceTime = 1.3f;
				break;
			case Player_Run_Stop:
				m_MagicNum = 0.25;
				m_fAdvanceTime = 1.8f;
				break;
			case Player_Speedy_move:
				m_MagicNum = 0.25;
				m_fAdvanceTime = 1.7;
				break;
			case Player_Mana_blade:
				m_MagicNum = 0.2;
				m_fAdvanceTime = 1.4f;
				break;
			case 	Player_idle_mb:
				m_fAdvanceTime = 1.2f;
				break;
			default:
				m_MagicNum = 0.25;
				m_fAdvanceTime = 1.4f;
				break;
			}
		}

		m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
		m_pMeshCom->SetTrackPos(0);
	}
}

void CPlayer::Create_Effect()
{
	// GameObject Add
	CGameObject* pGameObject = nullptr;

	if (m_eCurrentState == Player_Attack_Resonance)
	{
		pGameObject = CEffect_Resonance::Create(m_pGraphicDev);
	}
	else if (m_eCurrentState == Player_Speedy_move)
	{
		pGameObject = CEffect_SpeedyMove::Create(m_pGraphicDev);
	}
	else if (m_eCurrentState == Player_Attack1)
	{
		pGameObject = CTrail_LongBlade::Create(m_pGraphicDev);
	}
	else if (m_eCurrentState == Player_Attack_mb_strong1)
	{
	m_pSoundMgr->PlaySound(L"magic_blink_appear.wav", CHANNEL_PLAYER_EFFECT, 0.9f);

		_vec3	vPosition;
		const _matrix* matParent;
		const _matrix* matWorld = m_pTransCom->Get_WorldMatrix();

		matParent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Anim_Attachment_LH");
		const _matrix matResult = (*matParent) * (*matWorld);
		vPosition = *(_vec3*)(&matResult.m[3][0]);

		pGameObject = CEffect_Smash::Create(m_pGraphicDev, vPosition);
	}

	if (nullptr == pGameObject)
		return;

	CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject);
}

void CPlayer::Create_UI()
{
	CGameObject* pGameObject = nullptr;


	pGameObject = CDamageScreen::Create(m_pGraphicDev);


	if (nullptr == pGameObject)
		return;

	CObject_Manager::Get_Instance()->Add_GameObject(SCENE_STAGE, L"Layer_DamageScreen", pGameObject);
}

void CPlayer::Check_Period_ForAnimation()
{
	if (m_bIsStrongMode == false)
	{
		if (m_pMeshCom->Get_Period() - m_MagicNum <= m_pMeshCom->Get_TrackPosition()
			&& (m_eCurrentState != Player_Run)) // Run 할 때는 애니메이션 끝까지 돌릴 필요 없당.
		{
			if (m_eCurrentState == Player_Damage_middle_front_begin)
			{
				if (m_bBeingDamagedMiddleFront == false)
					m_eCurrentState = Player_Damage_middle_front_end;
				else
					m_bIsNotPermitted_ToPlayAnimation = true;
			}
			else
			{
				if (m_bIsOnManaBlade == false)
				{
					// Run stop 하고 나서 idle 되도록 처리.
					if (m_bIsPermitted_Ani_RunStop)
					{
						m_eCurrentState = Player_Run_Stop;
						m_bIsPermitted_Ani_RunStop = false;
					}
					else
						m_eCurrentState = Player_idle;
				}
				else
				{
					m_eCurrentState = Player_idle_mb;
				}
			}
			m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
			//			m_pMeshCom->SetTrackPos(0);
		}
	}

}

void CPlayer::Set_MagicNum_InState()
{
	//system("cls");
	//	cout << m_MagicNum << endl;
	//
	//	if (m_eCurrentState == Player_Attack2)
	//	{
	//		m_MagicNum = 0.3;
	//	}
	//	else if (m_eCurrentState == Player_Attack3)
	//	{
	//		m_MagicNum = 0.3;
	//	}
	//	else if (m_eCurrentState == Player_Speedy_move)
	//	{
	//		m_MagicNum = 0.3;
	//	}
	//	else if (m_eCurrentState == Player_Attack_Strong1)
	//	{
	//		m_MagicNum = 0.3;
	//	}
	//	else if (m_eCurrentState == Player_Attack_Strong4)
	//	{
	//		m_MagicNum = 0.3;
	//	}
	//	else if (m_eCurrentState == Player_Attack_Strong5)
	//	{
	//		m_MagicNum = 0.3;
	//	}
	//	else if(m_eCurrentState != Player_idle && m_eCurrentState != Player_idle_mb)
	//	{
	//		m_MagicNum = 0.25;
	//	}
	//	m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
	//	m_pMeshCom->SetTrackPos(0);
}

void CPlayer::Set_BoneMatrix_WorldMatrix()
{
	m_pBoneMatrix_Back_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_Spine2");
	m_pWorldMatrix_Back_Parent = m_pTransCom->Get_WorldMatrix();

	m_pBoneMatrix_Front_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_R_Breast");
	m_pWorldMatrix_Front_Parent = m_pTransCom->Get_WorldMatrix();

	m_pBoneMatrix_Left_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_L_UpperArm01");
	m_pWorldMatrix_Left_Parent = m_pTransCom->Get_WorldMatrix();

	m_pBoneMatrix_Right_Parent = m_pMeshCom->Get_BoneMatrix("ValveBiped_Bip01_R_UpperArm01");
	m_pWorldMatrix_Right_Parent = m_pTransCom->Get_WorldMatrix();

}

void CPlayer::Move_Speed(const _float & fTimeDelta)
{

	if (m_bIsMovingSpeed == false)
		return;

	//cout << m_pMeshCom->Get_TrackPosition() << endl;

	if (m_pMeshCom->Get_TrackPosition() <= 0.2)
	{
		return;
	}
	if (m_pMeshCom->Get_TrackPosition() >= 1.5) // 일정 트랙 포지션 넘으면 끈다.
	{
		m_bIsMovingSpeed = false;
		m_fSpeed = m_fOriSpeed;
		return;
	}

	if (m_pMeshCom->Get_TrackPosition() < 1.8 && m_pMeshCom->Get_TrackPosition() >= 1.0) // 트랙 포지션에 따라서 speed 다르게.
	{
		m_fSpeed = m_fOriSpeed * 0.2f;
	}
	else
	{
		m_fSpeed = m_fOriSpeed * 2.f;
	}

	m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);



	//m_fTimeSpeedyMove += fTimeDelta;

	//m_fSpeed = m_fOriSpeed * 2.f;

	//m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);

	//if (m_fTimeSpeedyMove >= 0.35f)
	//{
	//	m_bIsMovingSpeed = false;
	//	m_fTimeSpeedyMove = 0.f;
	//	m_fSpeed = m_fOriSpeed;
	//}
}


void CPlayer::Move_InAnimation(const _float & fTimeDelta)
{
	// dash - back 분기.
	if (m_eCurrentState == Player_Attack1)
	{

		if (m_pMeshCom->Get_TrackPosition() >= 0.2	 && m_pMeshCom->Get_TrackPosition() <= 0.3)
		{
			m_bIsDashInAttack1 = true;
		}
		if (m_pMeshCom->Get_TrackPosition() >= 1.9 && m_pMeshCom->Get_TrackPosition() <= 2.0)
		{
			//m_bIsBackInAttack1 = true;
		}
	}
	if (m_eCurrentState == Player_Attack2)
	{
		// 2.1
		//cout << m_pMeshCom->Get_TrackPosition() << endl;

		if (m_pMeshCom->Get_TrackPosition() >= 0.3	 && m_pMeshCom->Get_TrackPosition() <= 0.4)
		{
			m_bIsDashInAttack2 = true;
		}
		if (m_pMeshCom->Get_TrackPosition() >= 1.9 && m_pMeshCom->Get_TrackPosition() <= 2.0)
		{
			//m_bIsBackInAttack2 = true;
		}
	}
	if (m_eCurrentState == Player_Attack3)
	{
		// 2.1
		//cout << m_pMeshCom->Get_TrackPosition() << endl;

		if (m_pMeshCom->Get_TrackPosition() >= 0.3	 && m_pMeshCom->Get_TrackPosition() <= 0.4)
		{
			m_bIsDashInAttack3 = true;
		}
		if (m_pMeshCom->Get_TrackPosition() >= 1.9 && m_pMeshCom->Get_TrackPosition() <= 2.0)
		{
			//m_bIsBackInAttack3 = true;
		}
	}

	if (m_eCurrentState == Player_Attack_Strong1)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 0.6	 && m_pMeshCom->Get_TrackPosition() <= 0.7)
		{
			m_bIsDashInAttackStrong1 = true;
		}
		if (m_pMeshCom->Get_TrackPosition() >= 1.9 && m_pMeshCom->Get_TrackPosition() <= 2.0)
		{
			//m_bIsBackInAttack3 = true;
		}
	}
	if (m_eCurrentState == Player_Attack_Strong4)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 0.8	 && m_pMeshCom->Get_TrackPosition() <= 0.9)
		{
			m_bIsDashInAttackStrong4 = true;
		}
		if (m_pMeshCom->Get_TrackPosition() >= 1.9 && m_pMeshCom->Get_TrackPosition() <= 2.0)
		{
			//m_bIsBackInAttack3 = true;
		}
	}
	if (m_eCurrentState == Player_Attack_Strong5)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 0.6	 && m_pMeshCom->Get_TrackPosition() <= 0.7)
		{
			m_bIsDashInAttackStrong5 = true;
		}
		if (m_pMeshCom->Get_TrackPosition() >= 1.9 && m_pMeshCom->Get_TrackPosition() <= 2.0)
		{
			//m_bIsBackInAttack3 = true;
		}
	}

	if (m_eCurrentState == Player_Damage_light_front)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 0.2	 && m_pMeshCom->Get_TrackPosition() <= 0.3)
		{
			m_bIsBack_InLightFrontDamaged = true;
		}
	}
	if (m_eCurrentState == Player_Damage_middle_front_begin)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 0.2	 && m_pMeshCom->Get_TrackPosition() <= 0.3)
		{
			m_bIsBack_InMiddleFrontDamaged = true;
		}
	}

	//
	if (m_bIsDashInAttack1)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 3.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.1f)
		{
			m_bIsDashInAttack1 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsBackInAttack1)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		//m_fSpeed = m_fOriSpeed * 2.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(-m_fSpeed * fTimeDelta);

		if (m_fTimeSpeedyMove >= 0.04f)
		{
			m_bIsBackInAttack1 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsDashInAttack2)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 3.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.1f)
		{
			m_bIsDashInAttack2 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsDashInAttack3)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 3.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.1f)
		{
			m_bIsDashInAttack3 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsDashInAttackStrong1)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 3.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.1f)
		{
			m_bIsDashInAttackStrong1 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsDashInAttackStrong4)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 1.5f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.1f)
		{
			m_bIsDashInAttackStrong4 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}
	if (m_bIsDashInAttackStrong5)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 3.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.15f)
		{
			m_bIsDashInAttackStrong5 = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsBack_InLightFrontDamaged)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 3.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(-m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.3f)
		{
			m_bIsBack_InLightFrontDamaged = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	if (m_bIsBack_InMiddleFrontDamaged)
	{
		m_fTimeSpeedyMove += fTimeDelta;

		m_fSpeed = m_fOriSpeed * 4.f;

		if (m_bIsCollided_WithBoss == false)
			m_pTransCom->Go_Staight(-m_fSpeed * fTimeDelta);


		if (m_fTimeSpeedyMove >= 0.2f)
		{
			m_bIsBack_InMiddleFrontDamaged = false;
			m_fTimeSpeedyMove = 0.f;
			m_fSpeed = m_fOriSpeed;
		}
	}

	else
	{
		return;
	}
}

void CPlayer::Update_BoneMatrix()
{
	m_pTransCom_Left->Set_Parent((*m_pBoneMatrix_Left_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Right->Set_Parent((*m_pBoneMatrix_Right_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Back->Set_Parent((*m_pBoneMatrix_Back_Parent) * (*m_pTransCom->Get_WorldMatrix()));
	m_pTransCom_Front->Set_Parent((*m_pBoneMatrix_Front_Parent) * (*m_pTransCom->Get_WorldMatrix()));


	//  	_vec3 vPlayerPosition = *m_pTransCom->Get_Infomation(CTransform::INFO_POSITION);
	//  	m_pTransCom_Back->Set_Infomation(CTransform::INFO_POSITION, &vPlayerPosition);
	// 	m_pCollisionCom_Back->Update_SpherePos(vPlayerPosition);
	// 	const _matrix* matTransSParent = m_pTransCom_Back->Get_ParentMatrix_ForCollisionBox();
	// 	m_pCollisionCom_Back->Update_SpherePos(*((_vec3*)&matTransSParent->m[3][0]));
}

void CPlayer::Update_FollowingCollision()
{
	// sphere 정보에 플레이어 position 세팅.
	//m_pCollisionCom_Back->Update_SpherePos(*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));
	const _matrix* matTransSParent = m_pTransCom_Back->Get_ParentMatrix_ForCollisionBox();
	m_pCollisionCom_Back->Update_SpherePos(*((_vec3*)&matTransSParent->m[3][0]));
	// 보스 Sphere - 플레이어 Sphere
	// 구충돌 실패함 ..ㅜ ㅜ
	// 	CCollision* pBossCollision_Front = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Collision_Front", 0);
	// 	if (m_pCollisionCom_Back->Collision_Sphere(pBossCollision_Front))
	// 	{
	// 		m_bIsPermitted_OBB_DependingOnWholeSphere = true;
	// 	}
	// 	else
	// 		m_bIsPermitted_OBB_DependingOnWholeSphere = false;


	// 보스 Foot - 플레이어 Whole
	//	CCollision* pBossCollision_Foot = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Collision_Foot", 0);
	//
	//	if (m_pCollisionCom_Whole->Collision_OBB(pBossCollision_Foot) &&
	//		(m_eCurrentState == Player_Run || m_eCurrentState == Player_Run_Stop || m_eCurrentState == Player_Attack1 || m_eCurrentState == Player_Attack2 || m_eCurrentState == Player_Attack3))
	//	{ 
	//		CTransform* pBossTransform = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Transform", 0);
	//		_vec3 vBossPos = *pBossTransform->Get_Infomation(CTransform::INFO_POSITION);
	//		_vec3 vBossLook = *pBossTransform->Get_Infomation(CTransform::INFO_LOOK);
	//
	//		_vec3 vBossToPlayer = vBossPos - (*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION));
	//		_vec3 vPlayerToBoss = (*m_pTransCom->Get_Infomation(CTransform::INFO_POSITION)) - vBossPos;
	//		_vec3 vPlayerLook = *m_pTransCom->Get_Infomation(CTransform::INFO_LOOK);
	//
	//		float fCos = D3DXVec3Dot(D3DXVec3Normalize(&vBossLook, &vBossLook), D3DXVec3Normalize(&vPlayerLook, &vPlayerLook));
	//		float fAngle = D3DXToDegree(acosf(fCos));
	//
	//		float fTheta = D3DXToDegree(acosf(D3DXVec3Dot(D3DXVec3Normalize(&vPlayerToBoss, &vPlayerToBoss), D3DXVec3Normalize(&vBossToPlayer, &vBossToPlayer))));
	//
	////   		system("cls");
	////   		cout << "fAngle : " << fAngle << endl;
	//// 			cout << "fTheta : " << fTheta << endl;
	//
	//		if (fAngle < 60 || fAngle > 150)
	//			m_bIsCollided_WithBoss = false;
	//		else
	//			m_bIsCollided_WithBoss = true;
	//	}
	//	else
	//	{
	//		m_bIsCollided_WithBoss = false;
	//	}

	// 보스 - 플레이어 
	CCollision* pZecallion_Axe = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Collision_Axe", 1);
	CCollision* pZecallion_LeftHand = (CCollision*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Collision_LeftHand", 0);
	CMesh_Dynamic* pZecallion_Mesh = (CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Mesh", 0);
	_float fZecallion_GetTrackPosition = pZecallion_Mesh->Get_TrackPosition();
	_int iZecallionIdx = pZecallion_Mesh->Get_AnimationIdx();

	// 	system("cls");
	// 	cout << fZecallion_GetTrackPosition << endl;
	// 	cout << iZecallionIdx << endl;

	// 보스 Axe - 플레이어 Front
	if (m_pCollisionCom_Front->Collision_OBB(pZecallion_Axe) && m_bIsPermitted_Collision_WithAxe == true)
	{
		CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
		OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
		_float fAtt = tInfo.fAtt;

		if (fZecallion_GetTrackPosition >= 3.5 && fZecallion_GetTrackPosition <= 4.4 && iZecallionIdx == 1) // attack_Combo_0
		{
			m_eCurrentState = Player_Damage_light_front;
//			m_pSoundMgr->PlaySound(L"arisha_hurt_weak_01.wav", CHANNEL_PLAYER_DAMAGE);
			m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
			m_bIsPermitted_Collision_WithAxe = false;
			m_pInformationCom->HPMinus(fAtt);
		}
		if (fZecallion_GetTrackPosition >= 1.1 && fZecallion_GetTrackPosition <= 3.3 && iZecallionIdx == 2) // attack_Combo_1
		{
			m_eCurrentState = Player_Damage_middle_front_begin;
//			m_pSoundMgr->PlaySound(L"arisha_hurt_medium_01.wav", CHANNEL_PLAYER_DAMAGE);
			m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
			m_bIsPermitted_Collision_WithAxe = false;
			m_bBeingDamagedMiddleFront = true;
			m_pInformationCom->HPMinus(fAtt);
		}
		if (((fZecallion_GetTrackPosition >= 2.8 && fZecallion_GetTrackPosition <= 3.5) || (fZecallion_GetTrackPosition >= 5.3 && fZecallion_GetTrackPosition <= 7.5))
			&& iZecallionIdx == 3) // attack_DoubleMash
		{
			m_eCurrentState = Player_Damage_middle_front_begin;
			m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
			m_bIsPermitted_Collision_WithAxe = false;
			m_pInformationCom->HPMinus(fAtt * 1.5f);
			Create_UI();
		}


	}

	// 보스 LHand - 플레이어 Front
	if (m_pCollisionCom_Front->Collision_OBB(pZecallion_LeftHand) && m_bIsPermitted_Collision_WithLeftHand == true)
	{
		CInformation* pPlayerInformation = (CInformation*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Information", 0);
		OBJ_INFO tInfo = pPlayerInformation->Get_ObjInfo();
		_float fAtt = tInfo.fAtt;

		if (fZecallion_GetTrackPosition >= 0.6 && fZecallion_GetTrackPosition <= 1.6 && iZecallionIdx == 1) // attack_Combo_0
		{
			m_eCurrentState = Player_Damage_light_front;
			m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
			m_bIsPermitted_Collision_WithLeftHand = false;
			m_pInformationCom->HPMinus(fAtt);
		}
		if (fZecallion_GetTrackPosition >= 0.6 && fZecallion_GetTrackPosition <= 1.2 && iZecallionIdx == 5) // Attack_Jump_End
		{
			m_eCurrentState = Player_Damage_light_front;
			m_pMeshCom->Set_AnimationSet(m_eCurrentState, m_MagicNum);
			m_bIsPermitted_Collision_WithLeftHand = false;
			m_pInformationCom->HPMinus(fAtt);
		}
	}

}

HRESULT CPlayer::Ready_Component(void)
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
	// For.Com_Transform_Back
	pComponent = m_pTransCom_Back = (CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
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


	// For.Com_Mesh (Player)
	pComponent = m_pMeshCom = (CMesh_Dynamic*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Player");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	pComponent->AddRef();

	// For.Com_Collision --> Whole
	pComponent = m_pCollisionCom_Whole = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_Whole", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Whole->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom, 0, _vec3(1.f, 2.5f, 2.3f), _vec3(0.f, -100.f, 0.f));
	// For.Com_Collision --> Front
	pComponent = m_pCollisionCom_Front = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_Front", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Front->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Front, 0, _vec3(0.6f, 1.5f, 4.5f), _vec3(0.f, -90.f, 0.f));
	// For.Com_Collision --> Back
	pComponent = m_pCollisionCom_Back = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision_Back", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Back->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Back, 0, _vec3(0.3f, 0.7f, 0.4f), _vec3(0.f, -37.f, -5.f));
	// 	m_tSphereInfo_Whole.fRadius = 50.f; m_tSphereInfo_Whole.iSlices = 10; m_tSphereInfo_Whole.iStacks = 10; m_tSphereInfo_Whole.vCenterPos = *m_pTransCom_Front->Get_Infomation(CTransform::INFO_POSITION);
	// 	m_pCollisionCom_Back->SetUp_Collision_Sphere(CCollision::TYPE_SPHERE, m_pMeshCom, m_pTransCom_Back, 0, m_tSphereInfo_Whole);

	// For.Com_Collision --> Left
	pComponent = m_pCollisionCom_Left = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Left->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Left, 0, _vec3(0.3f, 0.2f, 0.4f), _vec3(0.f, -8.f, 0.f));
	// For.Com_Collision --> Right
	pComponent = m_pCollisionCom_Right = (CCollision*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	pComponent->AddRef();
	m_pCollisionCom_Right->SetUp_Collision_Ex(CCollision::TYPE_OBB, m_pMeshCom, m_pTransCom_Right, 0, _vec3(0.3f, 0.2f, 0.4f), _vec3(0.f, -10.f, 0.f));


	// For.Com_Collision_Sphere
	// 	pComponent = m_pCollisionCom_Sphere = (CCollision_Sphere*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collision_Sphere");
	// 	if (nullptr == pComponent)
	// 		return E_FAIL;
	// 	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Collision", pComponent));
	// 	pComponent->AddRef();

	// For.Com_Shader
	pComponent = m_pShaderCom = (CShader*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	pComponent->AddRef();

	// For.Com_Navigation
	pComponent = m_pNavigationCom = (CNavigation*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Navigation", pComponent));
	pComponent->AddRef();
	m_pNavigationCom->SetUp_CurrentIndex(0);

	// For.Com_Information
	pComponent = m_pInformationCom = (CInformation*)m_pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Information_Player");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent.insert(CGameObject::MAPCOMPONENT::value_type(L"Com_Information", pComponent));
	pComponent->AddRef();

	return NOERROR;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CPlayer::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pTransCom_Front);
	Engine::Safe_Release(m_pTransCom_Left);
	Engine::Safe_Release(m_pTransCom_Right);
	Engine::Safe_Release(m_pTransCom_Back);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pCollisionCom_Front);
	Engine::Safe_Release(m_pCollisionCom_Left);
	Engine::Safe_Release(m_pCollisionCom_Right);
	Engine::Safe_Release(m_pCollisionCom_Back);
	Engine::Safe_Release(m_pCollisionCom_Whole);
	Engine::Safe_Release(m_pNavigationCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pInformationCom);



	return CGameObject::Free();
}
