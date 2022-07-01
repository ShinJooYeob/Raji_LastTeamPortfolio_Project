#pragma once
#include "GameObject.h"

BEGIN(Client)

class CMonster_Bullet abstract : public CGameObject
{
protected:
	explicit CMonster_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Bullet(const CMonster_Bullet& rhs);
	virtual ~CMonster_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT	Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END