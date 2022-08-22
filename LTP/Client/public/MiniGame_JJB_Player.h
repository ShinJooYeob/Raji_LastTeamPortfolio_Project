#pragma once
#include "GameObject.h"
#include "PlayerWeapon.h"

class CMiniGame_JJB_Player final : public CGameObject
{
private:
	explicit CMiniGame_JJB_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_JJB_Player(const CMiniGame_JJB_Player& rhs);
	virtual ~CMiniGame_JJB_Player() = default;

public:
	virtual HRESULT				Initialize_Prototype(void* pArg) override;
	virtual HRESULT				Initialize_Clone(void* pArg) override;


public:
	virtual _int				Update(_double fDeltaTime) override;
	virtual _int				LateUpdate(_double fDeltaTimer) override;
	virtual _int				Render() override;
	virtual _int				LateRender() override;

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CModel*							m_pModel = nullptr;
	CTransform*						m_pTransformCom = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	_float							m_fAnimSpeed = 1.f;

private:
	CPlayerWeapon*					m_pPlayerWeapons;

private:
	HRESULT						SetUp_Components();
	HRESULT						Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CMiniGame_JJB_Player*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

