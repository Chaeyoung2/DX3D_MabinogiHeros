#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CFrame;
class DLL_EXPORT CFrame_Manager : public CBase
{
	DECLARE_SINGLETON(CFrame_Manager)
private:
	explicit CFrame_Manager();
	virtual ~CFrame_Manager() = default;
public:
	HRESULT Add_Frame(const _tchar* pFrameTag, const _float& fCallCnt);
	_bool Permit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
private:
	map<const _tchar*, CFrame*>				m_mapFrames;
	typedef map<const _tchar*, CFrame*>		MAPFRAMES;
private:
	CFrame* Find_Frame(const _tchar* pFrameTag);

public:
	virtual _ulong Free(void);
	
};

END

