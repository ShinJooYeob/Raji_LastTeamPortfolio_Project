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

	_float4x4 Get_ValueMat() {	return m_fValueMat;	};
	void Set_ValueMat(_float4x4* pMat){memcpy(&m_fValueMat, pMat, sizeof(_float4x4));};

	void Set_eNumberNObjectID(_uint eNumber, _uint eObjectID)
	{
		m_eNumber = eNumber; m_eObjectID = eObjectID;
	}
	_uint Get_eNumber() { return m_eNumber; }
	_uint Get_eObjectID() { return m_eObjectID; }


public:
	virtual _int					Update(_double fDeltaTime) override;
	virtual _int					LateUpdate(_double fDeltaTimer) override;
	virtual _int					Render() override;
	virtual _int					LateRender() override;

	virtual _int					Active_Trigger(CGameObject* pTarget, _double fDeltaTime) = 0;
	virtual _int					DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime) = 0;

	virtual EParkourTriggerType 	Get_ParkourTriggerType();


protected:
	CTransform*						m_pTransformCom	 = nullptr;
	CCollider*						m_pColliderCom	= nullptr;
	_float4x4						m_fValueMat = XMMatrixIdentity();

private:
	_uint							m_eNumber = 0;
	_uint							m_eObjectID = Prototype_Trigger_ChangeCameraView;
public:
	virtual CGameObject*		Clone(void* pArg)PURE;
	virtual void				Free() override;
};
END
