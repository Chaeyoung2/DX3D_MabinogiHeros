#include "..\Headers\Shader.h"

USING(Engine)

CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrBuffer(rhs.m_pErrBuffer)
{
	m_pEffect->AddRef();

	if (nullptr != m_pErrBuffer)
		m_pErrBuffer->AddRef();
}

HRESULT CShader::Ready_Shader(const _tchar * pFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &m_pErrBuffer)))
	{
		MessageBoxA(0, (char*)m_pErrBuffer->GetBufferPointer(), nullptr, MB_OK);
		return E_FAIL;
	}

	if (nullptr != m_pErrBuffer)
		MessageBoxA(0, (char*)m_pErrBuffer->GetBufferPointer(), nullptr, MB_OK);

	return NOERROR;
}

CComponent * CShader::Clone(void)
{
	return new CShader(*this);
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath)
{
	CShader *	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
	{
		MessageBox(0, L"CShader Created Failed", nullptr, MB_OK);
		pInstance->Release();
		pInstance = nullptr;
	}
	return pInstance;
}

_ulong CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pErrBuffer);

	return CComponent::Free();
}
