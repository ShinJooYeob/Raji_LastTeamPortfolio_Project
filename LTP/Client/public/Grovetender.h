#pragma once

#include "GameObject.h"

BEGIN(Client)
class CPlayer;
class CCamera_Main;
class CGrovetender final : public CGameObject
{
public:
	enum EGOLEM_STATE {
		STATE_SPAWNIDLE, STATE_SPAWN, STATE_IDLE, STATE_MOVE, STATE_END
	};

	enum EGOLEM_ANIM {
		ANIM_SPAWN_IDLE, ANIM_SPAWN, ANIM_IDLE, ANIM_MOVE, ANIM_END
	};

private:
	explicit CGrovetender(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGrovetender(const CGrovetender& rhs);
	virtual ~CGrovetender() = default;

public:
	virtual HRESULT				Initialize_Prototype(void* pArg) override;
	virtual HRESULT				Initialize_Clone(void* pArg) override;


public:
	virtual _int				Update(_double fDeltaTime) override;
	virtual _int				LateUpdate(_double fDeltaTimer) override;
	virtual _int				Render() override;
	virtual _int				LateRender() override;


	virtual void				CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
								_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public: /* Damage Logic*/
	virtual _float				Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;
	_float						Apply_Damage(CGameObject* pTargetObject, _float fDamageAmount, _bool bKnockback);

public:
	void						Set_CutSceneState(_uint iCutSceneState);

private:
	HRESULT						Update_State_SpawnIdle(_double fDeltaTime);
	HRESULT						Update_State_Spawn(_double fDeltaTime);
	HRESULT						Update_State_Idle(_double fDeltaTime);
	HRESULT						Update_State_Move(_double fDeltaTime);

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_EtcInfo();

private:
	EGOLEM_STATE				m_eCurState = STATE_SPAWNIDLE;
	EGOLEM_ANIM					m_eCurAnim = ANIM_SPAWN_IDLE;
	_float						m_fAnimSpeed;
	CPlayer*					m_pPlayer = nullptr;
	CCamera_Main*				m_pMainCam = nullptr;
	_uint						m_iCutSceneState = 0;
	_float						m_fDelayTime = 0.f;
	_bool						m_bOnceSwitch = false;
	_float						m_fEmissiveValue = 0.3f;
	_bool						m_bOncePlaySound = false;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CGrovetender*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;
};

END
