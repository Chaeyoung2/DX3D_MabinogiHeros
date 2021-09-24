#include "..\Headers\Frame.h"

USING(Engine)

CFrame::CFrame()
	: m_fCallPerSec(0.f)
	, m_fTimeAcc(0.f)
{
}

HRESULT CFrame::Ready_Frame(const _float & fCallCnt)
{
	m_fCallPerSec = 1.f / fCallCnt;

	return NOERROR;
}

_bool CFrame::Permit_Call(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= m_fCallPerSec)
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	return false;
}

CFrame * CFrame::Create(const _float & fCallCnt)
{
	CFrame * pInstance = new CFrame();

	if (FAILED(pInstance->Ready_Frame(fCallCnt)))
	{
		MSG_BOX("CFrame Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CFrame::Free(void)
{
	return _ulong();
}
