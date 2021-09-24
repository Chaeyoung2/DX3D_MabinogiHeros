#pragma once

#include "Component.h"
#include "Cell.h"

BEGIN(Engine)


class DLL_EXPORT CNavigation : public CComponent
{
private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;
public:
	void SetUp_CurrentIndex(const _uint& iIndex) {
		m_iCurrentCell = iIndex;
	}

public:
	HRESULT Ready_Navigation(const _tchar* pFilePath);

	CCell::MOVETYPE isMoving(const _vec3* pPosition, const _vec3* pDirection);
	_vec3		GetWallNormal(const _vec3* pPosition, const _vec3* pDirection);

	void Render_Navigation(void);

private:
	vector<CCell*>			m_vecCell;
	typedef vector<CCell*>	VECCELL;
private:
	_uint					m_iCurrentCell = 0;
	_uint					m_iNumCell = 0;
private:
	HRESULT Make_Neighbor(void);
public:
	virtual CComponent* Clone(void);
	static CNavigation* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual _ulong Free(void);
};

END