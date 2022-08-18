#pragma once

#include "GameObject.h"

BEGIN(Client)
class CCamera_Main;
class CBeachBall;
class CMiniGame_Jino_Player  final : public CGameObject
{
public:
	enum EPLAYER_STATE {
		STATE_IDLE, STATE_MOV, STATE_JUMP, STATE_LOSE_BEACHBALL, STATE_THROWAWAY_BEACHBALL_JUMP, STATE_DEAD, STATE_END
	};

	enum EPLAYER_ANIM {
		ANIM_IDLE, ANIM_MOV_F, ANIM_MOV_B, ANIM_JUMP, ANIM_END
	};

private:
	explicit CMiniGame_Jino_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_Jino_Player(const CMiniGame_Jino_Player& rhs);
	virtual ~CMiniGame_Jino_Player() = default;

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
	// Idle
	void						Set_State_IdleStart();

	// Move
	void						Set_State_MoveStart();

	// Jump
	void						Set_State_JumpStart();

	// Lose BeachBall
	void						Set_State_LoseBeachBall();

	// Conflict BeachBall
	void						Set_State_ConflictBeachBall();

	// Throw Away BeachBall
	void						Set_State_ThrowAwayBeachBall_Jump();

	// Dead
	void						Set_State_DeadStart();

public:
	void						Set_GameStart();
	void						Set_GameOver();

public:
	void						Set_BeachBall(CBeachBall* pBeachBall);
	EPLAYER_STATE				Get_PlayerState();
	_bool						Is_Falling();

private:
	HRESULT						Update_State_Idle(_double fDeltaTime);
	HRESULT						Update_State_Move(_double fDeltaTime);
	HRESULT						Update_State_Jump(_double fDeltaTime);
	HRESULT						Update_State_LostBeachBall(_double fDeltaTime);
	HRESULT						Update_State_ThrowAwayBeachBall(_double fDeltaTime);
	HRESULT						Update_State_Dead(_double fDeltaTime);

	HRESULT						Update_Collider(_double fDeltaTime);
	HRESULT						Update_BeachBallPos();

private:
	void						Check_PlayerInput();
	void						Check_Move_OutOfStage();

private:
	void						Set_PhysX_Head();

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_Colliders();
	HRESULT						SetUp_EtcInfo();

private:
	EPLAYER_STATE					m_eCurState = STATE_IDLE;

private:
	_float							m_fAnimSpeed = 1.f;

	_float							m_fJumpStart_Y = 0.f;
	_float							m_fJumpEnd_Y = 0.f;
	_float							m_fFallingAcc = 0.f;
	_float							m_fJumpPower = 0.f;

	_bool							m_bBlock_KeyInput = false;

	_bool							m_bGameStart = false;
	_bool							m_bGameOver = false;

	CBeachBall*						m_pBeachBall = nullptr;
	CTransform*						m_pBeachBallTransform = nullptr;

	_float							m_fDurTime_Invincibility = 0.f;

	_bool							m_bIsFalling = false;

	_bool							m_bRimLightArrow = false;
	_float							m_fRimLightValue = 0.f;
	_bool							m_bOnRimLight = false;

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CModel*							m_pModel = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CDissolve*						m_pDissolveCom = nullptr;

	CCollider*						m_pCollider = nullptr;
	vector<ATTACHEDESC>				m_vecAttachedDesc;

	class CCollider_PhysX_Joint*	m_pHeadJoint = nullptr;

public:
	static CMiniGame_Jino_Player*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END
