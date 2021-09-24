#include "stdafx.h"
#include "..\Headers\Loading.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pGraphic_Device(pGraphicDev)
{
	m_pGraphic_Device->AddRef();
	m_pComponent_Manager->AddRef();
}

_uint APIENTRY Thread_Main(void* pArgList)
{
	CLoading*		pLoading = (CLoading*)pArgList;

	EnterCriticalSection(pLoading->Get_Critical_Section());

	switch (pLoading->Get_SceneID())
	{
	case SCENE_STAGE:
		pLoading->Loading_ForStage();
		break;
	case SCENE_BOSS:
		pLoading->Loading_ForBoss();
		break;
	}
	LeaveCriticalSection(pLoading->Get_Critical_Section());

	return 0;
}

HRESULT CLoading::Ready_Loading(SCENE eSceneID)
{
	m_eSceneID = eSceneID;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	return NOERROR;
}

HRESULT CLoading::Loading_ForStage(void)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	CComponent*			pComponent = nullptr;




	lstrcpy(m_szLoading, L"Loading Utilities..");

	// for.Component_Picking
	pComponent = CPicking::Create(m_pGraphic_Device);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Picking", pComponent)))
		return E_FAIL;
	// for.Component_Collision
	pComponent = CCollision::Create(m_pGraphic_Device);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Collision", pComponent)))
		return E_FAIL;
// 	// for.Component_Collision_Sphere
// 	pComponent = CCollision_Sphere::Create(m_pGraphic_Device);
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Collision_Sphere", pComponent)))
// 		return E_FAIL;
	// for.Component_Navigation
	pComponent = CNavigation::Create(m_pGraphic_Device, L"../Bin/Data/Cell_0105.dat");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Navigation", pComponent)))
		return E_FAIL;
	// for.Component_Buffer_Trail
	pComponent = CTrail_Buffer::Create(m_pGraphic_Device);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Trail", pComponent)))
		return E_FAIL;
	// For.Component_Frustum
	pComponent = CFrustum::Create(m_pGraphic_Device);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Frustum", pComponent)))
		return E_FAIL;
	// For.Component_Information(Player)
	OBJ_INFO tPlayerInfo; tPlayerInfo.fAtt = 100.f; tPlayerInfo.fMaxHP = 1000.f; tPlayerInfo.fHP = 1000.f; tPlayerInfo.iLife = 1;
		pComponent = CInformation::Create(m_pGraphic_Device, tPlayerInfo);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Information_Player", pComponent)))
		return E_FAIL;
	// For.Component_Information(Zecallion)
	OBJ_INFO tZecallionInfo; tZecallionInfo.fAtt = 100.f;  tZecallionInfo.fMaxHP = 1500.f; tZecallionInfo.fHP = 1500.f; tZecallionInfo.iLife = 1;
	pComponent = CInformation::Create(m_pGraphic_Device, tZecallionInfo);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Information_Zecallion", pComponent)))
		return E_FAIL;


	lstrcpy(m_szLoading, L"Loading Shaders Files..");
	pComponent = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Terrain", pComponent)))
		return E_FAIL;
	pComponent = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;
	pComponent = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Effect.fx");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Effect", pComponent)))
		return E_FAIL;
	pComponent = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_UI.fx");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_UI", pComponent)))
		return E_FAIL;
	pComponent = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Effect_Multi.fx");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Effect_Multi", pComponent)))
		return E_FAIL;
	pComponent = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_MapObject.fx");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_MapObject", pComponent)))
		return E_FAIL;


	lstrcpy(m_szLoading, L"Loading Buffers..");

	// for.Component_Buffer_Terrain
	// Load.
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(L"../Bin/Data/Terrain_0105.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;
	while (true)
	{
		TERRAIN_DATA_SOLE tTerrainDataSole;
		ReadFile(hFile, &tTerrainDataSole, sizeof(TERRAIN_DATA_SOLE), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		m_vecTerrainDataSole.push_back(tTerrainDataSole);
	}

	pComponent = CTerrain_Buffer::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Terrain/Height.bmp", 129, 257, 1, &m_vecTerrainDataSole);
	if (nullptr == pComponent)
		return E_FAIL;	
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Terrain", pComponent)))
		return E_FAIL;

	// for.Component_Buffer_Cube_Texture
	pComponent = CCube_Texture::Create(m_pGraphic_Device);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Cube_Texture", pComponent)))
		return E_FAIL;




	lstrcpy(m_szLoading, L"Loading Textures..");



	// for.Component_Texture_Environment_Fire
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/Fire/Fire%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_Fire", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_FireB
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/FireB/FireB%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_FireB", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_FlameA
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/FlameA/FlameA%d.tga", 50);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_FlameA", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_FlameB
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/FlameB/FlameB%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_FlameB", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_FlameC
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/FlameC/FlameC%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_FlameC", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_GroundFireA
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/GroundFireA/FireGroundA%d.tga", 25);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_GroundFireA", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_GroundFireB
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/GroundFireB/FireGroundB%d.tga", 26);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_GroundFireB", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Environment_GroundFireC
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Environment/GroundFireC/GroundFireC%d.tga", 21);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Environment_GroundFireC", pComponent)))
		return E_FAIL;


	// for.Component_Texture_RingLine
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/ring_line.tga");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_RingLine", pComponent)))
		return E_FAIL;

	// for.Component_Texture_RingLine
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/ring_outer_wind01.tga");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_RingOuterWind01", pComponent)))
		return E_FAIL;



	// for.Component_Texture_HandEffect
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Hand/%d.png", 35);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_HandEffect", pComponent)))
		return E_FAIL;


	// for.Component_Texture_SmashEffect
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/SmashEffect0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_SmashEffect", pComponent)))
		return E_FAIL;

	// for.Component_Texture_DamageScreen
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/DamageScreen.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_DamageScreen", pComponent)))
		return E_FAIL;

	// for.Component_Texture_NameCard
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/NameCard.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_NameCard", pComponent)))
		return E_FAIL;
	// for.Component_Texture_ClearUI
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/UI_Zecallion_Clear.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_Clear", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Guard0
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/UI_Zecallion_Guard_0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_Gaurd_0", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Guard1
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/UI_Zecallion_Guard_1.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_Gaurd_1", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Zecallion_HPBar_Font
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HpBar/MonsterHpBar_Font.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Zecallion_HPBar_Font", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Player_HPBar1
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HpBar/PlayerHpBar0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Player_HPBar1", pComponent)))
		return E_FAIL;

	//for.Component_Texture_Player_HPBar2
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HpBar/PlayerHpBar1.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Player_HPBar2", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Zecallion_HPBar1
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HpBar/MonsterHpBar0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Zecallion_HPBar1", pComponent)))
		return E_FAIL;

	//for.Component_Texture_Zecallion_HPBar2
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HpBar/MonsterHpBar1.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Zecallion_HPBar2", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Terrain
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/Terrain0.bmp");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Terrain1
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/Terrain1.bmp");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain_1", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Terrain3
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/Terrain3.bmp");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain_3", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Terrain_Filter
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/TerrainAlphaTexture0.bmp");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain_Filter", pComponent)))
		return E_FAIL;


	// for.Component_Texture_Effect (Resonance)
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Skill/Resonance/0.png", 1, 882, 843);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Resonance_0", pComponent)))
		return E_FAIL;
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Skill/Resonance/1.png", 1, 524, 526);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Resonance_1", pComponent)))
		return E_FAIL;
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Skill/Resonance/2.png", 1, 271, 271);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Resonance_2", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Effect(Speedy_Move)
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Speedy_Move/aming_point_big05.tga");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Speedy_Move", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Effect(blood)
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Blood_Ani/BloodAni_1/%d.png", 9);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_1", pComponent)))
		return E_FAIL;
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Blood_Ani/BloodAni_2/%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_2", pComponent)))
		return E_FAIL;
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Blood_Ani/BloodAni_3/%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Blood_3", pComponent)))
		return E_FAIL;

	// for.Component_Texture_Effect_HitEffect0
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HitEffect0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_HitEffect0", pComponent)))
		return E_FAIL;
	// for.Component_Texture_Effect_HitEffect1
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/HitEffect1.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_HitEffect1", pComponent)))
		return E_FAIL;


	// for.Component_Texture_Trail_LongBlade00
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Trail/heroes_longblade_trail00.tga");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Trail_LongBlade00", pComponent)))
		return E_FAIL;

	// for.Component_Texture_SkyBox
// 	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/SkyBox%d.dds", 4);
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent)))
// 		return E_FAIL;
// 	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/SkyBox5.dds");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent)))
// 		return E_FAIL;
	pComponent = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/SkyBox0.dds");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent)))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loading Mesh..");

	// for.Component_Mesh_Player
// 	pComponent = CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/DynamicMesh/Hurk/", L"Hurk.x");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Player", pComponent)))
// 		return E_FAIL;
	pComponent = CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/DynamicMesh/Arisha/", L"Arisha.x"); // Arisha
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Player", pComponent)))
		return E_FAIL;

	// for.Component_Mesh_GreatSword
// 	pComponent = CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/DynamicMesh/GreatSword/", L"GREATSWORD.x");
// 	if (nullptr == pComponent)
// 		return E_FAIL;
// 	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_GreatSword", pComponent)))
// 		return E_FAIL;
	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Blade/", L"Blade.x"); // Arisha's Blade
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Blade", pComponent)))
		return E_FAIL;


	// for.Component_Mesh_Zecallion
	pComponent = CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/DynamicMesh/Zecallion/", L"Zecallion.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Zecallion", pComponent)))
		return E_FAIL;

	// for.Component_Mesh_ZecallionAxe
	pComponent = CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/DynamicMesh/ZecallionAxe/", L"ZecallionAxe.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_ZecallionAxe", pComponent)))
		return E_FAIL;


#ifdef CREATE_OBJ
	// for.Component_Mesh_MapObject ///////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mesh (Ex)
	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Box/", L"Box_00.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Box_00", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Box/", L"Box_01.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Box_01", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Box/", L"Box_02.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Box_02", pComponent)))
		return E_FAIL; // Box

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/CampFire/", L"CampFire.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_CampFire", pComponent)))
		return E_FAIL; // CampFire


	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Door/", L"Door_0.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Door_00", pComponent)))
		return E_FAIL;


	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Door/", L"Door_1.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Door_01", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Door/", L"Door_2.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Door_02", pComponent)))
		return E_FAIL; // Door

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Furniture/", L"Furniture01.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Furniture_01", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Furniture/", L"Furniture02.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Furniture_02", pComponent)))
		return E_FAIL; // Furniture

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/House/", L"House01.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_House_01", pComponent)))
		return E_FAIL; // House


	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar0.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_00", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar1.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_01", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar2.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_02", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar3.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_03", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar4.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_04", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar5.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_05", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar6.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_06", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Pillar/", L"Pillar7.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Pillar_07", pComponent)))
		return E_FAIL; // Pillar

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Rock/", L"FireRock.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_FireRock", pComponent)))
		return E_FAIL; // FireRock

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Rock/", L"Rock.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Rock", pComponent)))
		return E_FAIL; // Rock

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Ship/", L"Ship.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Ship", pComponent)))
		return E_FAIL; // Ship

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_00.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_00", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_01.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_01", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_02.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_02", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_03.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_03", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_04.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_04", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_05.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_05", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_06.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_06", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_07.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_07", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_08.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_08", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_09.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_09", pComponent)))
		return E_FAIL;


	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_10.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_10", pComponent)))
		return E_FAIL;


	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_11.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_11", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_12.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_12", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_13.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_13", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_14.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_14", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Tree/", L"Tree_15.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Tree_15", pComponent)))
		return E_FAIL; // Tree


	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_00.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_00", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_01.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_01", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_02.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_02", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_03.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_03", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_04.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_04", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_05.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_05", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_06.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_06", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_07.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_07", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_08.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_08", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_09.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_09", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_10.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_10", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_11.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_11", pComponent)))
		return E_FAIL;

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Weed/", L"Weed_12.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weed_12", pComponent)))
		return E_FAIL; // Weed

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Torch/", L"Torch.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Torch_0", pComponent)))
		return E_FAIL; // Torch0

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Torch/", L"Torch1.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Torch_1", pComponent)))
		return E_FAIL; // Torch1

	pComponent = CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Mesh/StaticMesh/Torch/", L"Torch2.x");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Torch_2", pComponent)))
		return E_FAIL; // Torch2


#endif


	lstrcpy(m_szLoading, L"Loading Success!");

	m_isFinish = true;

	return NOERROR;
}

HRESULT CLoading::Loading_ForBoss(void)
{
	return NOERROR;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE eSceneID)
{
	CLoading *	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eSceneID)))
	{
		MessageBox(0, L"CLoading Created Failed", nullptr, MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CLoading::Free(void)
{
	

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pGraphic_Device);
	Engine::Safe_Release(m_pComponent_Manager);

	return _ulong();
}
