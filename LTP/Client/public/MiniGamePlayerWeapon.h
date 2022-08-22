#pragma once
#include "PlayerWeapon.h"

BEGIN(Client)


class CMiniGamePlayerWeapon final : public CPlayerWeapon
{
private:
	CMiniGamePlayerWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMiniGamePlayerWeapon(const CMiniGamePlayerWeapon& rhs);
	virtual ~CMiniGamePlayerWeapon() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

private:
	void				Update_AttachMatrix();

private:
	HRESULT				SetUp_Components();

private:
	_float4x4			m_fAttachedMatrix;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	static CMiniGamePlayerWeapon*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END