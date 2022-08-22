#pragma once
#include "GameObject.h"

BEGIN(Client)

class CLobbyUI final : public CGameObject
{
public:
	enum WeaponType {WEAPON_BOW, WEAPON_SPEAR, WEAPON_SWORDSHIELD, WEAPON_END};
private:
	CLobbyUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CLobbyUI(const CLobbyUI& rhs);
	virtual ~CLobbyUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	void	SetStageOnOff(_bool State) { m_bIsStageOn = State; }

private:
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_pBackGround2 = nullptr;
	class CUI*	m_RajiText = nullptr;

	vector<CUI*> m_vecLobbyBtns;

	vector<CUI*> m_vec3DStageUI;

private:
	_bool			m_bIsStageOn = false;
	_bool			m_bIsStageOnStart = false;
	_bool			m_bIsRajiTextOn = true;

	_int			m_iSelectScene = 0;

private:
	HRESULT Ready_Layer_UI();

	void	ChangeStageNumber();

private:
	HRESULT SetUp_Components();


public:
	static CLobbyUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END