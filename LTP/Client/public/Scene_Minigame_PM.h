#pragma once

#include "Scene.h"

BEGIN(Client)

class CScene_Minigame_PM final : public CScene
{
private:
	explicit CScene_Minigame_PM(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_Minigame_PM() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Change_to_NextScene() override;



private:
	HRESULT Ready_Light();
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);

	HRESULT	Ready_MapData(const _tchar* szMapDataFileName, SCENEID eSceneID, const _tchar* pLayerTag);
	HRESULT	Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);
	HRESULT	Ready_MonsterBatchTrigger(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT Ready_MiniGameBuilding(const _tchar* pLayerTag);


	HRESULT Ready_PostPorcessing();


private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;
	CTransform*						m_pGoluTransform = nullptr;

public:
	static CScene_Minigame_PM* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void Free() override;
};

END