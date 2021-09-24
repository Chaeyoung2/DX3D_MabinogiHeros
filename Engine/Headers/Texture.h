#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CTexture final : public CComponent
{
public:
	enum TEXTURETYPE { TYPE_NORMAL, TYPE_CUBE, TYPE_EFFECT, TYPE_END };
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT Ready_Texture(TEXTURETYPE eTextureType, const _tchar* pFilename, const _uint& iNumTexture, const _uint& iWidth = 0, const _uint& iHeight = 0);
	HRESULT SetUp_OnGraphicDev(const _uint& iStage, const _uint& iIndex = 0);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _uint& iIndex = 0);
	HRESULT Release_OnShader(const _uint& iIndex = 0);
private:	
	list<IDirect3DBaseTexture9*>			m_TextureList;
	typedef list<IDirect3DBaseTexture9*>	TEXTURELIST;
public:
	virtual CComponent* Clone(void);
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eTextureType, const _tchar* pFilename, const _uint& iNumTexture = 1, const _uint& iWidth = 0, const _uint& iHeight = 0);
	virtual _ulong Free(void);
};

END