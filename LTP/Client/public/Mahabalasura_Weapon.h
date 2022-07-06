#pragma once
#include "MonsterWeapon.h"

BEGIN(Client)
class CMahabalasura_Weapon final : public CMonsterWeapon
{
private:
	CMahabalasura_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura_Weapon(const CMahabalasura_Weapon& rhs);
	virtual ~CMahabalasura_Weapon() = default;


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

	ATTACHEDESC			m_eAttachedDesc;
	_float4x4			m_fAttachedMatrix;

private:
	void				Update_AttachMatrix();

private:
	HRESULT				SetUp_Components();

public:
	static CMahabalasura_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END