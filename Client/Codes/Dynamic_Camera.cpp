#include "stdafx.h"
#include "..\Headers\Dynamic_Camera.h"

CDynamic_Camera::CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_pInput_Device(CInput_Device::Get_Instance())
{
	m_pInput_Device->AddRef();
}

HRESULT CDynamic_Camera::Ready_GameObject(CAMERAINFO * pCamInfo, PROJINFO * pProjInfo)
{
	m_fSpeed = 20.f;

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

_int CDynamic_Camera::Update_GameObject(const _float & fTimeDelta)
{
	if (g_bUsingStaticCamera == true)
		return _int();

	if (nullptr == m_pInput_Device)
		return -1;

	POINT		ptCenter = { g_iBackCX >> 1, g_iBackCY >> 1 };

	ClientToScreen(g_hWnd, &ptCenter);

	SetCursorPos(ptCenter.x, ptCenter.y);

	ShowCursor(false);

	CCamera::Update_GameObject(fTimeDelta);	


	if (m_pInput_Device->Get_DIKState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

		m_CameraInfo.vEye += *D3DXVec3Normalize(&vRight, &vRight) * -m_fSpeed * fTimeDelta;
		m_CameraInfo.vAt += *D3DXVec3Normalize(&vRight, &vRight) * -m_fSpeed * fTimeDelta;

	}

	if (m_pInput_Device->Get_DIKState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

		m_CameraInfo.vEye += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;
		m_CameraInfo.vAt += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

	}

	if (m_pInput_Device->Get_DIKState(DIK_W) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(_vec3));

		m_CameraInfo.vEye += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;
		m_CameraInfo.vAt += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

	}

	if (m_pInput_Device->Get_DIKState(DIK_S) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(_vec3));

		m_CameraInfo.vEye += *D3DXVec3Normalize(&vLook, &vLook) * -m_fSpeed * fTimeDelta;
		m_CameraInfo.vAt += *D3DXVec3Normalize(&vLook, &vLook) * -m_fSpeed * fTimeDelta;
	}

	// 가로움직임
	_long	MouseMove = 0;

	if (MouseMove = m_pInput_Device->Get_DIMMoveState(CInput_Device::DIMMS_X))
	{
		_matrix			matRot;
		D3DXMatrixRotationY(&matRot, (MouseMove / 10.f) * fTimeDelta);

		_vec3			vLook = m_CameraInfo.vAt - m_CameraInfo.vEye;
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		
		m_CameraInfo.vAt = m_CameraInfo.vEye + vLook;
	}

	if (MouseMove = m_pInput_Device->Get_DIMMoveState(CInput_Device::DIMMS_Y))
	{
		_matrix			matRot;

		_vec3			vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

		//D3DXMatrixRotationAxis 임의의 축을기준으로 회전한다.
		D3DXMatrixRotationAxis(&matRot, &vRight, (MouseMove / 10.f) * fTimeDelta);

		_vec3			vLook = m_CameraInfo.vAt - m_CameraInfo.vEye;
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraInfo.vAt = m_CameraInfo.vEye + vLook;
	}
	


	return _int();
}

_int CDynamic_Camera::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (g_bUsingStaticCamera == true)
		return _int();

	return CCamera::LastUpdate_GameObject(fTimeDelta);
}

void CDynamic_Camera::Render_GameObject(void)
{
}

CDynamic_Camera * CDynamic_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, CAMERAINFO * pCamInfo, PROJINFO * pProjInfo)
{
	CDynamic_Camera *	pInstance = new CDynamic_Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pCamInfo, pProjInfo)))
	{
		MSG_BOX("CDynamic_Camera Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}


_ulong CDynamic_Camera::Free(void)
{
	Safe_Release(m_pInput_Device);

	return CCamera::Free();
}
