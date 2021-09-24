#include "..\Headers\VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVertexPos(nullptr)
	, m_dwCntX(0)
	, m_dwCntZ(0)
	, m_dwInterval(0)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_IdxFmt(rhs.m_IdxFmt)
	, m_pVertexPos(rhs.m_pVertexPos)
	, m_dwCntX(rhs.m_dwCntX)
	, m_dwCntZ(rhs.m_dwCntZ)
	, m_dwInterval(rhs.m_dwInterval)
{
	if(nullptr != m_pVB)
		m_pVB->AddRef();

	if (nullptr != m_pIB)
		m_pIB->AddRef();
}

HRESULT CVIBuffer::Ready_Component(void)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return NOERROR;
}

_int CVIBuffer::Update_Component(const _float & fTimeDelta)
{
	return _int();
}

void CVIBuffer::Render_Buffer(void)
{
}

_ulong CVIBuffer::Free(void)
{
	_ulong		dwRefCnt = 0;

	if(false == m_isClone)
	{
		Safe_Delete_Array(m_pVertexPos);

		if (dwRefCnt = Engine::Safe_Release(m_pVB))
		{
			MSG_BOX("Vertex Buffer Release Failed");
			return dwRefCnt;
		}

		if (dwRefCnt = Engine::Safe_Release(m_pIB))
		{
			MSG_BOX("Index Buffer Release Failed");
			return dwRefCnt;
		}
	}

	else
	{
		Engine::Safe_Release(m_pVB);
		Engine::Safe_Release(m_pIB);
	}

	return CComponent::Free();
}


