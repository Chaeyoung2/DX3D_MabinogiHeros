#include "stdafx.h"
#include "..\Headers\Static_Camera.h"
#include "Object_Manager.h"
#include "Transform.h"
#include "KeyMgr.h"

CStatic_Camera::CStatic_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pKey_Manager(CKeyMgr::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CStatic_Camera::Ready_GameObject(CAMERAINFO * pCamInfo, PROJINFO * pProjInfo)
{
	m_fSpeed = 5.f;

	m_CameraInfo.vEye = pCamInfo->vEye;
	m_CameraInfo.vAt = pCamInfo->vAt;
	m_CameraInfo.vAxisY = pCamInfo->vAxisY;

	if (nullptr != pProjInfo)
	{
		m_ProjInfo.fFovY = pProjInfo->fFovY;
		m_ProjInfo.fAspect = pProjInfo->fAspect;
		m_ProjInfo.fNear = pProjInfo->fNear;
		m_ProjInfo.fFar = pProjInfo->fFar;
	}


	if (CCamera::Ready_GameObject())
		return E_FAIL;

	return NOERROR;
}

_int CStatic_Camera::Update_GameObject(const _float & fTimeDelta)
{
	// 인풋 디바이스 말고 키 매니저 써야될듯.
	if (m_pKey_Manager->OnceKeyUp(VK_F2)) // 스태틱->다이나믹
	{
		g_bUsingStaticCamera = !g_bUsingStaticCamera;
	}
	if (m_pKey_Manager->OnceKeyUp(VK_F1)) // 바운딩 박스  on / off
	{
		g_bShowingCollider = !g_bShowingCollider;
	}

	if (g_bUsingStaticCamera == false)
		return _int();

	



	if (nullptr == m_pInput_Device)
		return -1;

	// Shaking 타임 카운트.

	if (m_bIsZecallion_StandUpFaceDown == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_StandUpFaceDown = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}
	if (m_bIsZecallion_Run == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_Run = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}
	if (m_bIsZecallion_AttackCombo1 == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_AttackCombo1 = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}

	if (m_bIsZecallion_DoubleMash == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_DoubleMash = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}

	if (m_bIsZecallion_GaurdBegin == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_GaurdBegin = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}


	if (m_bIsZecallion_JumpThrow == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_JumpThrow = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}

	if (m_bIsZecallion_Spawn == true)
	{
		m_fTimeDelta_Shaking += fTimeDelta;
		if (m_fTimeDelta_Shaking >= m_fShakeTime)
		{
			m_bIsZecallion_Spawn = false;
			m_fTimeDelta_Shaking = 0.f;
		}
	}

	CCamera::Update_GameObject(fTimeDelta);


	return _int();
}

_int CStatic_Camera::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (g_bUsingStaticCamera == false)
		return _int();

	// Default _ 플레이어 쫓기.
	if (g_iScene != SCENE_STATIC && !m_bIsCutSceneMode_InitZecallion)
	{
		if (!m_bIsZecallion_StandUpFaceDown && !m_bIsZecallion_DoubleMash && !m_bIsZecallion_AttackCombo1 && !m_bIsZecallion_GaurdBegin && !m_bIsZecallion_JumpThrow
			&& !m_bIsZecallion_Spawn)
		{
			CTransform* pTransCom_Player = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);

			_vec3 vPlayer_Position(0.f, 0.f, 0.f);

			if (pTransCom_Player != nullptr)
				vPlayer_Position = *pTransCom_Player->Get_Infomation(CTransform::INFO_POSITION);

			_float fGapZ = 15.f; _float fGapY = 5.f; _float fGapX = 3.f;

			m_CameraInfo.vEye = _vec3(vPlayer_Position.x, vPlayer_Position.y + fGapY, vPlayer_Position.z - fGapZ);
			m_CameraInfo.vAt = _vec3(vPlayer_Position.x + fGapX, vPlayer_Position.y, vPlayer_Position.z);
		}
	}

	// Shaking _ InBossAnimation.
	if (g_iScene == SCENE_STAGE)
	{
		CMesh_Dynamic* pZecallion_Mesh = (CMesh_Dynamic*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Mesh", 0);
		_float fZecallion_GetTrackPosition = pZecallion_Mesh->Get_TrackPosition();
		m_iCurZecallionAniIdx = pZecallion_Mesh->Get_AnimationIdx();


		///////////////////////////////////////// 분기 ///////////////////////////////////////////
		if (m_iCurZecallionAniIdx == 9 && m_bIsCutSceneMode_InitZecallion == true)
		{
			// 보스 IDLE 상태, 컷씬 ON 상태라면 컷씬 OFF.
			m_bIsCutSceneMode_InitZecallion = false;
		}

		if (m_iCurZecallionAniIdx == 13) // StandUp_FaceDown.
		{
			// <컷 씬> ON
			m_bIsCutSceneMode_InitZecallion = true;
			
			if (fZecallion_GetTrackPosition >= 1.4 && fZecallion_GetTrackPosition <= 1.45) //  쉐이킹 1.4부터~
			{
				CTransform* pTransCom_Player = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);

				_vec3 vPlayer_Position(0.f, 0.f, 0.f);

				if (pTransCom_Player != nullptr)
					vPlayer_Position = *pTransCom_Player->Get_Infomation(CTransform::INFO_POSITION);

				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_StandUpFaceDown = true;
				m_fShakeTime = 2.1f;
				m_fShakePower = 0.07f;
			}
		}

		if (m_iCurZecallionAniIdx == 0) // Run.
		{
			if ((fZecallion_GetTrackPosition >= 0.9f && fZecallion_GetTrackPosition <= 0.95f)
				|| (fZecallion_GetTrackPosition >= 0.2f && fZecallion_GetTrackPosition <= 0.25f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_Run = true;
				m_fShakeTime = 0.1f;
				m_fShakePower = 0.06f;
			}
		}

		if (m_iCurZecallionAniIdx == 8) // Spawn.
		{
			if ((fZecallion_GetTrackPosition >= 15.5f && fZecallion_GetTrackPosition <= 16.f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_Spawn = true;
				m_fShakeTime = 2.1f;
				m_fShakePower = 0.07f;
				m_bIsSettingCompleted_InitZecallionPos = false; // 이때 흔들면서 좌표 한번 더 받아와ㅏ야 해.
			}
		}

		if (m_iCurZecallionAniIdx == 2) // Attack_Combo_1
		{
			if ((fZecallion_GetTrackPosition >= 1.7f && fZecallion_GetTrackPosition <= 1.75f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_AttackCombo1 = true;
				m_fShakeTime = 0.2f;
				m_fShakePower = 0.08f;
			}
		}

		if (m_iCurZecallionAniIdx == 3) // Double_Mash
		{
			if ((fZecallion_GetTrackPosition >= 3.05f && fZecallion_GetTrackPosition <= 3.1f)
				|| (fZecallion_GetTrackPosition >= 5.4f && fZecallion_GetTrackPosition <= 5.45f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_DoubleMash = true;
				m_fShakeTime = 0.2f;
				m_fShakePower = 0.07f;
			}
		}

		if (m_iCurZecallionAniIdx == 17) // Gaurd_Begin
		{
			if ((fZecallion_GetTrackPosition >= 2.f && fZecallion_GetTrackPosition <= 2.05f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_GaurdBegin = true;
				m_fShakeTime = 6.f;
				m_fShakePower = 0.02f;
			}
		}

		if (m_iCurZecallionAniIdx == 4) // Jump_Throw
		{
			if ((fZecallion_GetTrackPosition >= 1.3f && fZecallion_GetTrackPosition <= 1.35f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_JumpThrow = true;
				m_fShakeTime = 1.1f;
				m_fShakePower = 0.06f;
			}

			if ((fZecallion_GetTrackPosition >= 4.65f && fZecallion_GetTrackPosition <= 4.7f))
			{
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				m_bIsZecallion_JumpThrow = true;
				m_fShakeTime = 0.2f;
				m_fShakePower = 0.08f;
			}
		}



		////////////////////////////////////////////////// 컷씬 /////////////////////////////////////////////////////////
		if (m_bIsCutSceneMode_InitZecallion == true && !m_bIsZecallion_StandUpFaceDown && !m_bIsZecallion_Spawn)
		{
			CTransform* pTransCom_Player = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform", 0);
			CTransform* pTransCom_Zecallion = (CTransform*)CObject_Manager::Get_Instance()->Get_Component(SCENE_STAGE, L"Layer_Zecallion", L"Com_Transform", 0);


			_vec3 vPlayer_Position, vPlayerToZec;

			if (pTransCom_Player != nullptr )
			{
				vPlayer_Position = *pTransCom_Player->Get_Infomation(CTransform::INFO_POSITION);
			}

			if (pTransCom_Zecallion != nullptr && m_bIsSettingCompleted_InitZecallionPos == false)
			{
				m_vZecallionPos = *pTransCom_Zecallion->Get_Infomation(CTransform::INFO_POSITION);
				m_vOriAtPos = m_CameraInfo.vAt;
				m_vOriEyePos = m_CameraInfo.vEye;
				vPlayer_Position = *pTransCom_Player->Get_Infomation(CTransform::INFO_POSITION);
				//m_bIsSettingCompleted_InitZecallionPos = true;
			}

			vPlayerToZec = vPlayer_Position - m_vZecallionPos;
			_float fPlayerToZec = D3DXVec3Length(&vPlayerToZec);

			m_CameraInfo.vEye = _vec3(m_vZecallionPos.x, vPlayer_Position.y + 5.f, m_vZecallionPos.z - fPlayerToZec * 0.3f);
			m_CameraInfo.vAt = m_vZecallionPos;
		}

		//  		system("cls");
		// 		cout << "m_CameraInfo.vAt.x : " << m_CameraInfo.vAt.x << endl;
		// 		cout << "m_CameraInfo.vAt.y : " << m_CameraInfo.vAt.y << endl;
		// 		cout << "m_iPreZecallionAniIdx : " << m_iPreZecallionAniIdx << endl;
		// 		cout << "m_iCurZecallionAniIdx : " << m_iCurZecallionAniIdx << endl;
		// 		cout << "m_bIsPermitted_Shaking : " << m_bIsPermitted_Shaking << endl;
				///////////////////////////////////////////// 쉐이킹 ON OFF //////////////////////////


				////////////////////////////////////////////// 쉐이킹 /////////////////////////////////// 
		if (m_bIsZecallion_StandUpFaceDown || m_bIsZecallion_Run || m_bIsZecallion_AttackCombo1 || m_bIsZecallion_DoubleMash || m_bIsZecallion_GaurdBegin
			|| m_bIsZecallion_JumpThrow || m_bIsZecallion_Spawn)
		{
			// 2번째. (Eye)
// 			_float fX = 0.1f; _float fY = 0.1f;
// 			_float fUpToX = 0.1f; _float fUpToY = 0.1f;
// 
// 			// bIsPlusX, bIsPlusY 분기.
// 			if (m_CameraInfo.vEye.x <= m_vOriEyePos.x - fUpToX)
// 			{
// 				m_bIsPlusX = true;
// 			}
// 			if (m_CameraInfo.vEye.x >= m_vOriEyePos.x + fUpToX)
// 			{
// 				m_bIsPlusX = false;
// 			}
// 			if (m_CameraInfo.vEye.y >= m_vOriEyePos.y + fUpToY)
// 			{
// 				m_bIsPlusY = false;
// 			}
// 			if (m_CameraInfo.vEye.y <= m_vOriEyePos.y - fUpToY)
// 			{
// 				m_bIsPlusY = true;
// 			}
// 
// 			// pos 변환.
// 			if (m_bIsPlusY && m_bIsPlusX)
// 			{
// 				//			m_CameraInfo.vEye = _vec3(m_vOriAtPos.x + fX, m_vOriAtPos.y + fY, m_vOriAtPos.z);
// 				m_CameraInfo.vEye = _vec3(m_CameraInfo.vEye.x + fX, m_CameraInfo.vEye.y + fY, m_CameraInfo.vEye.z);
// 			}
// 			else if (!m_bIsPlusY && m_bIsPlusX)
// 			{
// 				//				m_CameraInfo.vEye = _vec3(m_vOriAtPos.x + fX, m_vOriAtPos.y - fY, m_vOriAtPos.z);
// 				m_CameraInfo.vEye = _vec3(m_CameraInfo.vEye.x + fX, m_CameraInfo.vEye.y - fY, m_CameraInfo.vEye.z);
// 			}
// 			else if (!m_bIsPlusY && !m_bIsPlusX)
// 			{
// 				//				m_CameraInfo.vEye = _vec3(m_vOriAtPos.x - fX, m_vOriAtPos.y - fY, m_vOriAtPos.z);
// 				m_CameraInfo.vEye = _vec3(m_CameraInfo.vEye.x - fX, m_CameraInfo.vEye.y - fY, m_CameraInfo.vEye.z);
// 			}
// 			else if (m_bIsPlusY && !m_bIsPlusX)
// 			{
// 				//				m_CameraInfo.vEye = _vec3(m_vOriAtPos.x - fX, m_vOriAtPos.y + fY, m_vOriAtPos.z);
// 				m_CameraInfo.vEye = _vec3(m_CameraInfo.vEye.x - fX, m_CameraInfo.vEye.y + fY, m_CameraInfo.vEye.z);
// 			}

			// 1번째. (At)
 
 			// bIsPlusX, bIsPlusY 분기.
 			if (m_CameraInfo.vAt.x <= m_vOriAtPos.x - m_fShakePower)
 			{
 				m_bIsPlusX = true;
 			}
 			if (m_CameraInfo.vAt.x >= m_vOriAtPos.x + m_fShakePower)
 			{
 				m_bIsPlusX = false;
 			}
 			if (m_CameraInfo.vAt.y >= m_vOriAtPos.y + m_fShakePower)
 			{
 				m_bIsPlusY = false;
 			}
 			if (m_CameraInfo.vAt.y <= m_vOriAtPos.y - m_fShakePower)
 			{
 				m_bIsPlusY = true;
 			}
 
 			// pos 변환.
 			if (m_bIsPlusY && m_bIsPlusX)
 			{
 				m_CameraInfo.vAt = _vec3(m_CameraInfo.vAt.x + m_fShakePower, m_CameraInfo.vAt.y + m_fShakePower, m_CameraInfo.vAt.z);
 			}
 			else if (!m_bIsPlusY && m_bIsPlusX)
 			{
 				m_CameraInfo.vAt = _vec3(m_CameraInfo.vAt.x + m_fShakePower, m_CameraInfo.vAt.y - m_fShakePower, m_CameraInfo.vAt.z);
 			}
 			else if (!m_bIsPlusY && !m_bIsPlusX)
 			{
 				m_CameraInfo.vAt = _vec3(m_CameraInfo.vAt.x - m_fShakePower, m_CameraInfo.vAt.y - m_fShakePower, m_CameraInfo.vAt.z);
 			}
 			else if (m_bIsPlusY && !m_bIsPlusX)
 			{
 				m_CameraInfo.vAt = _vec3(m_CameraInfo.vAt.x - m_fShakePower, m_CameraInfo.vAt.y + m_fShakePower, m_CameraInfo.vAt.z);
 			}
		}
	}

	return CCamera::LastUpdate_GameObject(fTimeDelta);
}

void CStatic_Camera::Render_GameObject(void)
{
}

CStatic_Camera * CStatic_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, CAMERAINFO * pCamInfo, PROJINFO * pProjInfo)
{
	CStatic_Camera *	pInstance = new CStatic_Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pCamInfo, pProjInfo)))
	{
		MSG_BOX("CStatic_Camera Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CStatic_Camera::Free(void)
{
	Safe_Release(m_pInput_Device);

	return CCamera::Free();
}
