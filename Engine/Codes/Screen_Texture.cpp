#include "..\Headers\Screen_Texture.h"

USING(Engine)

CScreen_Texture::CScreen_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

CScreen_Texture::CScreen_Texture(const CScreen_Texture & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CScreen_Texture::Ready_Component(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXSCREENTEX);
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	m_dwTriCnt = 2;
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_pVertexPos = new _vec3[m_dwVtxCnt];

	// 정점들을 보관하기위한 메모리공간을 생성한다.
	if (FAILED(CVIBuffer::Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pVB)
		return E_FAIL;

	// 추가적인 스레드가 값을 갱신하는동안 접근하지못하도록 잠근다.
	// 갱신하고자하는 공간의 주소를 리턴해준다.
	VTXSCREENTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(-0.5f, -0.5f, 0.f, 1.f);
	m_pVertexPos[0] = _vec3(pVertex[0].vPosition.x, pVertex[0].vPosition.y, pVertex[0].vPosition.z);
	pVertex[0].vTexUV = _vec2(0.0f, 0.f);

	pVertex[1].vPosition = _vec4(0.5f, -0.5f, 0.f, 1.f);
	m_pVertexPos[1] = _vec3(pVertex[1].vPosition.x, pVertex[1].vPosition.y, pVertex[1].vPosition.z);
	pVertex[1].vTexUV = _vec2(1.0f, 0.f);

	pVertex[2].vPosition = _vec4(0.5f, 0.5f, 0.f, 1.f);
	m_pVertexPos[2] = _vec3(pVertex[2].vPosition.x, pVertex[2].vPosition.y, pVertex[2].vPosition.z);
	pVertex[2].vTexUV = _vec2(1.0f, 1.f);

	pVertex[3].vPosition = _vec4(-0.5f, 0.5f, 0.f, 1.f);
	m_pVertexPos[3] = _vec3(pVertex[3].vPosition.x, pVertex[3].vPosition.y, pVertex[3].vPosition.z);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	//pVertex[0].vPosition = _vec4(0.0f, 0.0f, 0.f, 1.f);
	//pVertex[0].vTexUV = _vec2(0.0f, 0.f);

	//pVertex[1].vPosition = _vec4(512.0f, 0.0f, 0.f, 1.f);
	//pVertex[1].vTexUV = _vec2(1.0f, 0.f);

	//pVertex[2].vPosition = _vec4(512.f, 768.0f, 0.f, 1.f);
	//pVertex[2].vTexUV = _vec2(1.0f, 1.f);

	//pVertex[3].vPosition = _vec4(-512.0f, 768.0f, 0.f, 1.f);
	//pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;

}

_int CScreen_Texture::Update_Component(const _float & fTimeDelta)
{
	return _int();
}

void CScreen_Texture::Render_Buffer(const _matrix* pWorldMatrix)
{
	if (nullptr == m_pGraphicDev)
		return;

	Transform(pWorldMatrix);

	// 장치가 사용해야할 정점버퍼를 장치에게 셋팅한다.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CScreen_Texture::Transform(const _matrix* pWorldMatrix)
{
	VTXSCREENTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (size_t i = 0; i < m_dwVtxCnt; i++)
	{
		_vec3		vVector;

		D3DXVec3TransformCoord(&vVector, &m_pVertexPos[i], pWorldMatrix);

		pVertex[i].vPosition = _vec4(vVector, 1.f);
	}

	m_pVB->Unlock();


}

CComponent * CScreen_Texture::Clone(void)
{
	return new CScreen_Texture(*this);
}

CScreen_Texture * CScreen_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScreen_Texture *	pInstance = new CScreen_Texture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CScreen_Texture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
	
}

_ulong CScreen_Texture::Free(void)
{

	return CVIBuffer::Free();
}
