#pragma once

#include "GameObject.h"

BEGIN(Client)

class CNonInstanceMeshEffect final : public CGameObject
{

private:
	CNonInstanceMeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNonInstanceMeshEffect(const CNonInstanceMeshEffect& rhs);
	virtual ~CNonInstanceMeshEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	HRESULT ReInitialize(_float3 vPosition, _float3 vLookDir);
	void	Set_GonnabeDie() { m_tMeshDesc.fMaxTime_Duration = m_fCurTime_Duration + m_tMeshDesc.fAppearTime; };
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
	static CNonInstanceMeshEffect*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
