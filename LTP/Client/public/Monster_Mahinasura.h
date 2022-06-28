#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)
class CMonster_Mahinasura final : public CGameObject
{
private:
	CMonster_Mahinasura(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_Mahinasura(const CMonster_Mahinasura& rhs);
	virtual ~CMonster_Mahinasura() = default;

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
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;


	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

	_int				m_iTest = 0;

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CMonster_Mahinasura* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END