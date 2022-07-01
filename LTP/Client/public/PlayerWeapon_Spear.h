#pragma once

#include "PlayerWeapon.h"

BEGIN(Client)

class CPlayerWeapon_Spear final : public CPlayerWeapon
{
private:
	CPlayerWeapon_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Spear(const CPlayerWeapon_Spear& rhs);
	virtual ~CPlayerWeapon_Spear() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

private:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual void		Update_AttachCamPos() override;

	_int				Update_Structure(_double fDeltaTime);
	_int				Update_Equip(_double fDeltaTime);
	_int				Update_NoEquip(_double fDeltaTime);

	_int				LateUpdate_Structure(_double fDeltaTime);
	_int				LateUpdate_Equip(_double fDeltaTime);
	_int				LateUpdate_NoEquip(_double fDeltaTime);

	void				Update_AttachMatrix();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();

private:
	_float4x4			m_fAttachedMatrix;
	CHierarchyNode*		m_pAttachedNode = nullptr;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	static CPlayerWeapon_Spear*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END