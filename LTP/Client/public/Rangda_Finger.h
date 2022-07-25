#pragma once
#include "GameObject.h"


class CRangda_Finger final : public CGameObject
{
private:
	CRangda_Finger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRangda_Finger(const CRangda_Finger& rhs);
	virtual ~CRangda_Finger() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;
	CDissolve*			m_pDissolveCom = nullptr;

	_uint				m_iPassIndex = 1;

	ATTACHEDESC			m_AttacheDesc;
	_float				m_fAliveTime = 4.f;

	_bool				m_bIsDissolveStart = false;

private:
	HRESULT SetUp_Components();

public:
	static CRangda_Finger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

