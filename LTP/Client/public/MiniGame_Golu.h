#pragma once

BEGIN(Client)
class CCamera_Main;

class CMiniGame_Golu final : public CGameObject
{
private:
	explicit CMiniGame_Golu(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_Golu(const CMiniGame_Golu& rhs);
	virtual ~CMiniGame_Golu() = default;

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

public:
	virtual void Update_AttachCamPos()override;

	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

	HRESULT	SetUp_Info();
	HRESULT SetUp_UI();

	HRESULT	Camera_Walking(_double dDeltaTime);

//InGame
	HRESULT	Play_MiniGame(_double dDeltaTime);

//Input
	HRESULT	Keyboard_Input(_double dDeltatime);
	HRESULT	SkillNumber_Input(_double dDeltatime);
	HRESULT	Skill_Input(_double dDeltatime);

private:
	HRESULT	LookAt_MousePos(_float fWeight); //차크라의 회전할 때 사용되는것
	_float3	Check_MousePicking(); //플레이어에선 월드 좌표 픽킹이였으나 미니게임 골루에선 직교투영 픽킹으로 변환됨


private:
	HRESULT Ready_TriggerObject(const _tchar* szTriggerDataName, SCENEID eSceneID, const _tchar* pLayerTag);
	HRESULT	Ready_Round();


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	class CHpUI*		m_pHPUI = nullptr;
	CCamera_Main*		m_pMainCamera = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

private://Dissolve
	_double				m_dDissolveTime = 0;

private://Anim Speed
	_double				m_dAcceleration = 1;


//Attack
private:
	_int				m_iSkillNumber = 1;
	_double				m_dBarrierCoolTime = 10;
	_double				m_dBlackHoleCoolTime = 10;

private://Round
	_bool				m_bNextRoundOn = true;
	_uint				m_iNextRoundNumber = 1;

public:
	static CMiniGame_Golu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END