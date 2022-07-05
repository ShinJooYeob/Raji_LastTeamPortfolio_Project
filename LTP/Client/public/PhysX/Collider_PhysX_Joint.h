#pragma once

#include "Collider_PhysX_Base.h"

BEGIN(Client)
class CCollider_PhysX_Joit: public CCollider_PhysX_Base
{
private:
	explicit CCollider_PhysX_Joit(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX_Joit(const CCollider_PhysX_Joit& rhs);
	virtual ~CCollider_PhysX_Joit() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	virtual HRESULT Update_BeforeSimulation(CTransform* objTransform)override;
	virtual HRESULT Update_AfterSimulation(CTransform* objTransform)override;


public:
#ifdef _DEBUG
	virtual HRESULT Render() override;

#endif // _DEBUG


public:
	static CCollider_PhysX_Joit* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};

END