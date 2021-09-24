#pragma once

namespace Engine
{
	template <typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			if (dwRefCnt = pInstance->Release())
				return dwRefCnt;
			pInstance = nullptr;
		}

		return dwRefCnt;
	}


	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete [] pPointer;
			pPointer = nullptr;
		}
	}

}