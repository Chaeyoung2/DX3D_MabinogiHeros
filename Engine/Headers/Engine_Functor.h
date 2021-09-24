#pragma once


class CFind_Tag
{
public:
	explicit CFind_Tag(const TCHAR* pTag) : m_pTag(pTag) {}
	~CFind_Tag(void) = default;
public:
	template <typename T>
	bool operator () (T& Pair)
	{
		if(0 == lstrcmp(Pair.first, m_pTag))
			return true;
		return false;
	}
private:
	const TCHAR*	m_pTag;


};
