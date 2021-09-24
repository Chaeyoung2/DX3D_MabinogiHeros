#include "..\Headers\Texture.h"

USING(Engine)

CTexture::CTexture( LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)	
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_TextureList(rhs.m_TextureList)	
{
	for (auto& pTexture : m_TextureList)	
		pTexture->AddRef();	
}

HRESULT CTexture::Ready_Texture(TEXTURETYPE eTextureType, const _tchar * pFilename, const _uint & iNumTexture, const _uint& iWidth, const _uint& iHeight)
{
	_tchar			szFileName[128] = L"";

	for (size_t i = 0; i < iNumTexture; ++i)
	{
		wsprintf(szFileName, pFilename, i);

		IDirect3DBaseTexture9*		pTexture = nullptr;

		if (TYPE_NORMAL == eTextureType)
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (IDirect3DTexture9**)&pTexture)))
				return E_FAIL;				
		}
		else if(TYPE_CUBE == eTextureType)
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (IDirect3DCubeTexture9**)&pTexture)))
				return E_FAIL;
		}	
 		else if (TYPE_EFFECT == eTextureType)
		{
			TEX_INFO*	pTexInfo = new TEX_INFO;
			ZeroMemory(pTexInfo, sizeof(TEX_INFO));

			if (FAILED(D3DXGetImageInfoFromFile(szFileName, &pTexInfo->ImgInfo)))
				return E_FAIL;

			if (FAILED(D3DXCreateTextureFromFileEx(m_pGraphicDev, szFileName, iWidth, iHeight, 1, NULL,
				pTexInfo->ImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
				0xff000000, &pTexInfo->ImgInfo, NULL, (IDirect3DTexture9**)&pTexture/*&pTexInfo->pTexture*/)))
				return E_FAIL;

			Safe_Delete(pTexInfo);
 		}

		m_TextureList.push_back(pTexture);
	}


	return NOERROR;
}

HRESULT CTexture::SetUp_OnGraphicDev(const _uint & iStage, const _uint & iIndex)
{
	auto iter = m_TextureList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	m_pGraphicDev->SetTexture(iStage, *iter);

	return NOERROR;
}

HRESULT CTexture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _uint & iIndex)
{
	auto iter = m_TextureList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	pEffect->SetTexture(pConstantName, *iter);

	return NOERROR;
}

HRESULT CTexture::Release_OnShader(const _uint & iIndex)
{
	auto iter = m_TextureList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	Safe_Release(*iter);

	return NOERROR;
}

CComponent * CTexture::Clone(void)
{
	return new CTexture(*this);
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eTextureType, const _tchar * pFilename, const _uint & iNumTexture, const _uint& iWidth, const _uint& iHeight)
{
	CTexture *	pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(eTextureType, pFilename, iNumTexture, iWidth, iHeight)))
	{	
		MSG_BOX("CTexture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;

}

_ulong CTexture::Free(void)
{
	_ulong dwRefCnt = 0;

	for (auto& pTexture : m_TextureList)
	{
		if(false == m_isClone)
		{
			if (dwRefCnt = Safe_Release(pTexture))
				return dwRefCnt;
		}
		else
		{
			Safe_Release(pTexture);
		}
	}
	m_TextureList.clear();

	return CComponent::Free();	
}
