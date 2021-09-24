#include "..\Headers\Terrain_Buffer.h"
#include "Frustum.h"

USING(Engine)

CTerrain_Buffer::CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
	
}

CTerrain_Buffer::CTerrain_Buffer(const CTerrain_Buffer & rhs)
	: CVIBuffer(rhs)
	, m_pIndices(rhs.m_pIndices)
{

}


HRESULT CTerrain_Buffer::Ready_Component(const _tchar* pHeightmap, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ, const _ulong& dwInterval, vector<TERRAIN_DATA_SOLE>* pVecTerrainDataSole)
{
	if (pVecTerrainDataSole == nullptr)
	{
		m_dwVtxCnt = dwVtxCntX * dwVtxCntZ;
		m_dwCntX = dwVtxCntX;
		m_dwCntZ = dwVtxCntZ;
		m_dwInterval = dwInterval;
		m_pVertexPos = new _vec3[m_dwVtxCnt];
		m_dwVtxSize = sizeof(VTXNORTEX);
		m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

		m_dwTriCnt = (dwVtxCntX - 1) * (dwVtxCntZ - 1) * 2;
		m_pIndices = new INDEX32[m_dwTriCnt];
		m_dwIdxSize = sizeof(INDEX32);
		m_IdxFmt = D3DFMT_INDEX32;

		// 정점들을 보관하기위한 메모리공간을 생성한다.
		if (FAILED(CVIBuffer::Ready_Component()))
			return E_FAIL;

		// Heightmap.
// 		HANDLE			hFile = CreateFile(pHeightmap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
// 		if (0 == hFile)
// 			return E_FAIL;
// 
// 		_ulong					dwByte = 0;
// 
// 		BITMAPFILEHEADER		bitmapfileheader;
// 		ReadFile(hFile, &bitmapfileheader, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
// 
// 		BITMAPINFOHEADER		bitmapinfoheader;
// 		ReadFile(hFile, &bitmapinfoheader, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
// 
// 		_ulong*			pPixel = new _ulong[bitmapinfoheader.biWidth * bitmapinfoheader.biHeight];
// 		ReadFile(hFile, pPixel, sizeof(_ulong) * bitmapinfoheader.biWidth * bitmapinfoheader.biHeight, &dwByte, nullptr);

		if (nullptr == m_pVB)
			return E_FAIL;

		VTXNORTEX*		pVertex = nullptr;
		m_pVertexNormalTexture = new VTXNORTEX[dwVtxCntX * dwVtxCntZ];

		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		for (size_t i = 0; i < dwVtxCntZ; ++i)
		{
			for (size_t j = 0; j < dwVtxCntX; ++j)
			{
				_uint	iIndex = i * dwVtxCntX + j;

				pVertex[iIndex].vPosition = _vec3(j * dwInterval, 0.f/*(pPixel[iIndex] & 0x000000ff) / 10.f*/, i * dwInterval);
				m_pVertexPos[iIndex] = pVertex[iIndex].vPosition;
				pVertex[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
				pVertex[iIndex].vTexUV = _vec2(_float(j) / (dwVtxCntX - 1) , _float(i) / (dwVtxCntZ - 1) );
				m_pVertexNormalTexture[iIndex] = pVertex[iIndex];
			}
		}



		if (nullptr == m_pIB)
			return E_FAIL;

		INDEX32*		pIndex = nullptr;

		_uint			iTriIdx = 0;

		m_pIB->Lock(0, 0, (void**)&pIndex, 0);

		for (size_t i = 0; i < dwVtxCntZ - 1; ++i)
		{
			for (size_t j = 0; j < dwVtxCntX - 1; ++j)
			{
				_uint	iIndex = i * dwVtxCntX + j;

				_uint	iVtxIndex[] =
				{
					iIndex + dwVtxCntX,
					iIndex + dwVtxCntX + 1,
					iIndex + 1,
					iIndex
				};

				// 오른쪽 위
				pIndex[iTriIdx]._1 = iVtxIndex[0];
				pIndex[iTriIdx]._2 = iVtxIndex[1];
				pIndex[iTriIdx]._3 = iVtxIndex[2];

				_vec3		vSour, vDest, vNormal;
				vSour = pVertex[iVtxIndex[1]].vPosition - pVertex[iVtxIndex[0]].vPosition;
				vDest = pVertex[iVtxIndex[2]].vPosition - pVertex[iVtxIndex[1]].vPosition;

				D3DXVec3Cross(&vNormal, &vSour, &vDest);

				pVertex[iVtxIndex[0]].vNormal += vNormal;
				pVertex[iVtxIndex[1]].vNormal += vNormal;
				pVertex[iVtxIndex[2]].vNormal += vNormal;

				++iTriIdx;



				// 왼쪽 아래
				pIndex[iTriIdx]._1 = iVtxIndex[0];
				pIndex[iTriIdx]._2 = iVtxIndex[2];
				pIndex[iTriIdx]._3 = iVtxIndex[3];

				vSour = pVertex[iVtxIndex[2]].vPosition - pVertex[iVtxIndex[0]].vPosition;
				vDest = pVertex[iVtxIndex[3]].vPosition - pVertex[iVtxIndex[2]].vPosition;

				D3DXVec3Cross(&vNormal, &vSour, &vDest);

				pVertex[iVtxIndex[0]].vNormal += vNormal;
				pVertex[iVtxIndex[2]].vNormal += vNormal;
				pVertex[iVtxIndex[3]].vNormal += vNormal;

				++iTriIdx;
			}
		}

		for (size_t i = 0; i < m_dwVtxCnt; ++i)
		{
			D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
			m_pVertexNormalTexture->vNormal = pVertex[i].vNormal;
		}

		m_pVB->Unlock();
		m_pIB->Unlock();
// 
// 		CloseHandle(hFile);
// 
// 		Safe_Delete_Array(pPixel);
	}

	else
	{
		m_dwVtxCnt = dwVtxCntX * dwVtxCntZ;
		m_dwCntX = dwVtxCntX;
		m_dwCntZ = dwVtxCntZ;
		m_dwInterval = dwInterval;
		m_dwVtxSize = sizeof(VTXNORTEX);
		m_dwTriCnt = (dwVtxCntX - 1) * (dwVtxCntZ - 1) * 2;
		m_dwIdxSize = sizeof(INDEX32);
		m_IdxFmt = D3DFMT_INDEX32;
		m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
		m_pIndices = new INDEX32[m_dwTriCnt];

		m_pVertexPos = new _vec3[m_dwVtxCnt];

		// 정점들을 보관하기 위한 메모리공간을 생성한다.
		if (FAILED(CVIBuffer::Ready_Component()))
			return E_FAIL;

		// Height 적용
// 		HANDLE			hFile = CreateFile(pHeightmap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
// 		if (0 == hFile)
// 			return E_FAIL;
// 
// 		_ulong					dwByte = 0;
// 
// 		BITMAPFILEHEADER		bitmapfileheader;
// 		ReadFile(hFile, &bitmapfileheader, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
// 
// 		BITMAPINFOHEADER		bitmapinfoheader;
// 		ReadFile(hFile, &bitmapinfoheader, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
// 
// 		_ulong*			pPixel = new _ulong[bitmapinfoheader.biWidth * bitmapinfoheader.biHeight];
// 		ReadFile(hFile, pPixel, sizeof(_ulong) * bitmapinfoheader.biWidth * bitmapinfoheader.biHeight, &dwByte, nullptr);


		if (nullptr == m_pVB)
			return E_FAIL;

		VTXNORTEX*		pVertex = nullptr;

		m_pVertexNormalTexture = new VTXNORTEX[dwVtxCntX * dwVtxCntZ];

		m_pVB->Lock(0, 0, (void**)&pVertex, 0);




		vector<TERRAIN_DATA_SOLE>::iterator iter_begin = pVecTerrainDataSole->begin();
		vector<TERRAIN_DATA_SOLE>::iterator iter_end = pVecTerrainDataSole->end();

		_uint	iIndex = 0;

		for (; iter_begin != iter_end; ++iter_begin)
		{
			pVertex[iIndex].vPosition = (iter_begin)->vPos;
			pVertex[iIndex].vNormal = (iter_begin)->vNormal;
			//	pVertex[iIndex].vTexUV = _vec2(_float(j) / (dwVtxCntX - 1) * 20.f, _float(i) / (dwVtxCntZ - 1) * 20.f); // uv 좌표 설정은 위에서 해주고 있당.
			m_pVertexPos[iIndex] = (iter_begin)->vPos;
			m_pVertexNormalTexture[iIndex] = pVertex[iIndex];
			iIndex++;
		}


		for (size_t i = 0; i < dwVtxCntZ; ++i)
		{
			for (size_t j = 0; j < dwVtxCntX; ++j)
			{
				_uint	iIndex = i * dwVtxCntX + j;
				pVertex[iIndex].vTexUV = _vec2(_float(j) / (dwVtxCntX - 1), _float(i) / (dwVtxCntZ - 1));
			}
		}





		if (nullptr == m_pIB)
			return E_FAIL;

		INDEX32*		pIndex = nullptr;

		_uint			iTriIdx = 0;

		m_pIB->Lock(0, 0, (void**)&pIndex, 0);


		for (size_t i = 0; i < dwVtxCntZ - 1; ++i)
		{
			for (size_t j = 0; j < dwVtxCntX - 1; ++j)
			{
				_uint	iIndex = i * dwVtxCntX + j;

				_uint	iVtxIndex[] =
				{
					iIndex + dwVtxCntX,
					iIndex + dwVtxCntX + 1,
					iIndex + 1,
					iIndex
				};

				// 오른쪽 위
				pIndex[iTriIdx]._1 = iVtxIndex[0];
				pIndex[iTriIdx]._2 = iVtxIndex[1];
				pIndex[iTriIdx]._3 = iVtxIndex[2];

				_vec3		vSour, vDest, vNormal;
				vSour = pVertex[iVtxIndex[1]].vPosition - pVertex[iVtxIndex[0]].vPosition;
				vDest = pVertex[iVtxIndex[2]].vPosition - pVertex[iVtxIndex[1]].vPosition;

				D3DXVec3Cross(&vNormal, &vSour, &vDest);

				pVertex[iVtxIndex[0]].vNormal += vNormal;
				pVertex[iVtxIndex[1]].vNormal += vNormal;
				pVertex[iVtxIndex[2]].vNormal += vNormal;

				++iTriIdx;



				// 왼쪽 아래
				pIndex[iTriIdx]._1 = iVtxIndex[0];
				pIndex[iTriIdx]._2 = iVtxIndex[2];
				pIndex[iTriIdx]._3 = iVtxIndex[3];

				vSour = pVertex[iVtxIndex[2]].vPosition - pVertex[iVtxIndex[0]].vPosition;
				vDest = pVertex[iVtxIndex[3]].vPosition - pVertex[iVtxIndex[2]].vPosition;

				D3DXVec3Cross(&vNormal, &vSour, &vDest);

				pVertex[iVtxIndex[0]].vNormal += vNormal;
				pVertex[iVtxIndex[2]].vNormal += vNormal;
				pVertex[iVtxIndex[3]].vNormal += vNormal;

				++iTriIdx;
			}
		}

		for (size_t i = 0; i < m_dwVtxCnt; ++i)
		{
			D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
						m_pVertexNormalTexture->vNormal = pVertex[i].vNormal;
		}

		m_pVB->Unlock();
		m_pIB->Unlock(); // 연 순서대로 닫자.


// 		CloseHandle(hFile);
// 		Safe_Delete_Array(pPixel);

	}

	return NOERROR;
}

void CTerrain_Buffer::Render_Buffer(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// 장치가 사용해야할 정점버퍼를 장치에게 셋팅한다.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

HRESULT CTerrain_Buffer::Optimize_Buffer(CFrustum * pFrustum)
{
	if (nullptr == m_pVertexPos)
		return E_FAIL;

	ZeroMemory(m_pIndices, sizeof(INDEX32) * m_dwTriCnt);

	_ulong dwTriCnt = 0;

	/*for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{*/
	for (size_t i = 0; i < m_dwCntZ - 1; ++i)
	{
		for (size_t j = 0; j < m_dwCntX - 1; ++j)
		{
			_uint iVertexIndex = i * m_dwCntX + j;

			_uint		iIndex[4] = { iVertexIndex + m_dwCntX, iVertexIndex + m_dwCntX + 1, iVertexIndex + 1, iVertexIndex };
			_bool		isIn[4] = { true, true, true, true };

			for (size_t k = 0; k < 4; ++k)
				isIn[k] = pFrustum->Culling_toFrustum(&m_pVertexPos[iIndex[k]], 0.0f);

			if (true == isIn[0]
				|| true == isIn[1]
				|| true == isIn[2])
			{
				m_pIndices[dwTriCnt]._1 = iIndex[0];
				m_pIndices[dwTriCnt]._2 = iIndex[1];
				m_pIndices[dwTriCnt]._3 = iIndex[2];
				++dwTriCnt;
			}

			if (true == isIn[0]
				|| true == isIn[2]
				|| true == isIn[3])
			{
				m_pIndices[dwTriCnt]._1 = iIndex[0];
				m_pIndices[dwTriCnt]._2 = iIndex[2];
				m_pIndices[dwTriCnt]._3 = iIndex[3];
				++dwTriCnt;
			}
		}
	}

	/*}*/

	INDEX32*		pOriginal_Indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriginal_Indices, 0);

	memcpy(pOriginal_Indices, m_pIndices, sizeof(INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;

	return NOERROR;
}

CComponent * CTerrain_Buffer::Clone(void)
{
	return new CTerrain_Buffer(*this);
}

CTerrain_Buffer * CTerrain_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightmap, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ, const _ulong& dwInterval, vector<TERRAIN_DATA_SOLE>* pVecTerrainDataSole)
{
	CTerrain_Buffer *	pInstance = new CTerrain_Buffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(pHeightmap, dwVtxCntX, dwVtxCntZ, dwInterval, pVecTerrainDataSole)))
	{
		MSG_BOX("CTerrain_Buffer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTerrain_Buffer::Free(void)
{
	if (false == m_isClone)
		Safe_Delete_Array(m_pIndices);

	return CVIBuffer::Free();
}
