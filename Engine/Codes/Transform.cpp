#include "..\Headers\Transform.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_matParent(rhs.m_matWorld)
	, m_matWorld_Parent(rhs.m_matWorld_Parent)
	, m_matWorld_NotRot_Parent(rhs.m_matWorld_NotRot_Parent)
	//, m_matParent(rhs.m_matWorld)
{
}


_float CTransform::Get_AngleX(void)
{
	_vec3		vLook;

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(_vec3));

	_vec3		vAxis(0.f, 0.f, 1.f);

	_float		fCos_Angle = D3DXVec3Dot(D3DXVec3Normalize(&vLook, &vLook), &vAxis);

	_float		fAngle = acosf(fCos_Angle);

	return fAngle;
}

_float CTransform::Get_AngleY(void)
{
	_vec3		vLook;

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(_vec3));

	_vec3		vAxis(0.f, 0.f, 1.f);

	_float		fCos_Angle = D3DXVec3Dot(D3DXVec3Normalize(&vLook, &vLook), &vAxis);

	_float		fAngle = acosf(fCos_Angle);

	return fAngle;
}

_vec3 CTransform::Get_ScaleInfo(void)
{
	return _vec3(D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_RIGHT][0])
		, D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_UP][0])
		, D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_LOOK][0]));
}

void CTransform::SetUp_OnGraphicDev(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_matWorld_Parent = m_matWorld * m_matParent;
	m_matWorld_NotRot_Parent = m_matWorld_NotRot * m_matParent;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld_Parent);

	Make_WorldMatrix_NotRot();
}

void CTransform::Invalidate_Worldmatrix(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_matWorld_Parent = m_matWorld * m_matParent;
	m_matWorld_NotRot_Parent = m_matWorld_NotRot * m_matParent;

	Make_WorldMatrix_NotRot();
}

void CTransform::SetUpHeight_OnBuffer(const _vec3 * pVertexPos, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwInterval)
{
	_vec3		vPosition;
	memcpy(&vPosition, &m_matWorld.m[3][0], sizeof(_vec3));

	// 현재 플레이어가 존재하는 타일의 인덱스
	_ulong		dwCurrentIndex = (_ulong(vPosition.z) / dwInterval) * dwCntX + (_ulong(vPosition.x) / dwInterval);

	if (dwCurrentIndex > dwCntX * dwCntZ)
		return;

	_float		fHeight[4] = {
		pVertexPos[dwCurrentIndex + dwCntX].y ,
		pVertexPos[dwCurrentIndex + dwCntX + 1].y ,
		pVertexPos[dwCurrentIndex + 1].y ,
		pVertexPos[dwCurrentIndex].y };

	_float		fRatioX = (vPosition.x - pVertexPos[dwCurrentIndex + dwCntX].x) / dwInterval;
	_float		fRatioZ = (pVertexPos[dwCurrentIndex + dwCntX + 1].z - vPosition.z) / dwInterval;

	if (fRatioX > fRatioZ) // 우측상단
	{
		m_matWorld.m[3][1] = fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	else // 좌측하단
	{
		m_matWorld.m[3][1] = fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}

}

void CTransform::Scaling(_vec3 * pScale /*x, y, z*/)
{
	_vec3		vTmp;

	for (size_t i = 0; i < INFO_END - 1; ++i)
	{
		memcpy(&vTmp, &m_matWorld.m[i][0], sizeof(_vec3));
		vTmp *= *(((_float*)pScale) + i);
		memcpy(&m_matWorld.m[i][0], &vTmp, sizeof(_vec3));
	}
}

void CTransform::RotationX(_float fRadian)
{
	_matrix			matRot;
	D3DXMatrixRotationX(&matRot, fRadian);


	_vec3			vUp = _vec3(0.f, 1.f, 0.f) * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_UP][0]);
	_vec3			vLook = _vec3(0.f, 0.f, 1.f) * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_LOOK][0]);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[INFO_UP][0], &vUp, &matRot);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[INFO_LOOK][0], &vLook, &matRot);
}

void CTransform::RotationY(_float fRadian)
{
	_matrix			matRot;
	D3DXMatrixRotationY(&matRot, fRadian);

	_vec3			vRight = _vec3(1.f, 0.f, 0.f) * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_RIGHT][0]);
	_vec3			vLook = _vec3(0.f, 0.f, 1.f) * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_LOOK][0]);



	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[INFO_RIGHT][0], &vRight, &matRot);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[INFO_LOOK][0], &vLook, &matRot);
}

void CTransform::RotationZ(_float fRadian)
{
	_matrix			matRot;
	D3DXMatrixRotationZ(&matRot, fRadian);

	_vec3			vRight = _vec3(1.f, 0.f, 0.f) * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_RIGHT][0]);
	_vec3			vUp = _vec3(0.f, 1.f, 0.f) * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_UP][0]);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[INFO_RIGHT][0], &vRight, &matRot);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[INFO_UP][0], &vUp, &matRot);
}

void CTransform::Go_Staight(_float fDistanceToFrame)
{
	_vec3			vLook, vPos;

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(_vec3));
	memcpy(&vPos, &m_matWorld.m[INFO_POSITION][0], sizeof(_vec3));

	vPos += *D3DXVec3Normalize(&vLook, &vLook) * fDistanceToFrame;

	memcpy(&m_matWorld.m[INFO_POSITION][0], &vPos, sizeof(_vec3));
}



HRESULT CTransform::Ready_Component(void)
{
	m_matParent = *D3DXMatrixIdentity(&m_matWorld);

	return NOERROR;
}

void CTransform::Make_WorldMatrix_NotRot(void)
{
	_vec3	vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	m_matWorld_NotRot = m_matWorld;

	_vec3	vTmp = vRight * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_RIGHT][0]);
	memcpy(&m_matWorld_NotRot.m[INFO_RIGHT][0], &vTmp, sizeof(_vec3));

	vTmp = vUp * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_UP][0]);
	memcpy(&m_matWorld_NotRot.m[INFO_UP][0], &vTmp, sizeof(_vec3));

	vTmp = vLook * D3DXVec3Length((_vec3*)&m_matWorld.m[INFO_LOOK][0]);
	memcpy(&m_matWorld_NotRot.m[INFO_LOOK][0], &vTmp, sizeof(_vec3));
}

CComponent * CTransform::Clone(void)
{
	return new CTransform(*this);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform *	pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CTransform Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;

}
_ulong CTransform::Free(void)
{
	return CComponent::Free();
}
