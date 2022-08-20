#pragma once

BEGIN(Client)
class CCamera_Main;

class CMiniGame_KongRaji final : public CGameObject
{
public:
	enum AnimType{ANIM_IDLE,ANIM_WALK,ANIM_CLIMB_UP = 121, ANIM_CLIMB_DOWN, ANIM_JUMP = 143,ANIM_END};

	typedef	struct tagJumpDesc
	{
		_bool	bJump;
		_float	fJumpY, fJumpPower;
		_double	dTime;
	}JUMPDESC;
private:
	explicit CMiniGame_KongRaji(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_KongRaji(const CMiniGame_KongRaji& rhs);
	virtual ~CMiniGame_KongRaji() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void Set_IsDead();

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

public:
	virtual void Update_AttachCamPos()override;

	HRESULT				SetUp_Components();
	HRESULT				Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT				SetUp_Info();
	HRESULT				SetUp_Texture();

	HRESULT				SetUp_Collider();
	HRESULT				Update_Collider(_double dDeltaTime);

	//Camera
	HRESULT				Camera_Pos();

	//InGame
	HRESULT				Play_MiniGame(_double dDeltaTime);

	//Input
	HRESULT				Keyboard_Input(_double dDeltatime);
	HRESULT				Change_Anim();

	//Piput
	HRESULT				Pivot();
	//Jump
	HRESULT				Jumping(_double TimeDelta);


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

	CCamera_Main*		m_pMainCamera = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

	//점프하기
private:
	JUMPDESC			m_JumpDesc;

private:
	_bool				m_bChangeAnimOn = false;
	_uint				m_iAnimIndex = 0;
	_uint				m_iCurrentAnimIndex = 0;


private://Dissolve
	_double				m_dDissolveTime = 0;

private://Anim Speed
	_double				m_dAcceleration = 1;

public:
	static CMiniGame_KongRaji* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END