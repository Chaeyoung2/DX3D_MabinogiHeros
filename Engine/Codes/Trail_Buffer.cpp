#include "..\Headers\Trail_Buffer.h"


CTrail_Buffer::CTrail_Buffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pVertex(NULL)
{

}

CTrail_Buffer::CTrail_Buffer(const CTrail_Buffer & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CTrail_Buffer::Ready_Component()
{
	m_dwVtxSize = sizeof(VTXTRAIL);
	m_dwVtxCnt = 0;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_dwTriCnt = 0;

	return NOERROR;
}

void CTrail_Buffer::Render_Buffer(const _matrix * pMatWorld)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pMatWorld);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	/*m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);*/
	/*m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);*/

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_dwTriCnt, m_pVertex, m_dwVtxSize);

	/*m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);*/
	/*m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);*/

	Engine::Safe_Delete_Array(m_pVertex);
}

void CTrail_Buffer::Set_VertexTrail(const _vec3 * pPos, const _vec3 * pDir, const _float & fLength, const DWORD & dwCnt)
{
	m_pVertex = new VTXTRAIL[dwCnt];
	ZeroMemory(m_pVertex, sizeof(VTXTRAIL) * dwCnt);

	for (DWORD i = 0; i < dwCnt / 2; ++i)
	{
		m_pVertex[i * 2 + 0].vPosition =  pPos[i] - pDir[i] * fLength;
		m_pVertex[i * 2 + 0].vTexUV = D3DXVECTOR2(1.f, (i * 2 + 0) / float(dwCnt - 2.f));

		m_pVertex[i * 2 + 1].vPosition = pPos[i] + pDir[i] * fLength;
		m_pVertex[i * 2 + 1].vTexUV = D3DXVECTOR2(0.f, (i * 2 + 1) / float(dwCnt - 1.f));
	}
	m_dwTriCnt = dwCnt - 2;
}
 
CComponent * CTrail_Buffer::Clone(void)
{
	return  new CTrail_Buffer(*this);
}

CTrail_Buffer * CTrail_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrail_Buffer *	pInstance = new CTrail_Buffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CTrail_Buffer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTrail_Buffer::Free(void)
{
	return _ulong();
}
