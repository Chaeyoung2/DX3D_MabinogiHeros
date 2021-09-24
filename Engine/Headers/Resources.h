#pragma once

#include "Engine_Defines.h"
#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CResources : public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResources(const CResources& rhs);
	virtual ~CResources();
public:
	virtual CComponent* Clone(void) = 0;
protected:
	virtual _ulong Free(void) override;
};

_END