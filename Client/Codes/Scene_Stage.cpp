#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"

#include "Object_Manager.h"
#include "Terrain.h"
#include "Player.h"
#include "Effect.h"
#include "UI.h"
#include "SkyBox.h"
#include "GreatSword.h"
#include "Map_Object.h"
#include "Zecallion.h"
#include "ZecallionAxe.h"
#include "Blade.h"
#include "Door.h"
#include "Zecallion_HPBar.h"
#include "Player_HPBar.h"
#include "Zecallion_HPBar2.h"
#include "Player_HPBar2.h"
#include "Zecallion_HPBar_Font.h"
#include "Fire.h"
#include "HandEffect.h"
#include "SoundMgr.h"

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

HRESULT CScene_Stage::Ready_Scene(const _uint& eSceneID, const _tchar* pRendererTag)
{
	SOUNDMGR->StopSound(CHANNEL_BACKGROUND);
	SOUNDMGR->PlayBGM(L"Zecallion.mp3", CHANNEL_BACKGROUND, 0.3f);

	if (CScene::Ready_Scene(eSceneID, pRendererTag))
		return E_FAIL;

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

#ifdef CREATE_OBJ
	if (FAILED(Ready_Layer_MapObject(L"Layer_MapObject")))
		return E_FAIL;
#endif
	if (FAILED(Ready_Layer_Zecallion(L"Layer_Zecallion")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Door(L"Layer_Door")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(L"Layer_Effect")))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_HPBar")))
		return E_FAIL;
	if (FAILED(Ready_Layer_Fire(L"Layer_Fire")))
		return E_FAIL;


	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	if (iExitCode = CScene::Update_Scene(fTimeDelta))
		return iExitCode;	

	return iExitCode;
}

_int CScene_Stage::LastUpdate_Scene(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	if (iExitCode = CScene::LastUpdate_Scene(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

void CScene_Stage::Render_Scene(void)
{
	CScene::Render_Scene();

	//fkdajflkjdaklfaslflkdaklfjkdaljf
}

// 조명에 대한 정보를 셋팅한다.
HRESULT CScene_Stage::Ready_LightInfo(void)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetLight(0, &LightInfo); // 빛의 정보를 장치에 저장시킨다.
	m_pGraphicDev->LightEnable(0, TRUE); // 저장시킨 빛들 중에서 특정 인덱스의 빛을 활성화시킨다.

	

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Player(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;

	// For.Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

	// For.Sword
// 	pGameObject = CGreatSword::Create(m_pGraphicDev);
// 	if (nullptr == pGameObject)
// 		return E_FAIL;
// 	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

	// For.Blade
	pGameObject = CBlade::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

	// For.HandEffect

	pGameObject = CHandEffect::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);


	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;


	// For.Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

	// For.Skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);
	
	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;
 
 	// For.Effect (test)

//  		pGameObject = CEffect_Blood::Create(m_pGraphicDev, CEffect_Blood::BONE_LEFT_CALF, 1);
//  		if (nullptr == pGameObject)
// 			return E_FAIL;
// 		m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

		return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;



	pGameObject = CPlayer_HPBar::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);


	pGameObject = CPlayer_HPBar2::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);



// 	pGameObject = CUI::Create(m_pGraphicDev);
// 	if (nullptr == pGameObject)
// 		return E_FAIL;
// 	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);	

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_MapObject(const _tchar * pLayerTag)
{
	HANDLE hFile = CreateFile(L"../Bin/Data/MapObject_0105.dat", GENERIC_READ, 0, NULL
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("We Failed To Load Map_Object");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		MAPOBJ* pMapObj = new MAPOBJ;

		// 파일 입력 함수
		ReadFile(hFile, pMapObj, sizeof(MAPOBJ), &dwByte, NULL);

		// 기록한 byte가 0이면 더이상 읽을게 없다!!!
		if (0 == dwByte)
			break;

		if(pMapObj->byDrawID != 5)
			pGameObject = CMap_Object::Create(m_pGraphicDev, pMapObj->vScale, pMapObj->vRot, pMapObj->byOption, pMapObj->byDrawID, pMapObj->vPos);

		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Zecallion(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;

	// For.Zecallion
	pGameObject = CZecallion::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

 	// For.ZecallionAxe
 	pGameObject = CZecallionAxe::Create(m_pGraphicDev);
 	if (nullptr == pGameObject)
 		return E_FAIL;
 	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Door(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	CGameObject*	pGameObject = nullptr;

	// For.Door
	pGameObject = CDoor::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Fire(const _tchar * pLayerTag)
{
	HANDLE hFile = CreateFile(L"../Bin/Data/Fire.dat", GENERIC_READ, 0, NULL
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("We Failed To Load Fire");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		MAPOBJ* pMapObj = new MAPOBJ;

		// 파일 입력 함수
		ReadFile(hFile, pMapObj, sizeof(MAPOBJ), &dwByte, NULL);

		// 기록한 byte가 0이면 더이상 읽을게 없다!!!
		if (0 == dwByte)
			break;

		if (pMapObj->byDrawID == 0)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 0, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}

		if (pMapObj->byDrawID == 1)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 1, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}

		if (pMapObj->byDrawID == 2)
		{
// 			pGameObject = CFire::Create(m_pGraphicDev, 2, pMapObj->vPos);
// 			if (nullptr == pGameObject)
// 				return E_FAIL;
// 			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
// 				return E_FAIL;
		}

		if (pMapObj->byDrawID == 3)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 3, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}
		if (pMapObj->byDrawID == 4)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 4, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}
		if (pMapObj->byDrawID == 5)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 5, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}
		if (pMapObj->byDrawID == 6)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 6, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}
		if (pMapObj->byDrawID == 7)
		{
			pGameObject = CFire::Create(m_pGraphicDev, 7, pMapObj->vPos);
			if (nullptr == pGameObject)
				return E_FAIL;
			if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, pLayerTag, pGameObject)))
				return E_FAIL;
		}


	}

	return NOERROR;
}


CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& eSceneID, const _tchar* pRendererTag)
{
	CScene_Stage *	pInstance = new CScene_Stage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(eSceneID, pRendererTag)))
	{
		MessageBox(0, L"CScene_Stage Created Failed", nullptr, MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}
_ulong CScene_Stage::Free(void)
{
	return Engine::CScene::Free();
}
