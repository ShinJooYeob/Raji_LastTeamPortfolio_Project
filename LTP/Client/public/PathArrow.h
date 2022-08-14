#pragma once

#include "GameObject.h"

BEGIN(Client)


class CPathArrow final : public CGameObject
{
private:
	CPathArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPathArrow(const CPathArrow& rhs);
	virtual ~CPathArrow() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	void	Set_Appear(_bool bAppear);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_Etc();

private:
	CTransform*			m_pPlayerTransform = nullptr;
	CTransform*			m_pGoluTransform = nullptr;
	_float				m_fMovDir = 0.1f;
	_float				m_fMax_Dist = 3.f;
	_float				m_fMin_Dist = 2.f;
	_float				m_fCur_Dist = 2.f;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;
	CDissolve*			m_pDissolveCom = nullptr;

public:
	static CPathArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
