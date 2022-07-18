#pragma once

#include "GameObject.h"

BEGIN(Client)

class CTriggerObject abstract : public CGameObject
{
public:
	enum EParkourTriggerType { PACUR_LEDGE, PACUR_JUMP, PACUR_END };

protected:
	CTriggerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTriggerObject(const CTriggerObject& rhs);
	virtual ~CTriggerObject() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	virtual _int					Update(_double fDeltaTime) override;
	virtual _int					LateUpdate(_double fDeltaTimer) override;
	virtual _int					Render() override;
	virtual _int					LateRender() override;

	virtual _int					Active_Trigger(CGameObject* pTarget, _double fDeltaTime) = 0;
	virtual _int					DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime) = 0;

	virtual EParkourTriggerType 	Get_ParkourTriggerType();

public:
	virtual CGameObject*		Clone(void* pArg)PURE;
	virtual void				Free() override;
};
END
