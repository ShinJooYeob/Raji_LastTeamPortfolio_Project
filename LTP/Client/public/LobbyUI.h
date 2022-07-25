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

private:
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_RajiText = nullptr;
	class CUI*	m_pPressEnter = nullptr;

private:
	HRESULT Ready_Layer_UI();

private:
	HRESULT SetUp_Components();


public:
	static CLobbyUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END