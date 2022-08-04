#pragma once
#include "MonsterWeapon.h"

BEGIN(Client)

class CChiedtuan_2Page_Weapon  final : public CMonsterWeapon
{
public:
	enum KatanaPOSTYPE { KATANA_TR, KATANA_TL, KATANA_BR, KATANA_BL };

	typedef struct WeaponDesc
	{
		CGameObject* BossObj = nullptr;
		KatanaPOSTYPE KatanaPOSType;
	}WEAPOPNDESC;

private:
	CChiedtuan_2Page_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CChiedtuan_2Page_Weapon(const CChiedtuan_2Page_Weapon& rhs);
	virtual ~CChiedtuan_2Page_Weapon() = default;

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

public:
	void	Set_WeaponDesc(WEAPOPNDESC WeaponDesc) {
		m_WeaponDesc = WeaponDesc;
	}

	void	Set_InitializSpinAttackDistance();
	void	Set_SecondPageWeaponStart();
	void	Set_WeaponPosition();
	void	Set_Attack(_double fDeltaTime);
	void	Set_AttackfinishOff(_bool State) { m_bIsAttackfinish = State; }
	void	Set_PlayerPos(_float3 PlayerPos) { m_vPlayerPos = PlayerPos; }
	void	Set_SpinAttack(_bool State) { m_bIsSpinAttack = State; }
	void	Set_VolcanoAttack(_bool State) { m_bIsVolcanoAttack = State; }
	void	Set_Dissolve(_bool FadeIn, _double Time = 1.f);
	void	Set_InitialPosDessolve(_bool State) { m_bIsInitialPosDessolve = State; }

public:
	void	WeaponSpinAttack(_double fDeltaTime);
	void	SpinAttackOff();
	void	VolcanoAttackOff();
	void	BeginningPos();
	void	VolcanoAttack(_double fDeltaTime);
private:
		CRenderer*			m_pRendererCom = nullptr;
		CShader*			m_pShaderCom = nullptr;
		CModel*				m_pModel = nullptr;
		CTransform*			m_pTransformCom = nullptr;
		CDissolve*			m_pDissolveCom = nullptr;
		CCollider*			m_pCollider = nullptr;

		CGameObject*		m_PlayerObj = nullptr;
		_float3				m_vPlayerPos;

		WEAPOPNDESC			m_WeaponDesc;
		_float				m_WeaponMoveTime = 0;

		//Attack
		_bool				m_bIsAttackfinish = false;

		//SpinAttack
		_bool				m_bIsBeginningPos = true;
		_bool				m_bIsSpinAttack = false;
		_bool				m_bIsDistance = false;
		_float				m_fDistance = 20.f;

		//VolcanoAttack
		_bool				m_bIsVolcanoAttack = false;

		_bool				m_bIsInitialPosDessolve = false;
		_bool				m_DeadDessolve = false;

		_float				m_fAngle = 0.f;
private:
	HRESULT SetUp_Components();

public:
	static CChiedtuan_2Page_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END