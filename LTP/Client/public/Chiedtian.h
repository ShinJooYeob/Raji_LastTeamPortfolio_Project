#pragma once
#include "Boss.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CChiedtian final : public CBoss
{
public:
	enum AttackNumber{ATTACK_FIRE, ATTACK_SPIN, ATTACK_WHINING, ATTACK_END};
	enum AttackSecondPage {SECONDPAGEATTACK_SPIN, SECONDPAGEATTACK_VOLCANO, SECONDPAGEATTACK_END};
private:
	CChiedtian(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CChiedtian(const CChiedtian& rhs);
	virtual ~CChiedtian() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

public:
	// JH
	void				Change_Animation(_uint iAnimIndex);
	void				Set_Dissolve_InOut(_bool bIsIn);
	//

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pFireParticleTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;


	//몸체
	CCollider*				m_pCollider = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc;

	//화염
	CCollider*				m_pFireCollider = nullptr;
	ATTACHEDESC				m_FireAttachedDesc;

	//점프
	CCollider*				m_pJumpCollider = nullptr;
	ATTACHEDESC				m_JumpAttachedDesc;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	_float				m_fAttackCoolTime = 4.f;
	_float				m_fSkillCoolTime = 6.f;
	_bool				m_bIsHalf = false;
	_bool				m_bIsLookAt = true;

	//Jump
	_float				m_fJumpTime = 3.f;
	_bool				m_bIsTurn = false;
	_bool				m_bIsJump = false;



	//Attack
	_bool				m_bIsBasicAttack = false;
	_bool				m_bISkill = false;
	//walk
	_bool				m_bIsWalk = false;
	_float				m_fWalkTime = 3.f;

	//fire Attack
	_bool				m_bIsFireAttack = false;
	_float				m_fFireTime = 10.f;
	//Spin Attack
	_bool				m_bIsSpinAttack = false;
	_float				m_fSpinSpeed = 0.f;
	_float				m_fSpinTime = 14.f;

	class CHpUI*		m_pHPUI = nullptr;

	_double				m_OldPlayRate = 0;


	CGameObject*		m_pPlayerObj;
	_Vector				m_vAngle;

	_float				m_fTestHPIndex = 1.f;
	_int				m_iTest = 0;

	vector<class CChiedtuan_Weapon*>		m_pMainWeapons;
	_bool									m_bIsMainWeaponOff = false;
	vector<class CChiedtuan_Weapon*>		m_pSubWeapons;

	_float				m_fNarration = 15.f;

	_bool				m_bIsAtaackAimEnd = false;

/*--------------------------------------------------------------------------------------------------------*/
	//2Stage
	_bool										m_bMiddlepointPos = false;
	_bool										m_ActivateSecondPage = false;
	vector<class CChiedtuan_2Page_Weapon*>		m_pSecondPageWeapons;

	//Volcano Ataack
	_bool				m_bIsVolcanoAttack = false;
	_bool				m_bISVolcanoAttackStart = false; //점프 애니메이션에서 사용(체인지애님막을라고)
	_float				m_fVolcanoTime = 10.f;


private:
	void			Activate_SecondPage(_double fDeltaTime);
	

	vector<NONINSTNESHEFTDESC>	m_vecNonInstMeshDesc;
	vector<INSTPARTICLEDESC>	m_vecTexInstDesc;
	_bool						m_bIs2PageOnceJump = false;

private:
	// JH
	void				Update_Direction(_double fDeltaTime);

	CDissolve*			m_pDissolve = nullptr;
	_bool				m_bBlockUpdate = false;
	_bool				m_bBlockAnim = false;
	_bool				m_bOnceSwitch = false;
	_bool				m_bOncePlaySound = false;
	_uint				m_iNextAnimIndex = 0;
	_float				m_fDelayTime = 0.f;
	//

private:
	HRESULT SetUp_Components();
	HRESULT Ready_ParticleDesc();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CChiedtian* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END