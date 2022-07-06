#pragma once

#include "MonsterWeapon.h"

BEGIN(Client)

class CMonster_Weapon_Universal final : public CMonsterWeapon
{
public:
	enum Monster_Weapon_UniversalType {
		GADASURA_BLACK_WEAPON,
		GADASURA_RAGE_WEAPON,
		MONSTER_WEAPON_UNIVERSAL_END
	};
	typedef struct tagPlayerWeaponDesc
	{
		_uint			iMonsterWeaponMeshNumber;
		ATTACHEDESC		eAttachedDesc;
	}Monster_Weapon_UniversalDesc;

private:
	explicit CMonster_Weapon_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Weapon_Universal(const CMonster_Weapon_Universal& rhs);
	virtual ~CMonster_Weapon_Universal() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual	void Set_IsDead()override;

private:
	HRESULT SetUp_Components();

private:
	HRESULT	SetUp_Info();
	HRESULT	SetUp_BoneMatrix();
private:
	Monster_Weapon_UniversalDesc m_Monster_Weapon_UniversalDesc; //Monster_Bullet_Universal Desc;
	ATTACHEDESC			m_AttachedDesc; //Born Desc;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

public:
	static CMonster_Weapon_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END