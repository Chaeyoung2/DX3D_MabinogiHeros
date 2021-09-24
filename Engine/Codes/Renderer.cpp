#include "..\Headers\Renderer.h"
#include "GameObject.h"

USING(Engine)

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}


HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (eGroup >= RENDER_END)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

HRESULT CRenderer::Ready_Renderer(void)
{
	return NOERROR;
}

void CRenderer::Render_Renderer(void)
{
	Render_Priority();
	Render_NonAlpha();
	Render_Alpha();
	Render_UI();

	Clear_RenderGroup();
}

void CRenderer::Render_Priority(void)
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		pGameObject->Render_GameObject();
	}
}

void CRenderer::Render_NonAlpha(void)
{
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		pGameObject->Render_GameObject();
	}
}

_bool Compare(CGameObject* pSrc, CGameObject* pDest)
{
	return pSrc->Get_ViewZ() > pDest->Get_ViewZ();
}

void CRenderer::Render_Alpha(void)
{
	m_RenderList[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		pGameObject->Render_GameObject();
	}
}

void CRenderer::Render_UI(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		pGameObject->Render_GameObject();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CRenderer::Clear_RenderGroup(void)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}	
}

CRenderer * CRenderer::Clone(void)
{
	AddRef();

	return this;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer *	pInstance = new CRenderer(pGraphicDev);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		MSG_BOX("CRenderer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CRenderer::Free(void)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}

	return Engine::CComponent::Free();
}
