#pragma once

namespace Engine
{
	typedef struct  tagInformation
	{
		float fMaxHP;
		float fHP;
		float fAtt;
		int		iLife;
	}OBJ_INFO;

	typedef struct tagTexture
	{
		LPDIRECT3DTEXTURE9	pTexture;	//�̹��������� �����ϱ� ���� ����.(�̹����� �������� ����)
		D3DXIMAGE_INFO		ImgInfo;	//�̹����� ����(�ܺ�)�� ��´�.
	}TEX_INFO;

	typedef struct tagSprite 
	{
		int iSpriteNum; // ��ü ��������Ʈ �̹��� ��.
		int iCurrentIndex; // ���� ����ؾ� �ϴ� ��������Ʈ �ε���.
		int iFrameCount; // ���� ��������Ʈ�� ����ϰ� ���ӵ� ������ ��.
		int iFrameDelay; // ��������Ʈ ���� �ӵ��� �����ϱ� ���� ������ ������.
	}SPRITE;

	typedef struct InfoTerrainData_Sole
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNormal;
		int iTerrainIndex[5];
	}TERRAIN_DATA_SOLE;

	typedef struct tagCellPoint
	{
		D3DXVECTOR3 vPoint_First;
		D3DXVECTOR3	vPoint_Second;
		D3DXVECTOR3 vPoint_Third;
		D3DXVECTOR3 vPoint_Fourth;

		int		iOption;
		int		iIndex;
	}CELLPOINT, *PCELLPOINT, *pCELLPOINT;


	typedef struct tagOBB
	{
		D3DXVECTOR3		vPoint[8];
		D3DXVECTOR3		vCenterPos;
		D3DXVECTOR3		vProjDir[3];
		D3DXVECTOR3		vAlignAxis[3];
	}OBBINFO;

	typedef struct tagSPHERE
	{
		D3DXVECTOR3		vCenterPos;
		D3DXVECTOR3		vPosToArc;
		float						fRadius;
		int							iSlices;
		int							iStacks;
	}SPHEREINFO;

	typedef struct tagMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPD3DXMESH				pOriginal_Mesh;
		LPDIRECT3DTEXTURE9*		ppTextures;
		unsigned long			dwNumBones;
		D3DXMATRIX**			ppCombinedTransformationMatrices;
		D3DXMATRIX*				pOffsetMatrices;
		D3DXMATRIX*				pRenderingMatrices;
		_vec3					m_vMin, m_vMax;
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagFrame_Derived : public D3DXFRAME
	{
		_matrix			matCombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct tagProjectInfo
	{
		float				fFovY = 0.f;
		float				fAspect = 0.f;
		float				fNear = 0.f;
		float				fFar = 0.f;
	}PROJINFO;

	typedef struct tagCameraInfo
	{
		D3DXVECTOR3			vEye = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3			vAt = D3DXVECTOR3(0.f, 0.f, 0.f);;
		D3DXVECTOR3			vAxisY = D3DXVECTOR3(0.f, 1.f, 0.f);;
	}CAMERAINFO;

	typedef struct tagVertex_Color
	{
		D3DXVECTOR3		vPosition;
		unsigned long	dwColor;
	}VTXCOL;

	typedef struct tagVertex_Normal_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
	}VTXNORTEX;

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct tagVertex_Trail
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTRAIL;


	typedef struct tagVertex_Cube_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;
	}VTXCUBETEX;

	// D3DFVF_XYZ : 
	// D3DFVF_XYZRHW : Viewport�������� ��ǥ��
	typedef struct tagVertex_Screen_Texture
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXSCREENTEX;


	typedef struct tagIndex16
	{
		unsigned short		_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned long		_1, _2, _3;
	}INDEX32;

}