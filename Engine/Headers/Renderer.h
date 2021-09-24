#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class DLL_EXPORT CRenderer : public CComponent
{
public:
	enum RENDERGROUP {RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CRenderer(void) = default;
public:
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	HRESULT Ready_Renderer(void);
	void Render_Renderer(void);
private:
	list<CGameObject*>			m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;
private:
	void Render_Priority(void);
	void Render_NonAlpha(void);
	void Render_Alpha(void);
	void Render_UI(void);
	void Clear_RenderGroup(void);
public:
	virtual CRenderer* Clone(void);
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _ulong Free(void);
};

END