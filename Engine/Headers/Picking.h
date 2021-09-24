#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CPicking : public CComponent
{
private:
	explicit CPicking(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPicking(const CPicking& rhs);
	virtual ~CPicking() = default;
public:
	HRESULT Ready_Picking(void);
	_int Picking_OnRectBuffer(HWND hWnd, const _vec3* pVertices, const _uint& iCntX, const _uint& iCntZ);
	_vec3 Picking_OnRectBuffer_ReturnVec3(HWND hWnd, const _vec3* pVertices, const _uint& iCntX, const _uint& iCntZ);
public:
	virtual CComponent* Clone(void);
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END