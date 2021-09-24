#pragma once

#include "Engine_Defines.h"

// 어지간한 클래스들의 부모가되는 클래스이다.
// 레퍼런스 카운트를 관리한다.

BEGIN(Engine)

class DLL_EXPORT CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	_ulong AddRef(void); // _ulong 함수안에서 증가시킨 dwRefCnt를 리턴.
	_ulong Release(void); // _ulong 함수안에서 감소시키기 이전의 dwRefCnt를 리턴.[
private:
	_ulong			m_dwRefCnt = 0;
public:
	virtual _ulong Free(void) = 0;
};

END



