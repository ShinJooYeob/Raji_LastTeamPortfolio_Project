#pragma once

#include "GameObject.h"

BEGIN(Client)

class CTriggerObject abstract : public CGameObject
{
public:
	enum EParkourTriggerType { PACUR_LEDGE, PACUR_JUMP, PACUR_CHANGE_NAV_INDEX, PACUR_PILLAR, BLOCK_ZONE, DEAD_ZONE, TRIGGER_CHANGE_CAMERA_VIEW,INSTANCE_MONSTER, PACUR_END };

protected:
	CTriggerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTriggerObject(const CTriggerObject& rhs);
	virtual ~CTriggerObject() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual HRESULT After_Initialize() { return S_OK; };


	_float4x4 Get_ValueMat() { return m_fValueMat; };
	void Set_ValueMat(_float4x4* pMat) { memcpy(&m_fValueMat, pMat, sizeof(_float4x4)); };
	_float4x4 Get_SubValueMat() {	return m_fSubValueMat;	};
	void Set_SubValueMat(_float4x4* pMat){memcpy(&m_fSubValueMat, pMat, sizeof(_float4x4));};

	void Set_eNumberNObjectID(_uint eNumber, const _tchar* eObjectID)
	{
		m_eNumber = eNumber; m_eObjectID = eObjectID;
	}
	_uint Get_eNumber() { return m_eNumber; }
	const _tchar* Get_eObjectID() { return m_eObjectID.c_str(); }


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
	_float4x4						m_fSubValueMat = XMMatrixIdentity();

private:
	_uint							m_eNumber = 0;
	wstring							m_eObjectID = TAG_OP(Prototype_Trigger_ChangeCameraView);
public:
	virtual CGameObject*		Clone(void* pArg)PURE;
	virtual void				Free() override;
};
END
