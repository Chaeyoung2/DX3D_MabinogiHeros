#pragma once

#include "Engine_Defines.h"

// �������� Ŭ�������� �θ𰡵Ǵ� Ŭ�����̴�.
// ���۷��� ī��Ʈ�� �����Ѵ�.

BEGIN(Engine)

class DLL_EXPORT CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	_ulong AddRef(void); // _ulong �Լ��ȿ��� ������Ų dwRefCnt�� ����.
	_ulong Release(void); // _ulong �Լ��ȿ��� ���ҽ�Ű�� ������ dwRefCnt�� ����.[
private:
	_ulong			m_dwRefCnt = 0;
public:
	virtual _ulong Free(void) = 0;
};

END



