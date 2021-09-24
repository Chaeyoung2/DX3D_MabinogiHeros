#pragma once

#include "Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

namespace Engine
{
	class CComponent_Manager;
	class CManagement;
}

class CSoundMgr;
class CScene_Logo;
class CMainApp final : public Engine::CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();
public:
	HRESULT Ready_MainApp(void);
	// _int : �� ������ �������� �������� ���� ����
	// _float : �ܺο��� �����س��� Ÿ�ӵ�Ÿ�� �޾ƿ´�. (�ٸ� ��ü�鿡�� ����)
	_int Update_MainApp(const _float& fTimeDelta); 
	void Render_MainApp(void);
private:
	Engine::CGraphic_Device*		m_pGraphic_Device = nullptr;
	Engine::CComponent_Manager*		m_pComponent_Manager = nullptr;
	Engine::CManagement*			m_pManagement = nullptr;
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	CScene_Logo*						m_pScene_Logo = nullptr;
	CSoundMgr*						m_pSoundMgr = nullptr;
private:
	HRESULT	Ready_DefaultSetting(Engine::CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY);	
	HRESULT Ready_StaticComponent(void);
	HRESULT Ready_DefaultScene(SCENE eMode);
public:
	static CMainApp* Create(void);	
private:
	_int		m_iCount = 0;
	_float		m_fTime = 0.f;
	_float		m_fFPS = 0.f;
	TCHAR		m_szFPS[128] = L"";
public:
	virtual _ulong Free(void) final;
};

