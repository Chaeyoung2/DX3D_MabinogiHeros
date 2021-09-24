#pragma once

#include "GameObject.h"

BEGIN(Engine)

class DLL_EXPORT CCamera abstract : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);	
protected:
	_matrix				m_matView; // 뷰스페이스 변환행렬.[
	_matrix				m_matWorld;
	CAMERAINFO			m_CameraInfo;
protected:
	_matrix				m_matProj; // 투영 변환행렬.
	PROJINFO			m_ProjInfo;
private:
	_matrix* Matrix_LookAtLH(_matrix* pOutMatrix, const _vec3* pEye, const _vec3* pAt, const _vec3* pAxisY);
	_matrix* Matrix_PerspectiveFovLH(_matrix* pOutMatrix, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);
public:	
	virtual _ulong Free(void);
};

END