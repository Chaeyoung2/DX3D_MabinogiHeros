#include "..\Headers\Timer_Manager.h"
#include "Timer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_float CTimer_Manager::Get_TimeDelta(const _tchar * pTimerTag) const
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.0f;

	return pTimer->Get_TimeDelta();	
}

void CTimer_Manager::Compute_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return;

	pTimer->Compute_TimeDelta();
}

HRESULT CTimer_Manager::Add_Timer(const _tchar * pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimers.insert(MAPTIMERS::value_type(pTimerTag, pTimer));

	return NOERROR;
}

CTimer* CTimer_Manager::Find_Timer(const _tchar * pTimerTag) const
{
	auto iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CFind_Tag(pTimerTag));

	if(iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

_ulong CTimer_Manager::Free(void)
{
	_ulong		dwRefCnt = 0;

	for (auto& Pair : m_mapTimers)
	{
		if (dwRefCnt = Engine::Safe_Release(Pair.second))
			return dwRefCnt;
	}
	m_mapTimers.clear();

	return dwRefCnt;
}

