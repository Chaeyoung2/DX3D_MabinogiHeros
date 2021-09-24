#include "..\Headers\Information.h"


USING(Engine)


CInformation::CInformation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CInformation::CInformation(const CInformation & rhs)
	: CComponent(rhs)
	, m_tObjInfo(rhs.m_tObjInfo)
{

}

HRESULT CInformation::Ready_Information(OBJ_INFO tInfo)
{
	m_tObjInfo = tInfo;

	return NOERROR;
}

HRESULT CInformation::Update_Information(void)
{
	return NOERROR;
}

CInformation * CInformation::Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_INFO tInfo)
{
	CInformation *	pInstance = new CInformation(pGraphicDev);

	if (FAILED(pInstance->Ready_Information(tInfo)))
	{
		MSG_BOX("CInformation Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CInformation::Clone(void)
{
	return new CInformation(*this);
}

_ulong CInformation::Free(void)
{
	CComponent::Free();

	return _ulong();
}
