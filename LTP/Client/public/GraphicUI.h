#pragma once

#include "GameObject.h"

BEGIN(Client)

class CGraphicUI final : public CGameObject
{
private:
	CGraphicUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGraphicUI(const CGraphicUI& rhs);
	virtual ~CGraphicUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_bool		Get_OnOffState() { return m_bISUIOn; }

public:
	void		Set_PauseUIRef(void* UI);

public:
	void		UI_OnOff(_bool	UIOn) { m_bISUIOn = UIOn; }
	void		CheekBTN();

private:
	CRenderer*		m_pRendererCom = nullptr;
	class CPauseUI* m_pPauseUI = nullptr;

	_bool			m_bISUIOn = false;

	//_bool			m_bIsBtnClick = false;

private:
	//UI
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_pBackGroundImage = nullptr;
	class CUI*  m_pBackBtn = nullptr;
	vector<CUI*> m_pGraphicOptionUIs;
	vector<CUI*> m_pBtns;

	//Font
	FONTSDESC	m_OptionName_Bloom;
	FONTSDESC	m_OptionName_DOF;
	FONTSDESC	m_OptionName_DDFOG;
	FONTSDESC	m_OptionName_Shadow;
	FONTSDESC	m_OptionName_GODRAY;
	FONTSDESC	m_OptionName_Lenseflare;

	FONTSDESC	m_BloomOnOFf;
	FONTSDESC	m_DOFOnOFf;
	FONTSDESC	m_DDFOGOnOFf;
	FONTSDESC	m_ShadowOnOFf;
	FONTSDESC	m_GODRAYOnOFf;
	FONTSDESC	m_LenseflareOnOFf;



	FONTSDESC	m_BtnBack;


private:
	HRESULT Ready_Layer_UI();
	HRESULT Render_Fonts();

private:
	HRESULT SetUp_Components();

public:
	static CGraphicUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END