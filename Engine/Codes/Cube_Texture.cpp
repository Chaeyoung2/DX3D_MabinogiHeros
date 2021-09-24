#include "..\Headers\Cube_Texture.h"

USING(Engine)

CCube_Texture::CCube_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

CCube_Texture::CCube_Texture(const CCube_Texture & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CCube_Texture::Ready_Component(void)
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBETEX);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	m_dwTriCnt = 12;
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	// �������� �����ϱ����� �޸𸮰����� �����Ѵ�.
	if (FAILED(CVIBuffer::Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pVB)
		return E_FAIL;

	// �߰����� �����尡 ���� �����ϴµ��� �����������ϵ��� ��ٴ�.
	// �����ϰ����ϴ� ������ �ּҸ� �������ش�.
	VTXCUBETEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vTexUV = pVertex[3].vPosition;


	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vTexUV = pVertex[7].vPosition;

	

	m_pVB->Unlock();

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

_int CCube_Texture::Update_Component(const _float & fTimeDelta)
{
	return _int();
}

void CCube_Texture::Render_Buffer(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	// ��ġ�� ����ؾ��� �������۸� ��ġ���� �����Ѵ�.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CComponent * CCube_Texture::Clone(void)
{
	return new CCube_Texture(*this);
}

CCube_Texture * CCube_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube_Texture *	pInstance = new CCube_Texture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CCube_Texture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
	
}

_ulong CCube_Texture::Free(void)
{
	return CVIBuffer::Free();
}
