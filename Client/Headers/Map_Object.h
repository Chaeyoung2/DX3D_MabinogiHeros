
#include "Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMesh_Static;
	class CRenderer;
	class CCollision;
	class CShader;
	class CFrustum;
}


class CMap_Object : public CGameObject
{
public:
	explicit CMap_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMap_Object() = default;
public:
	virtual HRESULT Ready_GameObject(_vec3 vecScale, _vec3 vecRot, _int iOption, _int iDrawID, _vec3 VecPos);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CMesh_Static*			m_pMeshCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CFrustum*				m_pFrustumCom = nullptr;
private:
	D3DMATERIAL9			m_MtrlInfo;
	_float		m_fAngle = 0.f;
	_int		m_iDrawID = 0;
	_int		m_iOption = 0;
	_bool					m_isRender = true;
public:
	static CMap_Object* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vecScale, _vec3 vecRot, _int iOption, _int iDrawID, _vec3 VecPos);
	virtual _ulong Free(void);
};

