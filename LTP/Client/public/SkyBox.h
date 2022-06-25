#pragma once

#include "GameObject.h"

BEGIN(Client)

class CSkyBox final : public CGameObject
{
private:
	CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

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

private:
	HRESULT SetUp_Components();

public:
	static CSkyBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END