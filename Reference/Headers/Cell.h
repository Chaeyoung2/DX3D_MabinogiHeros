#pragma once

#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CCell : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum MOVETYPE { MOVE_IN, MOVE_TO_NEIGHBOR, MOVE_OUT };
private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell() = default;
public:
	_vec3 Get_Point(POINT ePoint) const {
		return m_vPoint[ePoint];
	}
	MOVETYPE GetMoveType(const _vec3& vMovedPos, _uint* pCurrentIdx);
	_vec3		GetWallNormal(const _vec3& vMovedPos, _uint* pCurrentIdx);
	void Set_Neighbor(NEIGHBOR eNeighbor, CCell* pCell) {
		m_pNeighbor[eNeighbor] = pCell;
	}
public:
	HRESULT Ready_Cell( _vec3& pPointA,  _vec3& pPointB,  _vec3& pPointC, const _uint& iIndex);
	void Render_Cell(void);
	_bool Check_Neighbor(_vec3 vSrcPoint, _vec3 vDestPoint, CCell* pCell);
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	_vec3				m_vPoint[POINT_END];
	CCell*				m_pNeighbor[NEIGHBOR_END] = { nullptr };
	_vec3				m_vDir[LINE_END];
	_vec2				m_vNormal[LINE_END];
	_uint				m_iIndex;
private: //For.Debug
	LPD3DXLINE			m_pLine = nullptr;
public:
	static CCell* Create(LPDIRECT3DDEVICE9 pGraphicDev,  _vec3& pPointA,  _vec3& pPointB,  _vec3& pPointC,  const _uint& iIndex);
	virtual _ulong Free(void);
};

END