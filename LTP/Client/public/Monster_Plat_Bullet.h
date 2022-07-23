#pragma once

#include "Monster_Bullet.h"

BEGIN(Client)

class CMonster_Plat_Bullet final : public CMonster_Bullet
{

private:
	CMonster_Plat_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_Plat_Bullet(const CMonster_Plat_Bullet& rhs);
	virtual ~CMonster_Plat_Bullet() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;


private:
	HRESULT				SetUp_Components();

private:
	NONINSTNESHEFTDESC		m_tMeshDesc;
	_float					m_fCurTime_Duration = 0.f;

	_float3				m_vRotAxis = _float3(0, 1, 0);
	_float3				m_vMoveDir = _float3(0, 1, 0);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pParentTranscom = nullptr;



public:
	static CMonster_Plat_Bullet*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
