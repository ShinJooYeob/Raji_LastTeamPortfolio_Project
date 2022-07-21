#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;
class CCamera_Main;
class CTrigger_ChangeCameraView final : public CTriggerObject
{
public:
	enum EChangeCameraViewType { TYPE_FIX, TYPE_FIX_SWITCH, TYPE_TWO_INTERP };

	typedef struct tagChangeCameraViewDesc
	{
		EChangeCameraViewType		eChangeCameraViewType;
		_float3						fMain_Pos;
		_float3						fMain_CamPos;
		_float3						fMain_CamLook;
		_float						fMain_CamMoveWeight;
		_float						fMain_CamLookWeight;
		_bool						bMainLockCamLook;
		_float						fMainMaxTargetArmLength;
		_float						fMainMinTargetArmLength;

		_float3						fSub_Pos;
		_float3						fSub_CamPos;
		_float3						fSub_CamLook;
		_float						fSub_CamMoveWeight;
		_float						fSub_CamLookWeight;
		_bool						bSubLockCamLook;

		_float						fSubMaxTargetArmLength;
		_float						fSubMinTargetArmLength;
	}CHANGECAMERAVIEWDESC;

protected:
	CTrigger_ChangeCameraView(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTrigger_ChangeCameraView(const CTrigger_ChangeCameraView& rhs);
	virtual ~CTrigger_ChangeCameraView() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int				Update(_double fDeltaTime) override;
	virtual _int				LateUpdate(_double fDeltaTimer) override;
	virtual _int				Render() override;
	virtual _int				LateRender() override;

	virtual _int				Active_Trigger(CGameObject* pTarget, _double fDeltaTime) override;
	virtual _int				DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime) override;

	virtual HRESULT				After_Initialize() override;

	virtual void				CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	_bool				Check_CollisionToPlayer();

private:
	// Active Method
	_int				Change_CameraView_Fix(_double fDeltaTime);
	_int				Change_CameraView_FixSwitch(_double fDeltaTime);
	_int				Change_CameraView_TwoPoint_Interp(_double fDeltaTime);

	// DeActive Method
	_int				ChangeBack_CameraView(_double fDeltaTime);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_PlayerAndCameraInfo();
	HRESULT				SetUp_EtcInfo();

private:
	_bool						m_bStoredPreCameraInfo = false;
	_bool						m_bPreCameraLookLockState = false;
	_float						m_fDist_MainToSub = 0.f;

	_float						m_fPreCameraMoveWeight = 0.f;
	_float						m_fPreCameraLookWeight = 0.f;

private:
	CPlayer*					m_pPlayer = nullptr;
	CCamera_Main*				m_pMainCamera = nullptr;
	CTransform*					m_pPlayerTransform = nullptr;

	CHANGECAMERAVIEWDESC		m_tChangeCameraViewDesc;
	CTransform*					m_pTransformCom_Main = nullptr;
	CTransform*					m_pTransformCom_Sub = nullptr;

	CCollider*					m_pCollider_DEBUG = nullptr;

public:
	static CTrigger_ChangeCameraView*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END