#pragma once
#include "GameObject.h"

BEGIN(Client)

class CSoundUI final : public CGameObject
{
private:
	CSoundUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSoundUI(const CSoundUI& rhs);
	virtual ~CSoundUI() = default;

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
	void		UI_OnOff(_bool	UIOn) { m_bISUIOn = UIOn; }
	void		CheekBTN();

public:
	void		Set_PauseUIRef(void* UI);

private:
	CRenderer*		m_pRendererCom = nullptr;
	class CPauseUI* m_pPauseUI = nullptr;

	_bool			m_bISUIOn = false;

private:
	//UI
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_pBackGroundImage = nullptr;
	class CUI*  m_pBackBtn = nullptr;

	vector<CUI*> m_pSoundOptionUIs;
	vector<CUI*> m_pSoundBarUI;
	vector<CUI*> m_pVolumeRects;

	//Font
	FONTSDESC	m_SoundOptionName_Totla;
	FONTSDESC	m_SoundOptionName_Player;
	FONTSDESC	m_SoundOptionName_Monster;
	FONTSDESC	m_SoundOptionName_MapObject;
	FONTSDESC	m_SoundOptionName_Effect;
	FONTSDESC	m_SoundOptionName_UI;
	FONTSDESC	m_SoundOptionName_BGM;

	FONTSDESC	m_BtnBack;

private:
	HRESULT Ready_Layer_UI();
	HRESULT Render_Fonts();

private:
	HRESULT SetUp_Components();

public:
	static CSoundUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END