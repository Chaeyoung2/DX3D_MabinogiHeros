#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTimer;
class DLL_EXPORT CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;
public:
	_float Get_TimeDelta(const _tchar* pTimerTag) const;
public:
	void Compute_TimeDelta(const _tchar* pTimerTag);
public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
private:
	map<const _tchar*, CTimer*>				m_mapTimers;
	typedef map<const _tchar*, CTimer*>		MAPTIMERS;
private:
	CTimer* Find_Timer(const _tchar* pTimerTag) const;
public:
	virtual _ulong Free(void) final;
};

END