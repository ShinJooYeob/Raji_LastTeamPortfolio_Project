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

public:
	void	DrawOnOff(_bool State);

public:
	void	Set_GraphicUIOn(_bool State) { m_bIsGraphicUIOn = State; }
	void	Set_SoundUIOn(_bool State) { m_bIsSoundUIOn = State; }
	void	Set_PauseOn(_bool State) { m_bIsPauseOn = State; }

private:
	CRenderer*		m_pRendererCom = nullptr;

	_bool			m_bIsBtnClick = false;
	_bool			m_bIsGraphicUIOn = false;
	_bool			m_bIsSoundUIOn = false;

	_bool			m_bIsPauseOn = false;

	class CGraphicUI*	m_pGraphicUI = nullptr;
	class CSoundUI*		m_pSoundUI = nullptr;


	//Font
	FONTSDESC	m_BtnContinueFont;
	FONTSDESC	m_BtnGraphicFont;
	FONTSDESC	m_BtnSoundFont;
	FONTSDESC	m_BtnExitFont;

private:
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_pBackGroundImage = nullptr;

	vector<CUI*> m_vecBtns;


private:
	HRESULT Ready_Layer_UI();
	HRESULT Render_Fonts();


private:
	HRESULT SetUp_Components();

public:
	static CPauseUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END