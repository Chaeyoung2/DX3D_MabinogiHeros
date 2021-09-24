#pragma once

typedef struct tagMapObject
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vScale;
	D3DXVECTOR3 vRot;

	float				fAngle;
	int					byDrawID;
	int					byOption;
}MAPOBJ, *PMAPOBJ, *pMAPOBJ;

