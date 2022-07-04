#pragma once

#include "PlayerWeapon.h"

BEGIN(Client)

class CPlayerWeapon_Chakra final : public CPlayerWeapon
{
public:
	enum EChakraState { CHAKRA_IDLE, CHAKRA_MOV, CHAKRA_GOBACK, CHAKRA_END };

private:
	CPlayerWeapon_Chakra(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Chakra(const CPlayerWeapon_Chakra& rhs);
	virtual ~CPlayerWeapon_Chakra() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

public:
	void				Set_ChakraState(EChakraState eChakraState);
	EChakraState		Get_ChakraState();

private:
	_int				Update_IdleState(_double fDeltaTime);
	_int				Update_MovState(_double fDeltaTime);
	_int				Update_GoBackState(_double fDeltaTime);

private:
	void				Check_AttackStart();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();

private:
	EChakraState		m_eCurState = CHAKRA_IDLE;
	_float3				m_fAttackTargetPoint;
	_bool				m_bAttackStart = false;
	_float				m_fAnimSpeed = 1.f;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	static CPlayerWeapon_Chakra*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END


