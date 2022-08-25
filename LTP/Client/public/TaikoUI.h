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

public:
	class CUI*		Get_StartTimeUI() { return m_StartTimeUI; }
	class CUI*		Get_QuestUI() { return m_QuestUI; }

	void			Set_IsQuestUIStart();
	void			Set_IsShowComboUI()
	{
		m_bIsShowComboUIOn = true; 
		m_fShowComboUITime = 0.f;
	}
	void			Set_IsShowGoodEffectUI();
	void			Set_ShowResultUI(_int TextureIndex);

	void			UseComboUI(_double fDeltaTime);


private:
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_pNoteUI = nullptr;
	class CUI*	m_StartTimeUI = nullptr;
	class CUI*	m_QuestUI = nullptr;
	class CUI*  m_ResultUI = nullptr;


	vector<CUI*> m_vecBasicUI;
	vector<CUI*> m_vecButtonUI;
	vector<CUI*> m_vecComboUI;
	vector<CUI*> m_vecGoodEffectUI;

	_bool		m_bISStartTime = false;
	_bool		m_bIsQuestUI = false;
	_bool		m_bIsShowComboUIOn = false;
	_bool		m_bIsShowGoodEffectTime = false;
	_bool		m_bIsShowResultt = false;

	_float		m_TestTime = 0.f;
	_float		m_fQuestTime = 0.f;
	_float		m_fShowComboUITime = 0.f;
	_float		m_fShowGoodEffectUITime = 0.f;
	_float		m_fShowResultTime = 0.f;


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