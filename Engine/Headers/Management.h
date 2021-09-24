#pragma once

#include "Base.h"

BEGIN(Engine)

class CScene;
class DLL_EXPORT CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement(void);
	virtual ~CManagement() = default;
public:
	HRESULT SetUp_CurrentScene(CScene* pNewScene);
	_int Update_CurrentScene(const _float& fTimeDelta);
	void Render_CurrentScene(void);
private:
	CScene*			m_pScene = nullptr;
public:
	virtual _ulong Free(void);

};

END

