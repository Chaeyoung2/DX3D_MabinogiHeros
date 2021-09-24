#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"

#include "Object_Manager.h"
#include "Back_Logo.h"
#include "Dynamic_Camera.h"
#include "Input_Device.h"
#include "Scene_Stage.h"
#include "Management.h"
#include "Component_Manager.h"
#include "Loading.h"
#include "Static_Camera.h"


CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
	, m_pLoading(nullptr)
{

}

HRESULT CScene_Logo::Ready_Scene(const _uint& eSceneID, const _tchar* pRendererTag)
{
	if (CScene::Ready_Scene(eSceneID, pRendererTag))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;


	m_pLoading = CLoading::Create(m_pGraphicDev, SCENE_STAGE);
	if (nullptr == m_pLoading)
		return E_FAIL;



	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	//SetWindowText(g_hWnd, m_pLoading->Get_LoadingFont());

	//m_pGraphicDev->Get_Font()->DrawTextW(nullptr, m_szFPS, lstrlen(m_szFPS), nullptr, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (iExitCode = CScene::Update_Scene(fTimeDelta))
		return iExitCode;	

	

	return iExitCode;
}

_int CScene_Logo::LastUpdate_Scene(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	if (iExitCode = CScene::LastUpdate_Scene(fTimeDelta))
		return iExitCode;

	if (CInput_Device::Get_Instance()->Get_DIKState(DIK_RETURN) & 0x80)
	{
		g_iScene = 1;

		if (false == m_pLoading->Get_Finish())
			return iExitCode;

		CScene_Stage*	pScene_Stage = CScene_Stage::Create(m_pGraphicDev, SCENE_STATIC, L"Component_Renderer");
		if (nullptr == pScene_Stage)
			return -1;
		if (FAILED(CManagement::Get_Instance()->SetUp_CurrentScene(pScene_Stage)))
			return -1;	

		g_iScene = SCENE_STAGE;

		return iExitCode;
	}
	return iExitCode;
}

void CScene_Logo::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;

	// For.Dynamic_Camera
	CAMERAINFO		CamInfo;
	CamInfo.vEye = _vec3(0.f, 3.f, -1.f);
	CamInfo.vAt = _vec3(0.f, 1.f, 0.f);	
	
	pGameObject = CDynamic_Camera::Create(m_pGraphicDev, &CamInfo);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;

	// For.Static_Camera
	pGameObject = CStatic_Camera::Create(m_pGraphicDev, &CamInfo);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;

	// For.Back_Logo
	pGameObject = CBack_Logo::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return E_FAIL;
	
	return NOERROR;
}

HRESULT CScene_Logo::Ready_Navigation(void)
{

	// 1.
	// 점을 만들자.
	_vec3		vPoint[3];

	HANDLE			hFile;
	_ulong			dwByte = 0;
// 
// 	hFile = CreateFile(L"../Bin/Data/Cell_0105.dat", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
// 
//  	vPoint[0] = _vec3(0.f, 0.f, 10.f);
//  	vPoint[1] = _vec3(10.f, 0.f, 0.f);
//  	vPoint[2] = _vec3(0.f, 0.f, 0.f);
//  	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
//  
//  	vPoint[0] = _vec3(0.f, 0.f, 10.f);
//  	vPoint[1] = _vec3(10.f, 0.f, 10.f);
//  	vPoint[2] = _vec3(10.f, 0.f, 0.f);
//  	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
//  
//  	vPoint[0] = _vec3(10.f, 0.f, 10.f);
//  	vPoint[1] = _vec3(20.f, 0.f, 0.f);
//  	vPoint[2] = _vec3(10.f, 0.f, 0.f);
//  	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
//  
//  	vPoint[0] = _vec3(0.f, 0.f, 20.f);
//  	vPoint[1] = _vec3(10.f, 0.f, 10.f);
//  	vPoint[2] = _vec3(0.f, 0.f, 10.f);
//  	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

	// 	// 2.
	// 	vPoint[0] = _vec3(0.f, 0.f, 1.f);
	// 	vPoint[1] = _vec3(1.f, 0.f, 0.f);
	// 	vPoint[2] = _vec3(0.f, 0.f, 0.f);
	// 	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
	// 
	// 	vPoint[0] = _vec3(0.f, 0.f, 1.f);
	// 	vPoint[1] = _vec3(1.f, 0.f, 1.f);
	// 	vPoint[2] = _vec3(1.f, 0.f, 0.f);
	// 	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
	// 
	// 	vPoint[0] = _vec3(1.f, 0.f, 1.f);
	// 	vPoint[1] = _vec3(2.f, 0.f, 0.f);
	// 	vPoint[2] = _vec3(1.f, 0.f, 0.f);
	// 	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
	// 
	// 	vPoint[0] = _vec3(1.f, 0.f, 1.f);
	// 	vPoint[1] = _vec3(2.f, 0.f, 1.f);
	// 	vPoint[2] = _vec3(2.f, 0.f, 0.f);
	// 	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);


	// 1.
// 	 	for (int z = 0; z < 1; ++z)
// 	 	{
// 	 		for (int x = 0; x < 1; ++x)
// 	 		{
// 	 			vPoint[0] = _vec3(0.f + (float)x, 0.f, 1.f + (float)z);
// 	 			vPoint[1] = _vec3(1.f + (float)x, 0.f, 0.f + (float)z);
// 	 			vPoint[2] = _vec3(0.f + (float)x, 0.f, 0.f + (float)z);
// 	 			WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
// 	 
// 	 			vPoint[0] = _vec3(0.f + (float)x, 0.f, 1.f + (float)z);
// 	 			vPoint[1] = _vec3(1.f + (float)x, 0.f, 1.f + (float)z);
// 	 			vPoint[2] = _vec3(1.f + (float)x, 0.f, 0.f + (float)z);
// 	 			WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
	 
	 // 			vPoint[0] = _vec3(1.f, 0.f, 1.f);
	 // 			vPoint[1] = _vec3(2.f, 0.f, 0.f);
	 // 			vPoint[2] = _vec3(1.f, 0.f, 0.f);
	 // 			WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
	 // 
	 // 			vPoint[0] = _vec3(0.f, 0.f, 2.f);
	 // 			vPoint[1] = _vec3(1.f, 0.f, 1.f);
	 // 			vPoint[2] = _vec3(0.f, 0.f, 1.f);
	 // 			WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
// 	 		}
// 	 	}


	CloseHandle(hFile);

	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& eSceneID, const _tchar* pRendererTag)
{
	CScene_Logo *	pInstance = new CScene_Logo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(eSceneID, pRendererTag)))
	{
		MessageBox(0, L"CScene_Logo Created Failed", nullptr, MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}
_ulong CScene_Logo::Free(void)
{
	Safe_Release(m_pLoading);

	CObject_Manager::Get_Instance()->Release_GameObject(SCENE_LOGO);
	CComponent_Manager::Get_Instance()->Release_Component(SCENE_LOGO);
	

	return Engine::CScene::Free();
}
