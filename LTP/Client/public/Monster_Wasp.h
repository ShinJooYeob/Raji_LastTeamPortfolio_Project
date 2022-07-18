#pragma once
#include "Monster.h"

BEGIN(Client)

class CMonster_Wasp final : public CMonster
{
	enum AnimationType{ANIM_RUN_Frame1, ANIM_RUN_Frame2, ANIM_ATTACK_Frame1, ANIM_ATTACK_Frame2, ANIM_ATTACK_Frame3, ANIM_ATTACK_Frame4, ANIM_ATTACK_Frame5, ANIM_END};

	typedef struct tagState
	{
		CTransform*		pTransform = nullptr;
		CNavigation*	pNavigation = nullptr;
		_uint			iType = ANIM_END;

		_int			iHp = 3;
		_bool			bHit = false;
		_bool			bOnceSwtich = false;
		_double			dTime = 0;
	}TRANSFORM_STATE;

private:
	CMonster_Wasp(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_Wasp(const CMonster_Wasp& rhs);
	virtual ~CMonster_Wasp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

private:
	HRESULT	SetUp_Info();

private:
	HRESULT FollowMe(_double dDeltaTime);
	HRESULT	Update_Collider(_double fDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

	CModel*				m_pModel[ANIM_END] = {nullptr};
	CModelInstance*		m_pModelInstance[ANIM_END] = { nullptr };

	vector<TRANSFORM_STATE> m_vecInstancedTransform;

	vector<CTransform*> m_ModelTransGroup[ANIM_END];

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

	_uint				m_iTempAnimNumber = 0;

	_double				m_dAcceleration = 1.5;
private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

public:
	static CMonster_Wasp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
