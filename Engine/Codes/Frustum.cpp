#include "..\Headers\Frustum.h"

USING(Engine)

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CFrustum::CFrustum(const CFrustum & rhs)
	: CComponent(rhs)
{
	memcpy(m_vPoint, rhs.m_vPoint, sizeof(_vec3) * 8);

}

HRESULT CFrustum::Ready_Frustum(void)
{
	// 투영영역상에 존재하는 절두체의 정보를 구성했다.

	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return NOERROR;
}

_bool CFrustum::Culling_toFrustum(const _vec3 * pPositionInWorld, const _float & fRadius)
{
	if (nullptr == m_pGraphicDev)
		return false;

	_vec3		vPoint[8];

	memcpy(vPoint, m_vPoint, sizeof(_vec3) * 8);

	// 투영스페이스에 존재하는 정점들을 월드 스페이스로 옮기자
	_matrix			matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	// 월드스페이스로로 옮기자.
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
	}

	// 평면을 구성하자.
	D3DXPLANE			Plane[6];

	// +x
	D3DXPlaneFromPoints(&Plane[0], &vPoint[1], &vPoint[5], &vPoint[6]);
	// -x
	D3DXPlaneFromPoints(&Plane[1], &vPoint[4], &vPoint[0], &vPoint[3]);

	// +y
	D3DXPlaneFromPoints(&Plane[2], &vPoint[4], &vPoint[5], &vPoint[1]);
	// -y
	D3DXPlaneFromPoints(&Plane[3], &vPoint[3], &vPoint[2], &vPoint[6]);

	// +z
	D3DXPlaneFromPoints(&Plane[4], &vPoint[5], &vPoint[4], &vPoint[7]);
	// -z
	D3DXPlaneFromPoints(&Plane[5], &vPoint[0], &vPoint[1], &vPoint[2]);

	// a b c d
	// x y z




	return isIn_Frustum(Plane, pPositionInWorld, fRadius);
}

_bool CFrustum::isIn_Frustum(const D3DXPLANE* pPlane, const _vec3 * pPositionInWorld, const _float& fRadius)
{
	// ax + by + cz + d == 0 -> 평면상
	// ax + by + cz + d > 0 -> 평면의 앞쪽
	// ax + by + cz + d < 0 -> 평면의 뒤쪽
	for (size_t i = 0; i < 6; ++i)
	{
		_float			fDistance = D3DXPlaneDotCoord(&pPlane[i], pPositionInWorld); // 점과 평면사이의 거리(수직인거리)

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

CFrustum * CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum *	pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
	{
		MSG_BOX("CFrustum Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CFrustum::Clone(void)
{
	return new CFrustum(*this);
}

_ulong CFrustum::Free(void)
{
	CComponent::Free();

	return _ulong();
}
