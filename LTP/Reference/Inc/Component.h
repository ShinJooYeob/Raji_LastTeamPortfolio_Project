#pragma once

#include "Base.h"


BEGIN(Engine)
class ENGINE_DLL CComponent abstract :public CBase
{
protected:
	explicit CComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);

	_bool	Get_IsOwnerDead() { return m_bIsOwnerDead; };
	void	Set_IsOwnerDead(_bool	bBool = true) { m_bIsOwnerDead = bBool; };



public:
	virtual HRESULT Render() { return S_OK; }
#ifdef _DEBUG
#endif // _DEBUG

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	_bool					m_bIsClone = false;
	_bool					m_bIsOwnerDead = false;


public:
	virtual CComponent* Clone(void* pArg = nullptr)PURE;
	virtual void  Free() override;
};

END	