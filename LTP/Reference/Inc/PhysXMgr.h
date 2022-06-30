#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPhysXMgr final : public CBase
{
	DECLARE_SINGLETON(CPhysXMgr)
public:
	CPhysXMgr();
	virtual ~CPhysXMgr() = default;

public:
	HRESULT Initialize_PhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	void TestPhysX();


public:

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;


public:
	virtual void Free() override;
};

END