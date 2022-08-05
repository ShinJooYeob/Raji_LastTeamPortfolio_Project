#pragma once

#include "GameObject.h"
#include "PlayerWeapon.h"
#include "TriggerObject.h"

BEGIN(Engine)
class CTimer;
class CNavigation;
END

BEGIN(Client)
class CCamera_Main;
class CShellingSkillRange;
class CTriggerObject;

class CGolu final : public CGameObject
{
public:
	enum EPLAYER_STATE {
		STATE_IDLE, STATE_MOV, STATE_FALL, STATE_LANDING, STATE_END
	};

	enum EINPUT_MOVDIR {
		MOVDIR_F, MOVDIR_B, MOVDIR_L, MOVDIR_R, MOVDIR_FL, MOVDIR_FR, MOVDIR_BL, MOVDIR_BR, MOVDIR_END
	};

	//**	   Enum_Anim		**//
	enum EPLAYERANIM_BASE {
		BASE_ANIM_IDLE, BASE_ANIM_WALK, BASE_ANIM_FALL, BASE_ANIM_LANDING, BASE_ANIM_END
	};

private:
	explicit CGolu(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGolu(const CGolu& rhs);
	virtual ~CGolu() = default;

public:
	virtual HRESULT				Initialize_Prototype(void* pArg) override;
	virtual HRESULT				Initialize_Clone(void* pArg) override;


public:
	virtual _int				Update(_double fDeltaTime) override;
	virtual _int				LateUpdate(_double fDeltaTimer) override;
	virtual _int				Render() override;
	virtual _int				LateRender() override;


	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;
	
public: /* Damage Logic*/
	virtual _float				Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;
	_float						Apply_Damage(CGameObject* pTargetObject, _float fDamageAmount, _bool bKnockback);

public:
	// Idle
	void						Set_State_IdleStart(_double fDeltaTime);

	// Move
	void						Set_State_MoveStart(_double fDeltaTime);

	// Falling
	void						Set_State_FallingStart(_double fDeltaTime);

	// Falling
	void						Set_State_LandingStart(_double fDeltaTime);

public:
	void						Set_GameOver();

private:
	HRESULT						Update_State_Idle(_double fDeltaTime);
	HRESULT						Update_State_Move(_double fDeltaTime);
	HRESULT						Update_State_Fall(_double fDeltaTime);
	HRESULT						Update_State_Landing(_double fDeltaTime);

	HRESULT						Update_Collider(_double fDeltaTime);

private: /* Key Input */
	_bool						Check_Mov_KeyInput(_double fDeltaTime);

private: /* Actions */
	void						Move(EINPUT_MOVDIR eMoveDir, _double fDeltaTime);
	
private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_Colliders();
	HRESULT						SetUp_EtcInfo();

private: /* private Setter */
	void Set_InputDir(_int iAxis_F, _int iAxis_R, _double fDeltaTime);

private: /* Key Input State */
	EINPUT_MOVDIR		m_eInputDir = MOVDIR_END;
	
private: /* Enum Stats */
	_uint					m_eCurAnim = BASE_ANIM_IDLE;
	EPLAYER_STATE			m_eCurState = STATE_IDLE;

private: /* Animation Control */
	_float					m_fAnimSpeed = 1.f;
	_float3					m_fMovDir;

	_float					m_fJumpStart_Y = 0.f;
	_float					m_fFallingStart_Y = 0.f;
	_float					m_fFallingAcc = 0.f;
	_float					m_fJumpPower = 0.f;

	_bool					m_bGameOver = false;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

	CCollider*				m_pCollider = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc;


	CCollider*				m_pCollider_GameClear = nullptr;

public:
	static CGolu*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END
