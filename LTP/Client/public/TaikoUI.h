#pragma once
#include "GameObject.h"

BEGIN(Client)

class CTaikoUI final : public CGameObject
{
private:
	CTaikoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTaikoUI(const CTaikoUI& rhs);
	virtual ~CTaikoUI() = default;

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
	class CUI*	m_pNoteUI = nullptr;


	vector<CUI*> m_vecBasicUI;
	vector<CUI*> m_vecButtonUI;


private:

private:
	HRESULT Ready_Layer_UI();

private:
	HRESULT SetUp_Components();

public:
	static CTaikoUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END