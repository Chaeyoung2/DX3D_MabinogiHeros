#include "..\Headers\Collision.h"
#include "Mesh.h"
#include "Transform.h"



USING(Engine)

CCollision::CCollision(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CCollision::CCollision(const CCollision & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
	, m_isColl(rhs.m_isColl)
	, m_pWorldMatrix(rhs.m_pWorldMatrix)
	, m_pWorldMatrix_NotRot(rhs.m_pWorldMatrix_NotRot)
{
	for (size_t i = 0; i < COL_END; ++i)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
}

HRESULT CCollision::Ready_Collision(void)
{
	for (size_t i = 0; i < COL_END; ++i)
	{
		if (FAILED(m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], nullptr)))
			return E_FAIL;
	}

	D3DLOCKED_RECT		LockRect;

	m_pTexture[COL_RED]->LockRect(0, &LockRect, nullptr, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pTexture[COL_RED]->UnlockRect(0);

	m_pTexture[COL_GREEN]->LockRect(0, &LockRect, nullptr, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.0f, 1.f, 0.f, 1.f);

	m_pTexture[COL_GREEN]->UnlockRect(0);

	return NOERROR;
}

HRESULT CCollision::SetUp_Collision(TYPE eType, CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex)
{
	m_eType = eType;

	m_vMin = *pMeshCom->Get_Min(iIndex);
	m_vMax = *pMeshCom->Get_Max(iIndex);

	m_pWorldMatrix = pTransformCom->Get_WorldMatrix();
	m_pWorldMatrix_NotRot = pTransformCom->Get_WorldMatrix_NotRot();

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXTEX) * 8, 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 앞면
	pVertex[0].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVertex[1].vTexUV = _vec2(0.f, 0.f);

	pVertex[2].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVertex[2].vTexUV = _vec2(0.f, 0.f);

	pVertex[3].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVertex[3].vTexUV = _vec2(0.f, 0.f);


	// 뒷면
	pVertex[4].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVertex[4].vTexUV = _vec2(0.f, 0.f);

	pVertex[5].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVertex[5].vTexUV = _vec2(0.f, 0.f);

	pVertex[6].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVertex[6].vTexUV = _vec2(0.f, 0.f);

	pVertex[7].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVertex[7].vTexUV = _vec2(0.f, 0.f);

	for (_uint i = 0; i < 8; ++i)
		m_OBBInfo.vPoint[i] = pVertex[i].vPosition;

	SetUp_Axis();

	m_pVB->Unlock();

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1; pIndex[0]._2 = 5; pIndex[0]._3 = 6;
	pIndex[1]._1 = 1; pIndex[1]._2 = 6; pIndex[1]._3 = 2;
	// -x
	pIndex[2]._1 = 4; pIndex[2]._2 = 0; pIndex[2]._3 = 3;
	pIndex[3]._1 = 4; pIndex[3]._2 = 3; pIndex[3]._3 = 7;
	// +y
	pIndex[4]._1 = 4; pIndex[4]._2 = 5; pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1; pIndex[5]._3 = 0;
	// -y
	pIndex[6]._1 = 3; pIndex[6]._2 = 2; pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6; pIndex[7]._3 = 7;
	// +z
	pIndex[8]._1 = 5; pIndex[8]._2 = 4; pIndex[8]._3 = 7;
	pIndex[9]._1 = 5; pIndex[9]._2 = 7; pIndex[9]._3 = 6;
	// -z
	pIndex[10]._1 = 0; pIndex[10]._2 = 1; pIndex[10]._3 = 2;
	pIndex[11]._1 = 0; pIndex[11]._2 = 2; pIndex[11]._3 = 3;

	m_pIB->Unlock();




	return NOERROR;
}

HRESULT CCollision::SetUp_Collision_Ex(TYPE eType, CMesh * pMeshCom, CTransform * pTransformCom, const _uint & iIndex, _vec3& vScale, _vec3& vTrans)
{
	m_eType = eType;

	m_vMin = (*pMeshCom->Get_Min(iIndex));
	m_vMax = (*pMeshCom->Get_Max(iIndex));

	// 스케일 값 적용.
	m_vMin.x *= vScale.x; 	m_vMin.y *= vScale.y; 	m_vMin.z *= vScale.z;
	m_vMax.x *= vScale.x; 	m_vMax.y *= vScale.y; 	m_vMax.z *= vScale.z;

	// 이동 값 적용.
	m_vMin.x += vTrans.x; 	m_vMin.y += vTrans.y; 	m_vMin.z += vTrans.z;
	m_vMax.x += vTrans.x; 	m_vMax.y += vTrans.y; 	m_vMax.z += vTrans.z;


	m_pWorldMatrix = pTransformCom->Get_WorldMatrix();
	m_pWorldMatrix_NotRot = pTransformCom->Get_WorldMatrix_NotRot();

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXTEX) * 8, 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 앞면
	pVertex[0].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVertex[1].vTexUV = _vec2(0.f, 0.f);

	pVertex[2].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVertex[2].vTexUV = _vec2(0.f, 0.f);

	pVertex[3].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVertex[3].vTexUV = _vec2(0.f, 0.f);


	// 뒷면
	pVertex[4].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVertex[4].vTexUV = _vec2(0.f, 0.f);

	pVertex[5].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVertex[5].vTexUV = _vec2(0.f, 0.f);

	pVertex[6].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVertex[6].vTexUV = _vec2(0.f, 0.f);

	pVertex[7].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVertex[7].vTexUV = _vec2(0.f, 0.f);

	for (_uint i = 0; i < 8; ++i)
		m_OBBInfo.vPoint[i] = pVertex[i].vPosition;

	SetUp_Axis();

	m_pVB->Unlock();

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1; pIndex[0]._2 = 5; pIndex[0]._3 = 6;
	pIndex[1]._1 = 1; pIndex[1]._2 = 6; pIndex[1]._3 = 2;
	// -x
	pIndex[2]._1 = 4; pIndex[2]._2 = 0; pIndex[2]._3 = 3;
	pIndex[3]._1 = 4; pIndex[3]._2 = 3; pIndex[3]._3 = 7;
	// +y
	pIndex[4]._1 = 4; pIndex[4]._2 = 5; pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1; pIndex[5]._3 = 0;
	// -y
	pIndex[6]._1 = 3; pIndex[6]._2 = 2; pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6; pIndex[7]._3 = 7;
	// +z
	pIndex[8]._1 = 5; pIndex[8]._2 = 4; pIndex[8]._3 = 7;
	pIndex[9]._1 = 5; pIndex[9]._2 = 7; pIndex[9]._3 = 6;
	// -z
	pIndex[10]._1 = 0; pIndex[10]._2 = 1; pIndex[10]._3 = 2;
	pIndex[11]._1 = 0; pIndex[11]._2 = 2; pIndex[11]._3 = 3;

	m_pIB->Unlock();




	return NOERROR;
}

HRESULT CCollision::SetUp_Collision_Sphere(TYPE eType, CMesh * pMeshCom, CTransform * pTransformCom, const _uint & iIndex, SPHEREINFO pSphereInfo)
{
	m_eType = eType;
	m_SphereInfo = pSphereInfo;

	m_pWorldMatrix = pTransformCom->Get_WorldMatrix();
	m_pWorldMatrix_NotRot = pTransformCom->Get_WorldMatrix_NotRot();

	D3DXCreateSphere(m_pGraphicDev, m_SphereInfo.fRadius, m_SphereInfo.iSlices, m_SphereInfo.iStacks, &m_pSphereMesh, &m_pAdjacency);



	return NOERROR;
}

_bool CCollision::Collision_AABB(CCollision* pTargetCollision)
{
	_vec3			vSrcMin, vSrcMax, vDstMin, vDstMax;

	D3DXVec3TransformCoord(&vSrcMin, &m_vMin, m_pWorldMatrix_NotRot);
	D3DXVec3TransformCoord(&vSrcMax, &m_vMax, m_pWorldMatrix_NotRot);

	D3DXVec3TransformCoord(&vDstMin, pTargetCollision->Get_Min(), pTargetCollision->Get_WorldMatrix_NotRot());
	D3DXVec3TransformCoord(&vDstMax, pTargetCollision->Get_Max(), pTargetCollision->Get_WorldMatrix_NotRot());

	// x축 선상에서 바라봤을때 겹치는지에 대한 비교
	if (max(vSrcMin.x, vDstMin.x) > min(vSrcMax.x, vDstMax.x))
	{
		m_isColl = false;
		return false;
	}

	// y축 선상에서 바라봤을때 겹치는지에 대한 비교
	if (max(vSrcMin.y, vDstMin.y) > min(vSrcMax.y, vDstMax.y)) 
	{
		m_isColl = false;
		return false;
	}

	// z축 선상에서 바라봤을때 겹치는지에 대한 비교
	if (max(vSrcMin.z, vDstMin.z) > min(vSrcMax.z, vDstMax.z))
	{
		m_isColl = false;
		return false;
	}

	m_isColl = true;
	return true;
}

_bool CCollision::Collision_OBB(CCollision * pTargetCollsion)
{
	// 큐브의 중심에서 면의 중심을 바라보는 벡터(내꺼)
	_vec3		vSrcProjDir[3];
	_vec3		vSrcAlignAxis[3];
	_vec3		vSrcCenterPos;


	OBBINFO TargetOBBInfo = pTargetCollsion->Get_OBBInfo();

	// 큐브의 중심에서 면의 중심을 바라보는 벡터(니꺼)
	_vec3		vDestProjDir[3];
	_vec3		vDestAlignAxis[3];
	_vec3		vDestCenterPos;

	// 로커레잉ㅆ는 벡터를 월드롤 옮기자.
	for (size_t i = 0; i < 3; ++i)
	{
		// soRj
		D3DXVec3TransformNormal(&vSrcProjDir[i], &m_OBBInfo.vProjDir[i], m_pWorldMatrix);
		D3DXVec3TransformNormal(&vSrcAlignAxis[i], &m_OBBInfo.vAlignAxis[i], m_pWorldMatrix);
		D3DXVec3Normalize(&vSrcAlignAxis[i], &vSrcAlignAxis[i]);

		// 니꺼
		D3DXVec3TransformNormal(&vDestProjDir[i], &TargetOBBInfo.vProjDir[i], pTargetCollsion->Get_WorldMatrix());
		D3DXVec3TransformNormal(&vDestAlignAxis[i], &TargetOBBInfo.vAlignAxis[i], pTargetCollsion->Get_WorldMatrix());
		D3DXVec3Normalize(&vDestAlignAxis[i], &vDestAlignAxis[i]);
	}

	D3DXVec3TransformCoord(&vSrcCenterPos, &m_OBBInfo.vCenterPos, m_pWorldMatrix);
	D3DXVec3TransformCoord(&vDestCenterPos, &TargetOBBInfo.vCenterPos, pTargetCollsion->Get_WorldMatrix());

	_float		fDistance[3] = { 0.0f };


	for (size_t j = 0; j < 3; j++)
	{
		fDistance[0] = fabs(D3DXVec3Dot(&vSrcProjDir[0], &vSrcAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vSrcProjDir[1], &vSrcAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vSrcProjDir[2], &vSrcAlignAxis[j]));

		fDistance[1] = fabs(D3DXVec3Dot(&vDestProjDir[0], &vSrcAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vDestProjDir[1], &vSrcAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vDestProjDir[2], &vSrcAlignAxis[j]));

		_vec3		vCenterDir = vDestCenterPos - vSrcCenterPos;
		fDistance[2] = fabs(D3DXVec3Dot(&vCenterDir, &vSrcAlignAxis[j]));

		if (fDistance[0] + fDistance[1] < fDistance[2])
		{
			m_isColl = false;
			return false;
		}
	}

	for (size_t j = 0; j < 3; j++)
	{
		fDistance[0] = fabs(D3DXVec3Dot(&vSrcProjDir[0], &vDestAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vSrcProjDir[1], &vDestAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vSrcProjDir[2], &vDestAlignAxis[j]));

		fDistance[1] = fabs(D3DXVec3Dot(&vDestProjDir[0], &vDestAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vDestProjDir[1], &vDestAlignAxis[j]))
			+ fabs(D3DXVec3Dot(&vDestProjDir[2], &vDestAlignAxis[j]));

		_vec3		vCenterDir = vDestCenterPos - vSrcCenterPos;
		fDistance[2] = fabs(D3DXVec3Dot(&vCenterDir, &vDestAlignAxis[j]));

		if (fDistance[0] + fDistance[1] < fDistance[2])
		{
			m_isColl = false;
			return false;
		}
	}



	m_isColl = true;

	return true;
}

_bool CCollision::Collision_Sphere(CCollision * pTargetCollision)
{
	_vec3 vTargetPos = pTargetCollision->Get_SPHEREInfo().vCenterPos;
	_vec3 vMyPos = m_SphereInfo.vCenterPos;
	D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, pTargetCollision->Get_WorldMatrix());
	D3DXVec3TransformCoord(&vMyPos, &vMyPos, m_pWorldMatrix);

	// 중심 - 중심간의 벡터.
	_vec3 vGap = vTargetPos - vMyPos;

	_float fGap = D3DXVec3Length(&vGap);


	// 중심 - 호까지의 벡터. (반지름)
	_vec3 vTarget_PosToArc = _vec3(vTargetPos.x + pTargetCollision->Get_SPHEREInfo().fRadius * 0.5f, vTargetPos.y, vTargetPos.z);
	_vec3 vMy_PosToArc = _vec3(m_SphereInfo.vCenterPos.x + m_SphereInfo.fRadius * 0.5f, m_SphereInfo.vCenterPos.y, m_SphereInfo.vCenterPos.z);

	//_float fRadSum = pTargetCollision->Get_SPHEREInfo().fRadius * 0.5f + m_SphereInfo.fRadius * 0.5f;

	// 반지름 벡터 * 월드행렬
	D3DXVec3TransformNormal(&m_SphereInfo.vPosToArc, &vMy_PosToArc, m_pWorldMatrix);
	D3DXVec3TransformNormal(&vTarget_PosToArc, &vTarget_PosToArc, pTargetCollision->Get_WorldMatrix());

	//_float fRadSum = D3DXVec3Length(&m_SphereInfo.vPosToArc) + D3DXVec3Length(&vTarget_PosToArc);
	_float fRadSum = m_SphereInfo.vPosToArc.x + vTarget_PosToArc.x;

	// 반지름 벡터 합.

	if (fGap <= fRadSum)
	{
		m_isColl = true;
		return true;
	}
	else
	{
		m_isColl = false;
		return false;
	}
}

void CCollision::Render_Collider(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// 부모 행렬 세팅.
/*	m_pTransCom->Set_Parent((*m_pBoneMatrix_Parent) * (*m_pWorldMatrix_Parent));*/

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (true == m_isColl)
		m_pGraphicDev->SetTexture(0, m_pTexture[COL_RED]);
	if (false == m_isColl)
		m_pGraphicDev->SetTexture(0, m_pTexture[COL_GREEN]);

	if (TYPE_AABB == m_eType)
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pWorldMatrix_NotRot);
	if (TYPE_OBB == m_eType)
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pWorldMatrix);


	// 장치가 사용해야할 정점버퍼를 장치에게 셋팅한다.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));

	m_pGraphicDev->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	if(m_pSphereMesh != nullptr)
		m_pSphereMesh->DrawSubset(0);


	///////////////
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CCollision::Update_SpherePos(_vec3 vPosition)
{
	m_SphereInfo.vCenterPos = vPosition;
}

void CCollision::SetUp_Axis(void)
{
	m_OBBInfo.vCenterPos = (m_OBBInfo.vPoint[0] + m_OBBInfo.vPoint[6]) * 0.5f;

	// obb으;ㅣ 중심에서 각 면의 중점을 바라보는 방향벡터를 구했다.
	// +ㅌ
	m_OBBInfo.vProjDir[0] = (m_OBBInfo.vPoint[1] + m_OBBInfo.vPoint[6]) * 0.5f - m_OBBInfo.vCenterPos;
	// +ㅛ
	m_OBBInfo.vProjDir[1] = (m_OBBInfo.vPoint[0] + m_OBBInfo.vPoint[5]) * 0.5f - m_OBBInfo.vCenterPos;
	// +ㅋ
	m_OBBInfo.vProjDir[2] = (m_OBBInfo.vPoint[7] + m_OBBInfo.vPoint[5]) * 0.5f - m_OBBInfo.vCenterPos;

	// obb를 이루믄 면에 평행한 방향벡터를 구한다.
	m_OBBInfo.vAlignAxis[0] = m_OBBInfo.vPoint[2] - m_OBBInfo.vPoint[3];
	m_OBBInfo.vAlignAxis[1] = m_OBBInfo.vPoint[0] - m_OBBInfo.vPoint[3];
	m_OBBInfo.vAlignAxis[2] = m_OBBInfo.vPoint[7] - m_OBBInfo.vPoint[3];
}

CComponent * CCollision::Clone(void)
{
	return new CCollision(*this);
}

CCollision * CCollision::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollision *	pInstance = new CCollision(pGraphicDev);

	if (FAILED(pInstance->Ready_Collision()))
	{
		MSG_BOX("CCollision Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CCollision::Free(void)
{
	for (size_t i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pSphereMesh);

	return CComponent::Free();
}
