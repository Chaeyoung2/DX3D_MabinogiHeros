 #include "..\Headers\Collision_Sphere.h"
 #include "Mesh.h"
 #include "Transform.h"
 
 USING(Engine)
 
 CCollision_Sphere::CCollision_Sphere(LPDIRECT3DDEVICE9 pGraphicDev)
 	: CComponent(pGraphicDev)
 {
 
 }
 
 CCollision_Sphere::CCollision_Sphere(const CCollision_Sphere & rhs)
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
 
 HRESULT CCollision_Sphere::Ready_Collision(void)
 {
 	// D3DXCreateSphere(m_pGraphicDev, m_fRadius, 20, 20, &m_pBoundingSphere, NULL);
 
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
 
 HRESULT CCollision_Sphere::SetUp_Collision(CMesh* pMeshCom, CTransform* pTransformCom, const _uint& iIndex)
 {
 
 	m_vMin = *pMeshCom->Get_Min(iIndex);
 	m_vMax = *pMeshCom->Get_Max(iIndex);
 
 	m_fRadius = 5.f;
 
 	D3DXCreateSphere(m_pGraphicDev, m_fRadius, 20, 20, &m_pBoundingSphere, NULL);
 
 	VTXTEX* pVertex = nullptr;
 	
 	pVertex[0].vPosition.x = 0.f;
 	pVertex[0].vPosition.y = m_fRadius;
 	pVertex[0].vPosition.z = 0.0f;
	pVertex[0].vTexUV = _vec2(0.f, 0.f);
 
 	_int iStack = 20;
 	_int iSlices = 20;
	_int iNumVertex = (iSlices + 1) * 2 + ((iStack - 3) * iSlices);
	_int iNumFace = (iSlices * 2) + ((iStack - 2) * 2) * iSlices;

 	for (int i = 0 + 1, k = 0; i < iStack; ++i)
 	{
 		_vec3 temp(0.f, m_fRadius * cosf(D3DX_PI / iStack) * i, m_fRadius * sinf((D3DX_PI / iStack)));
 
 		for (int j = 0; j < iSlices; ++j, ++k)
 		{
 			_float fTemp = m_fRadius - (m_fRadius - temp.z);
 			pVertex[k].vPosition.x = fTemp * cosf(D3DXToRadian(360 / iSlices) * j);
 			pVertex[k].vPosition.y = temp.y;
 			pVertex[k].vPosition.z = fTemp * sinf(D3DXToRadian(360 / iSlices) * j);
			pVertex[k].vTexUV = _vec2(0.f, 0.f);
 		}
 	}

	pVertex[iNumVertex - 1].vPosition.x = 0.f;
	pVertex[iNumVertex - 1].vPosition.y = -m_fRadius;
	pVertex[iNumVertex - 1].vTexUV = _vec2(0.f, 0.f);

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(iNumVertex * sizeof(VTXTEX), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;
 
	void* pVertices;
	m_pVB->Lock(0, sizeof(pVertices), (void**)&pVertices, 0);
	memcpy(pVertices, pVertex, sizeof(VTXTEX) * iNumVertex);
	m_pVB->Unlock();

	int j = 0, z = 0;

	INDEX16*			pIndex = nullptr;

	for (z = 0; z < iSlices; ++z)
	{
		pIndex[z]._1 = 0;
		pIndex[z]._2 = z + 1;
		pIndex[z]._3 = z + 2 - j;

		if (z == iSlices - 2)
		{
			j = iSlices;
		}
	}

	int t = z;
	int tmp = 0;
	int tmp2 = 0;
	int a = 0;

	for (int i = 0; i < iStack - 2; ++i)
	{
		tmp = (iSlices + 1) + (i * iSlices);
		tmp2 = 2 + (i * iSlices);
		for (int j = 0; j < iSlices; ++j)
		{
			if (j == (iSlices - 1))
				a = iSlices;
			pIndex[t]._1 = tmp2 + j - a;
			pIndex[t]._2 = tmp2 + j - 1;
			pIndex[t]._3 = tmp2 + j + iSlices - a;

			pIndex[t + 1]._1 = tmp + j + 1 - a;
			pIndex[t + 1]._2 = tmp + j - (iSlices);
			pIndex[t + 1]._3 = tmp + j;
			t += 2;
		}
		a = 0;
	}
	int b;
	b = t;

	int q = 0;
	for (int i = 0; i < iSlices; ++i)
	{
		pIndex[b]._1 = tmp + i;
		pIndex[b]._2 = tmp + iSlices;
		pIndex[b]._3 = tmp + i + 1 - q;
		if (i == iSlices - 2)
			q = iSlices - i;
		b++;
	}

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * iNumFace, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;


	INDEX16*			pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	memcpy(pIndices, pIndex, sizeof(INDEX16) * iNumFace);
	m_pVB->Unlock();

 
 	return NOERROR;
 }
 
 
 _bool CCollision_Sphere::Collision_SPHERE(CCollision_Sphere * pTargetCollsion)
 {
 	return _bool();
 }
 
 void CCollision_Sphere::Render_Collider(void)
 {
 	if (nullptr == m_pGraphicDev)
 		return;
 
 	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
 	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
 
 	if (true == m_isColl)
 		m_pGraphicDev->SetTexture(0, m_pTexture[COL_RED]);
 	if (false == m_isColl)
 		m_pGraphicDev->SetTexture(0, m_pTexture[COL_GREEN]);
 
 	//if (TYPE_AABB == m_eType)
 	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pWorldMatrix_NotRot);
 	//if (TYPE_OBB == m_eType)
 	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pWorldMatrix);
 
 
 	// 장치가 사용해야할 정점버퍼를 장치에게 셋팅한다.
 	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
 
 	m_pGraphicDev->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
 
 	m_pGraphicDev->SetIndices(m_pIB);
 
 	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
 
 	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
 	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
 }
 
 void CCollision_Sphere::SetUp_Axis(void)
 {
 
 }
 
 CComponent * CCollision_Sphere::Clone(void)
 {
 	return new CCollision_Sphere(*this);
 }
 
 CCollision_Sphere * CCollision_Sphere::Create(LPDIRECT3DDEVICE9 pGraphicDev)
 {
 	CCollision_Sphere *	pInstance = new CCollision_Sphere(pGraphicDev);
 
 	if (FAILED(pInstance->Ready_Collision()))
 	{
 		MSG_BOX("CCollision_Sphere Created Failed");
 		Safe_Release(pInstance);
 	}
 	return pInstance;
 }
 
 _ulong CCollision_Sphere::Free(void)
 {
 	for (size_t i = 0; i < COL_END; ++i)
 		Safe_Release(m_pTexture[i]);
 
 	Safe_Release(m_pVB);
 	Safe_Release(m_pIB);
 
 	return CComponent::Free();
 }
