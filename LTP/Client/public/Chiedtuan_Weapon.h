#pragma once
#include "MonsterWeapon.h"
class CChiedtuan_Weapon final : public CMonsterWeapon
{
private:
	CChiedtuan_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CChiedtuan_Weapon(const CChiedtuan_Weapon& rhs);
	virtual ~CChiedtuan_Weapon() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	_float				m_fAttackCoolTime = 5.f;
	_float				m_fSkillCoolTime = 8.f;
	_bool				m_bIsHalf = false;
	_bool				m_bIsLookAt = true;

	CGameObject*		m_pPlayerObj;

	_float				m_fTestHPIndex = 1.f;

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CChiedtuan_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

