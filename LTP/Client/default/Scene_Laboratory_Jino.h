#pragma once

#include "Scene.h"

BEGIN(Client)
class CDynamicPlatform;
class CScene_Laboratory_Jino final : public CScene
{
private:
	explicit CScene_Laboratory_Jino(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_Laboratory_Jino() = default;

public:
	virtual HRESULT		Initialize();
	virtual _int		Update(_double fDeltaTime)override;
	virtual _int		LateUpdate(_double fDeltaTime)override;
	virtual _int		Render()override;
	virtual _int		LateRender()override;

public:
	void				Set_PlatformIndex_Raji(_uint iIndex);
	void				Set_PlatformIndex_Golu(_uint iIndex);

	void				Update_Game();
	void				Check_GameOver();

	void				Set_GameClear();

private:
	HRESULT				Ready_Light();
	HRESULT				Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Player(const _tchar * pLayerTag);
	HRESULT				Ready_Layer_Objects(const _tchar * pLayerTag);


	HRESULT				Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);
	HRESULT				Ready_PostPorcessing();

private:
	vector<CDynamicPlatform*>		m_vecDynamicPlatforms_Raji;
	_uint							m_iPlatformIndex_Raji = 0;
	CTransform*						m_pTransform_Raji = nullptr;
	class CPlayer*					m_pPlayer = nullptr;

	vector<CDynamicPlatform*>		m_vecDynamicPlatforms_Golu;
	_uint							m_iPlatformIndex_Golu = 0;
	CTransform*						m_pTransform_Golu = nullptr;
	class CGolu*					m_pGolu = nullptr;

	_bool							m_bGameOver = false;
	_float							m_fGameOverDelay = 11.f;

	CDynamicPlatform*				m_pEndPlatform = nullptr;

	_bool							m_bEndUpdate = false;
	_bool							m_bGameClear = false;
	_bool							m_bOncePlaySound = false;

private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;


public:
	static CScene_Laboratory_Jino*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void					Free() override;
};

END