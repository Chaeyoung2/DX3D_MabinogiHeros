#pragma once

#include "Mesh.h"

BEGIN(Engine)

class CHierarchyLoader;
class CAnimationCtrl;
class DLL_EXPORT CMesh_Dynamic : public CMesh
{
private:
	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual ~CMesh_Dynamic() = default;
public:
	virtual const _vec3* Get_Min(const _uint& iMeshIdx = 0) const;
	virtual const _vec3* Get_Max(const _uint& iMeshIdx = 0) const;
	_double Get_TrackPosition();
	_double Get_Period();
	_uint		Get_AnimationIdx();
	void SetTrackPos(_double dTrackPos);

public:
	const _matrix* Get_BoneMatrix(const char* pBoneName)
	{
		D3DXFRAME_DERIVED* pFrame = ((D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName));
		return &pFrame->matCombinedTransformationMatrix;

	}
public:
	HRESULT Ready_Mesh_Dynamic(const _tchar* pFilePath, const _tchar* pFileName);
	virtual void Render_Mesh(LPD3DXEFFECT pEffect = nullptr);
	void Set_AnimationSet(const _uint& iIndex, _double dMagicNumber);
	void Play_AnimationSet(const _float& fTimeDelta);
private:
	D3DXFRAME*									m_pRootFrame = nullptr;
	CHierarchyLoader*							m_pLoader = nullptr;
	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;
	typedef list<D3DXMESHCONTAINER_DERIVED*>	MESHCONTAINERLIST;
	CAnimationCtrl*								m_pAnimationCtrl = nullptr;
	_uint													m_iAnimationIdx = 0;
public:
	void Update_CombiendTransformationMatrices(D3DXFRAME_DERIVED* pFrame, _matrix matParent);
	void SetUp_CombinedMatricesPointer(D3DXFRAME_DERIVED* pFrame);
public:
	static CMesh_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void);
	virtual _ulong Free(void);
};

END

