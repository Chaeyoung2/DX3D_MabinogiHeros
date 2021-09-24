#include "..\Headers\Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_ProjInfo.fFovY = D3DXToRadian(60.0f);

	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);
	
	m_ProjInfo.fAspect = _float(ViewPort.Width) / _float(ViewPort.Height);
	m_ProjInfo.fNear = 0.2f;
	m_ProjInfo.fFar = 1000.f;

}


HRESULT CCamera::Ready_GameObject(void)
{
	Matrix_LookAtLH(&m_matView, &m_CameraInfo.vEye, &m_CameraInfo.vAt, &m_CameraInfo.vAxisY);

	Matrix_PerspectiveFovLH(&m_matProj, m_ProjInfo.fFovY, m_ProjInfo.fAspect, m_ProjInfo.fNear, m_ProjInfo.fFar);

	

	return NOERROR;
}

_int CCamera::Update_GameObject(const _float & fTimeDelta)
{
	D3DXMatrixInverse(&m_matWorld, nullptr, &m_matView);

	return _int();
}

_int CCamera::LastUpdate_GameObject(const _float & fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_CameraInfo.vEye, &m_CameraInfo.vAt, &m_CameraInfo.vAxisY);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_ProjInfo.fFovY, m_ProjInfo.fAspect, m_ProjInfo.fNear, m_ProjInfo.fFar);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return _int();
}

_matrix * CCamera::Matrix_LookAtLH(_matrix * pOutMatrix, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pAxisY)
{
	D3DXMatrixIdentity(pOutMatrix);

	// 카메라의 월드행렬을 구한다.
	_vec3		vRight, vUp, vLook, vPos;
	vLook = m_CameraInfo.vAt - m_CameraInfo.vEye;
	D3DXVec3Cross(&vRight, pAxisY, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	vPos = m_CameraInfo.vEye;

	memcpy(&pOutMatrix->m[0][0], D3DXVec3Normalize(&vRight, &vRight), sizeof(_vec3));
	memcpy(&pOutMatrix->m[1][0], D3DXVec3Normalize(&vUp, &vUp), sizeof(_vec3));
	memcpy(&pOutMatrix->m[2][0], D3DXVec3Normalize(&vLook, &vLook), sizeof(_vec3));
	memcpy(&pOutMatrix->m[3][0], &vPos, sizeof(_vec3));
	
	// 이 월드행렬의 역행렬을 구한다.
	D3DXMatrixInverse(pOutMatrix, nullptr, pOutMatrix);

	return pOutMatrix;
}

_matrix * CCamera::Matrix_PerspectiveFovLH(_matrix * pOutMatrix, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	D3DXMatrixIdentity(pOutMatrix);

	pOutMatrix->_11 = (1.f / tanf(fFovY * 0.5f)) / fAspect;
	pOutMatrix->_22 = (1.f / tanf(fFovY * 0.5f));
	pOutMatrix->_33 = fFar / (fFar - fNear);
	pOutMatrix->_43 = (fFar * fNear) / (fFar - fNear) * -1.f;
	pOutMatrix->_34 = 1.f;
	pOutMatrix->_44 = 0.f;

	return pOutMatrix;
}

_ulong CCamera::Free(void)
{
	return CGameObject::Free();
}
