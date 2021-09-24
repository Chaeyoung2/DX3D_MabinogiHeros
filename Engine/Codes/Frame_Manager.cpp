#include "..\Headers\Frame_Manager.h"
#include "Frame.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrame_Manager)

CFrame_Manager::CFrame_Manager()
{
}




HRESULT CFrame_Manager::Add_Frame(const _tchar * pFrameTag, const _float & fCallCnt)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallCnt);

	if (nullptr == pFrame)
		return E_FAIL;

	m_mapFrames.insert(MAPFRAMES::value_type(pFrameTag, pFrame));

	return NOERROR;
}

_bool CFrame_Manager::Permit_Call(const _tchar * pFrameTag, const _float& fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (nullptr == pFrame)
		return false;
	
	return pFrame->Permit_Call(fTimeDelta);
}

CFrame * CFrame_Manager::Find_Frame(const _tchar * pFrameTag)
{
	auto iter = find_if(m_mapFrames.begin(), m_mapFrames.end(), CFind_Tag(pFrameTag));

	if(iter == m_mapFrames.end())
		return nullptr;

	return iter->second;
}

_ulong CFrame_Manager::Free(void)
{
	_ulong		dwRefCnt = 0;

	for (auto& Pair : m_mapFrames)
	{
		if (dwRefCnt = Engine::Safe_Release(Pair.second))
			return dwRefCnt;
	}
	m_mapFrames.clear();

	return dwRefCnt;
}
