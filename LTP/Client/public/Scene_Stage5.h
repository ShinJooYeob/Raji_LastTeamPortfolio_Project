#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_Stage5 final : public CScene
{
private:
	explicit CScene_Stage5(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_Stage5() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Change_to_NextScene() override;

	virtual void Play_Scene_BGM() override;

public:
	void	Start_EndingCinematic();
	void	Start_Ending();
	
public:
	void	Set_EndGame();
	void	GameEnd();

private:
	HRESULT Ready_Light();
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar * pLayerTag);
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_TestMapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Boss(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	

	HRESULT	Ready_MapData(const _tchar* szMapDataFileName, SCENEID eSceneID, const _tchar* pLayerTag);
	HRESULT	Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);
	HRESULT	Ready_MonsterBatchTrigger(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);


	HRESULT Ready_PostPorcessing();

	HRESULT Ready_CinematicCamAction();
	
private:
	CAMERAACTION						m_tCamActionDesc;
	_bool								m_bActive = false;
	_bool								m_bEndGame = false;

private:
	class CCamera_Main*					m_pMainCam = nullptr;
	CUtilityMgr*						m_pUtilMgr = nullptr;
	CTransform*							m_pPlayerTransform = nullptr;

public:
	static CScene_Stage5* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void Free() override;
};

END