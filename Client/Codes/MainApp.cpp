#include "stdafx.h"
#include "..\Headers\MainApp.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Management.h"
#include "Scene_Logo.h"
#include "Input_Device.h"
#include "Timer_Manager.h"

#include "Loading.h"
#include "SoundMgr.h"



CMainApp::CMainApp()
	: m_pGraphic_Device(Engine::CGraphic_Device::Get_Instance())
	, m_pGraphicDev(nullptr)
	, m_pComponent_Manager(Engine::CComponent_Manager::Get_Instance())
	, m_pManagement(Engine::CManagement::Get_Instance())
	, m_pSoundMgr(CSoundMgr::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pComponent_Manager->AddRef();
	m_pManagement->AddRef();
}


CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	// 창모드
	if (FAILED(Ready_DefaultSetting(Engine::CGraphic_Device::MODE_WIN/*MODE_FULL*/, g_iBackCX, g_iBackCY)))
		return E_FAIL;

	if (FAILED(Ready_StaticComponent()))
		return E_FAIL;
	
	if (FAILED(Ready_DefaultScene(SCENE_LOGO)))
		return E_FAIL;
	
	m_pSoundMgr->LoadSoundFile();
	m_pSoundMgr->PlayBGM(L"Logo.mp3", CHANNEL_BACKGROUND, 0.7f);

	return NOERROR;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	m_pSoundMgr->UpdateSound();

	if (nullptr == m_pManagement)
		return -1;

	m_fFPS += fTimeDelta;
	

	return m_pManagement->Update_CurrentScene(fTimeDelta);
}

void CMainApp::Render_MainApp(void)
{
	m_iCount++;

	if (m_fFPS > 1.f)
	{
		m_fFPS = 0.f;
		wsprintf(m_szFPS, L"FPS : %d", m_iCount);
		m_iCount = 0;
	}

	if (nullptr == m_pGraphicDev
		|| nullptr == m_pManagement)
		return;

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.0f, 0.f, 1.f, 1.f), 1.0f, 0);

	m_pGraphicDev->BeginScene();

	m_pManagement->Render_CurrentScene();

	m_pGraphic_Device->Get_Font()->DrawTextW(nullptr, m_szFPS, lstrlen(m_szFPS), nullptr, NULL, D3DCOLOR_ARGB(255, 255, 255, 255)); // FPS

	if (m_pScene_Logo != nullptr && g_iScene == 0) // 로딩 상태 출력
	{
		RECT rcLoading;
		rcLoading.left = g_iBackCX * 0.1;
		rcLoading.right = g_iBackCX * 0.1 + 300;
		rcLoading.top = g_iBackCY * 0.8;
		rcLoading.bottom = g_iBackCY * 0.8 + 100;

		m_pGraphic_Device->Get_Font()->DrawTextW(nullptr, m_pScene_Logo->Get_Loading()->Get_LoadingFont(),
			lstrlen(m_pScene_Logo->Get_Loading()->Get_LoadingFont()), &rcLoading, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	
	m_pGraphicDev->EndScene();

	m_pGraphicDev->Present(nullptr, nullptr, 0, nullptr);
}

HRESULT CMainApp::Ready_DefaultSetting(Engine::CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY)
{
	// 장치 초기화
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_GraphicDev(eMode, g_hWnd, iBackCX, iBackCY)))
		return E_FAIL;

	m_pGraphicDev = m_pGraphic_Device->Get_GraphicDev();
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->AddRef();

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 컴포넌트 매니져 초기화
	if (FAILED(m_pComponent_Manager->Reserve_ContainerSize(SCENE_END)))
		return E_FAIL;


	if (FAILED(CObject_Manager::Get_Instance()->Reserve_ContainerSize(SCENE_END)))
		return E_FAIL;



	return NOERROR;
}

HRESULT CMainApp::Ready_StaticComponent(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Shader_Default
	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Default.fx");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Shader_Default", pComponent)))
		return E_FAIL;

	// For.Renderer
	pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Transform
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Transform", pComponent)))
		return E_FAIL;
	
	// For.Buffer_RectTexture
	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectTexture", pComponent)))
		return E_FAIL;

	// for.Component_Buffer_Screen_Texture
	pComponent = CScreen_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_Screen_Texture", pComponent)))
		return E_FAIL;

	// For.Texture_Logo
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Logo/Logo.jpg");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Logo", pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_DefaultScene(SCENE eMode)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene_Logo*			pScene = nullptr;

	switch (eMode)
	{
	case SCENE_LOGO:
		g_iScene = 0;
		pScene = CScene_Logo::Create(m_pGraphicDev, SCENE_STATIC, L"Component_Renderer");
		m_pScene_Logo = pScene;
		break;
	case SCENE_STAGE:
		//pScene = CScene_Logo::Create(m_pGraphicDev);
		break;
	}	

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_CurrentScene(pScene)))
		return E_FAIL;

	return NOERROR;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp *	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MessageBox(0, L"CMainApp Created Failed", nullptr, MB_OK);
		pInstance->Release();
		pInstance = nullptr;
	}
	return pInstance;
}

Engine::_ulong CMainApp::Free(void)
{
	m_pSoundMgr->DestroyInstance();

	Engine::Safe_Release(m_pComponent_Manager);
	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pGraphic_Device);

	return Engine::_ulong();
}
