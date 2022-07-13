#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;
class CCamera_Main;

class CTestLedgeTrigger final : public CTriggerObject
{
public:
	enum ELedgeTriggerState { STATE_START, STATE_LEDGE, STATE_LAST_LEDGE, STATE_END };

	typedef struct tagLedgeTriggerDesc
	{
		_float3					fSpawnPos;
		ELedgeTriggerState		eLedgeTriggerState;
	}LEDGETRIGGERDESC;

protected:
	CTestLedgeTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestLedgeTrigger(const CTestLedgeTrigger& rhs);
	virtual ~CTestLedgeTrigger() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int					Update(_double fDeltaTime) override;
	virtual _int					LateUpdate(_double fDeltaTimer) override;
	virtual _int					Render() override;
	virtual _int					LateRender() override;

	virtual _int					Active_Trigger(_double fDeltaTime) override;
	virtual _int					DeActive_Trigger(_double fDeltaTime) override;

	virtual EParkourTriggerType 	Get_ParkourTriggerType();

public:
	ELedgeTriggerState				Get_LedgeType();

public:
	void				Ledge_Start();
	void				Ledge_Hanging();
	void				Ledge_LastHanging();
	void				Ledge_End();


public:
	_bool				Check_CollisionToPlayer();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_PlayerAndCameraInfo();
	HRESULT				SetUp_EtcInfo();

private:
	ELedgeTriggerState			m_eLedgeTriggerType;
	_bool						m_bOnTriggered = false;
	_bool						m_bTurnPoint = false;

private:
	CTransform*					m_pTransformCom = nullptr;
	CPlayer*					m_pPlayer = nullptr;
	CTransform*					m_pPlayerTransform = nullptr;

	// for Debug
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;

public:
	static CTestLedgeTrigger*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END