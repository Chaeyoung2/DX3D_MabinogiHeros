#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;
public:
	LPD3DXEFFECT Get_EffectHandle(void) const {
		return m_pEffect;
	}
public:
	HRESULT Ready_Shader(const _tchar* pFilePath);
private:
	LPD3DXEFFECT			m_pEffect = nullptr;
	LPD3DXBUFFER			m_pErrBuffer = nullptr;
public:
	virtual CComponent* Clone(void);
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual _ulong Free(void);
};

END