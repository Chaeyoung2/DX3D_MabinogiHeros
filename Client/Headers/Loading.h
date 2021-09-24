#pragma once

#include "Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
#include "Component_Manager.h"

// 각씬에서 필요한 자원들을 로드한다.
class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading() = default;
public:
	const _tchar* Get_LoadingFont(void) {
		return m_szLoading;}

	_bool Get_Finish(void) {
		return m_isFinish;}

	SCENE Get_SceneID(void) {
		return m_eSceneID;}
	CRITICAL_SECTION* Get_Critical_Section(void) {
		return &m_Critical_Section;}
public:

	HRESULT Ready_Loading(SCENE eSceneID);
	HRESULT Loading_ForStage(void);
	HRESULT Loading_ForBoss(void);
private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_Critical_Section;
	SCENE					m_eSceneID;
	_tchar					m_szLoading[128] = L"";
	_bool					m_isFinish = false;
	vector<TERRAIN_DATA_SOLE> m_vecTerrainDataSole;
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE eSceneID);
	virtual _ulong Free(void);
};

