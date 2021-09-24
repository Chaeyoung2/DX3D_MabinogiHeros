#include "..\Headers\AnimationCtrl.h"

USING(Engine)

CAnimationCtrl::CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXANIMATIONCONTROLLER	pAniCtrl)
	: m_pGraphicDev(pGraphicDev)
	, m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
{
	m_pAniCtrl->AddRef();
	m_pGraphicDev->AddRef();
}

CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), rhs.m_pAniCtrl->GetMaxNumAnimationSets()
		, rhs.m_pAniCtrl->GetMaxNumTracks(), rhs.m_pAniCtrl->GetMaxNumEvents(), &m_pAniCtrl);
	m_pGraphicDev->AddRef();
}

HRESULT CAnimationCtrl::Ready_AnimationCtrl(void)
{


	return NOERROR;
}

void CAnimationCtrl::Set_AnimationSet(const _uint& iIndex, _double dMagicNumber)
{
	//if (m_iCurrentIdx == iIndex) // ���� �ε����� ���ڷ� ���� �ε����� �����ϸ� return.
	//	return;
	m_iAnimationIdx = iIndex;

	if (nullptr == m_pAniCtrl) // �ִϸ��̼� ��Ʈ�ѷ��� ������ return.
		return;

	//D3DXTRACK_DESC			Track_Desc;
	//m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &Track_Desc); // ������ ���� Ʈ��.
	//m_TrackPosition = Track_Desc.Position; 


	if (m_iCurrentTrack == 0)
		m_iNewTrack = 1;
	else
		m_iNewTrack = 0;

	LPD3DXANIMATIONSET			pAniSet = nullptr; // �ִϸ��̼� �� ����.

	if (FAILED(m_pAniCtrl->GetAnimationSet(iIndex, &pAniSet))) // ��� �ִ� �ִϸ��̼� �¿� ���ڷ� ���� �ε����� �ش��ϴ� ���ϸ��̼� ���� ������ ��.
		return;

	// �ִϸ��̼� ���� ����Ǵµ� �ɸ��� ��ü�ð�
	m_Period = pAniSet->GetPeriod();

	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAniSet);
	pAniSet->Release(); // �ʿ���.

	// �ִϸ��̼ǿ� ���� �����۾�.
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

//	_double dMagicNumber = 0.25;

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeDeltaAcc + dMagicNumber);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.0f, m_TimeDeltaAcc, dMagicNumber, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_TimeDeltaAcc, dMagicNumber, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.0f, m_TimeDeltaAcc, dMagicNumber, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_TimeDeltaAcc, dMagicNumber, D3DXTRANSITION_LINEAR);

	m_TimeDeltaAcc = 0.f;
	m_iCurrentTrack = m_iNewTrack;
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_pAniCtrl->ResetTime();

	//m_iCurrentIdx = iIndex;
}

void CAnimationCtrl::Play_AnimationSet(const _float & fTimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return;

	//
// 	LPD3DXANIMATIONSET			pAniSet = nullptr; // �ִϸ��̼� �� ����.
// 
// 	if (FAILED(m_pAniCtrl->GetAnimationSet(m_iCurrentIdx, &pAniSet))) // ��� �ִ� �ִϸ��̼� �¿� ���ڷ� ���� �ε����� �ش��ϴ� ���ϸ��̼� ���� ������ ��.
// 		return;
// 
// 	// �ִϸ��̼� ���� ����Ǵµ� �ɸ��� ��ü�ð�
// 	m_Period = pAniSet->GetPeriod();
// 
// 	D3DXTRACK_DESC			Track_Desc;
// 	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &Track_Desc); // ������ ���� Ʈ��.
// 	//
	m_pAniCtrl->AdvanceTime(fTimeDelta, nullptr);
	m_TimeDeltaAcc += fTimeDelta;
}

_double CAnimationCtrl::Get_TrackPosition()
{
	D3DXTRACK_DESC			Track_Desc;
	ZeroMemory(&Track_Desc, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &Track_Desc); // ������ ���� Ʈ��.
	
	m_TrackPosition = Track_Desc.Position; // ���⼭ �޾ƿ��� �־��.

	return m_TrackPosition;
}

CAnimationCtrl * CAnimationCtrl::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXANIMATIONCONTROLLER	pAniCtrl)
{
	CAnimationCtrl *	pInstance = new CAnimationCtrl(pGraphicDev, pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		MSG_BOX("CAnimationCtrl Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimationCtrl * CAnimationCtrl::Create(const CAnimationCtrl & rhs)
{
	CAnimationCtrl *	pInstance = new CAnimationCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		MSG_BOX("CAnimationCtrl Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CAnimationCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
	Safe_Release(m_pGraphicDev);

	return _ulong();
}
