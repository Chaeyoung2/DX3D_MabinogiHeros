#pragma once

#include "Component.h"

BEGIN(Engine)


class DLL_EXPORT CInformation : public CComponent
{
private:
	explicit CInformation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInformation(const CInformation& rhs);
	virtual ~CInformation() = default;
public:
	HRESULT Ready_Information(OBJ_INFO tInfo);
	HRESULT	Update_Information(void);
	void		HPMinus(_float fMinusValue) {
		m_tObjInfo.fHP -= fMinusValue;
	}
public:
	OBJ_INFO		Get_ObjInfo() {
		return m_tObjInfo;
	}
public:
	static CInformation* Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_INFO tInfo);
	virtual CComponent* Clone(void);
	virtual _ulong Free(void);
private:
	OBJ_INFO			m_tObjInfo;
};

END