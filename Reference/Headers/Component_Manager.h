#pragma once

#include "Base.h"
#include "Rect_Texture.h"
#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "Terrain_Buffer.h"
#include "Trail_Buffer.h"
#include "Cube_Texture.h"
#include "Screen_Texture.h"
#include "Picking.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "Collision.h"
#include "Navigation.h"
#include "Shader.h"
#include "Frustum.h"
#include "Information.h"


BEGIN(Engine)

class CComponent;
class DLL_EXPORT CComponent_Manager : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	~CComponent_Manager() = default;
public:
	CComponent* Clone_Component(const _uint & iSceneIdx, const _tchar* pComponentTag);
public:
	HRESULT Add_Component(const _uint& iSceneIdx, const _tchar* pComponentTag, CComponent* pComponent);
	HRESULT Reserve_ContainerSize(const _uint& iSceneCnt);
	void Release_Component(const _uint& iSceneIdx);
private:
	map<const _tchar*, CComponent*>*		m_pMapComponent = nullptr;
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint									m_iReserve_Size = 0;
public:
	CComponent* Find_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
public:
	virtual _ulong Free(void);
};

END