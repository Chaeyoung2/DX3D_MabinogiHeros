#include "..\Headers\Rect_Texture.h"

USING(Engine)

CRect_Texture::CRect_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

CRect_Texture::CRect_Texture(const CRect_Texture & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CRect_Texture::Ready_Component(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	m_dwTriCnt = 2;
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	// 정점들을 보관하기위한 메모리공간을 생성한다.
	if (FAILED(CVIBuffer::Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pVB)
		return E_FAIL;

	// 추가적인 스레드가 값을 갱신하는동안 접근하지못하도록 잠근다.
	// 갱신하고자하는 공간의 주소를 리턴해준다.
	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.0f, 0.f);

	pVertex[1].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.0f, 0.f);

	pVertex[2].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.0f, 1.f);

	pVertex[3].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

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

_int CRect_Texture::Update_Component(const _float & fTimeDelta)
{
	return _int();
}

void CRect_Texture::Render_Buffer(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// 장치가 사용해야할 정점버퍼를 장치에게 셋팅한다.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CComponent * CRect_Texture::Clone(void)
{
	return new CRect_Texture(*this);
}

CRect_Texture * CRect_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRect_Texture *	pInstance = new CRect_Texture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CRect_Texture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
	
}

_ulong CRect_Texture::Free(void)
{
	return CVIBuffer::Free();
}
