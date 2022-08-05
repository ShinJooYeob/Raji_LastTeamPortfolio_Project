#pragma once
#include "GameObject.h"

BEGIN(Client)

class CPauseUI final : public CGameObject
{
private:
	CPauseUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPauseUI(const CPauseUI& rhs);
	virtual ~CPauseUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CRenderer*		m_pRendererCom = nullptr;

private:
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_pBackGroundImage = nullptr;


private:
	HRESULT Ready_Layer_UI();

private:
	HRESULT SetUp_Components();

public:
	static CPauseUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END