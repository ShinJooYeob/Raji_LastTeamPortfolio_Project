#pragma once

#include "Component.h"

BEGIN(Client)

class CCollider_PhysX : public CComponent
{
public:

private:
	explicit CCollider_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX(const CCollider_PhysX& rhs);
	virtual ~CCollider_PhysX() = default;
private:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

public:



public:
	static CCollider_PhysX* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};

END