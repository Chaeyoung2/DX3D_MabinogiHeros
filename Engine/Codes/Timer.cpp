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
	// QueryPerformanceCounter : 현재 CPU가 카운팅한 숫자를 담아준다.
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	// QueryPerformanceFrequency : 현재 CPU가 1초에 카운팅할 수 있는 숫자를 가져온다.
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

