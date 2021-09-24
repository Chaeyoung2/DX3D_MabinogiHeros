#include "..\Headers\Timer.h"

USING(Engine)

CTimer::CTimer()
{
}

void CTimer::Compute_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	//if (m_FrameTime.QuadPart - m_LastTime.QuadPart >= m_CpuTick.QuadPart)
	//{
	//	QueryPerformanceFrequency(&m_CpuTick);
	//	m_LastTime = m_FrameTime;
	//}

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_FixTime.QuadPart) 
		/ m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;

}

HRESULT CTimer::Ready_Timer(void)
{
	// QueryPerformanceCounter : ���� CPU�� ī������ ���ڸ� ����ش�.
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	// QueryPerformanceFrequency : ���� CPU�� 1�ʿ� ī������ �� �ִ� ���ڸ� �����´�.
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}

CTimer * CTimer::Create(void)
{
	CTimer *	pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("CTimer Created Failed");
		Safe_Release(pInstance);	
	}
	return pInstance;
}

_ulong CTimer::Free(void)
{
	return _ulong();
}

