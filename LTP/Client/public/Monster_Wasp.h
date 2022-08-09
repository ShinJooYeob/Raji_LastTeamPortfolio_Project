#pragma once
#include "Monster.h"

BEGIN(Client)

class CMonster_Wasp final : public CMonster
{
	enum AnimationType { ANIM_RUN_Frame1, ANIM_RUN_Frame2, ANIM_ATTACK_Frame1, ANIM_ATTACK_Frame2, ANIM_ATTACK_Frame3, ANIM_ATTACK_Frame4, ANIM_ATTACK_Frame5, ANIM_END };
	enum RenderType { RENDER_IDLE, RENDER_HIT, RENDMER_DIE, RENDER_END };

	typedef struct tagState
	{
		CTransform*		pTransform = nullptr;
		CNavigation*	pNavigation = nullptr;
		CCollider*		pCollider = nullptr;
		_uint			iAnimType = ANIM_END;
		_uint			iRenderType = RENDER_IDLE;
		_uint			iCellIndex = 0;

		_float4			fRimRight = _float4(0.5f, 0.5f, 0.5f, 1.f);
		_float4			fEmissive = _float4(0.5f, 0.5f, 0.5f, 0.8f); //R,G,B W==±¹¹ä°ª

		_float4			fDissolve = _float4(0.f, 1.f, 0.5f, 1.f);//1Èå¸¥½Ã°£, µðÁ¹ºê ½Ã°£, ±¹¹ä°¡°Ý ,0ÀÌ¸é µðÁ¹ºê ¾ÈÇÔ 1ÀÌ¸é µðÁ¹ºê


		_int			iHp = 3;
		_double			dTime = 0;
		_bool			bHit = false;
		_int			iSwtichIndex = 0;

		_uint			iLifeCount = 0;
		_bool			bDieOn = false;
	}TRANSFORM_STATE;

	typedef struct tagInstanceInfo
	{
		_float4x4 fValueMat;  //m[0][0] : MeshInstanceMonsterEnum, m[0][1] : Monster Max Size, m[0][2] : Cell Max Size, m[0][3] : Cell Count,
							//m[1][0] : Life Count, m[1][1] : Monster Hit Count, m[1][2] : TriggerDistance,
							//m[2][0]~m[2][2] : X,Y,Z
		_float4x4 fSubValueMat;
		void*		Object = nullptr;
	}INSTANCE_INFO;

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

private:
	HRESULT SetUp_Components();

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

private:
	HRESULT	SetUp_Info();

private:
	HRESULT FollowMe(_double dDeltaTime);
	HRESULT	Update_VectorGroup(_double dDeltaTime);
	HRESULT	Update_Collider(_double dDeltaTime);
	HRESULT	Update_Render(_double dDeltaTime);
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pAttackColliderCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

	CModel*				m_pModel[ANIM_END] = { nullptr };
	CModelInstance*		m_pModelInstance[ANIM_END] = { nullptr };

	vector<TRANSFORM_STATE> m_vecInstancedTransform;

	vector<CTransform*> m_ModelTransGroup[ANIM_END];
	vector<_float4>		m_vecRimLight[ANIM_END];
	vector<_float4>		m_vecEmissive[ANIM_END];
	vector<_float4>		m_vecDissolve[ANIM_END];//1Èå¸¥½Ã°£, µðÁ¹ºê ½Ã°£, ±¹¹ä°¡°Ý ,0ÀÌ¸é µðÁ¹ºê ¾ÈÇÔ 1ÀÌ¸é µðÁ¹ºê


	INSTANCE_INFO		m_Instance_Info;
	const _tchar*		m_charModellInstanceType = nullptr;
	class CInstanceMonsterBatchTrigger*	m_pBatchTrigger = nullptr;



	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

	_uint				m_iTempAnimNumber = 0;

	_double				m_dAcceleration = 1.5;

	_bool				m_bAttackOn = false;

private:
	_uint				m_iSoundIndex[ANIM_END] = {};
	_bool				m_bSoundSwitch[ANIM_END] = {};

private:
	_uint				m_iDieCount = 0;

public:
	static CMonster_Wasp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
