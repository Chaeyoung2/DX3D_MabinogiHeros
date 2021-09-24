#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase()
	: m_dwRefCnt(0)
{
}

_ulong CBase::AddRef(void)
{
	return ++m_dwRefCnt;
}

_ulong CBase::Release(void)
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}
