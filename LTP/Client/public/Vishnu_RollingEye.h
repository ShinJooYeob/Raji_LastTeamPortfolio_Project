#pragma once

#include "GameObject.h"

BEGIN(Client)
class CRepelWall;

class CVishnu_RollingEye  final : public CGameObject
{
private:
	explicit CVishnu_RollingEye(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVishnu_RollingEye(const CVishnu_RollingEye  & rhs);
	virtual ~CVishnu_RollingEye() = default;

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
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_uint				m_iKinds = 0;

public:
	static CVishnu_RollingEye*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
